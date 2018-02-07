/**
 * @file Memory.cpp
 * @brief Implementation of Memory functions
 *
 * @author Deep Dhillon
 * @date November 19, 2017
 * @bug No known bugs
 */

#include <string.h>

#include "Memory.h"
#include "StaticAllocatorPool.h"
#include "DynamicAllocatorPool.h"

#include "wlib/Wlib.h"
#include "wlib/utility/Math.h"

using namespace wlp;

static constexpr uint8_t OBJECT_INFO_BUFFER = 2; // 2 bytes to store object info
static constexpr size_type REQUIRED_EXTRA_BUFFER = sizeof(__Allocator *);

/**
 * This class restricts the size of blocks as the power of 2 gets bigger
 */
class RestrictSize {
    static constexpr uint16_t numRestrictions = 3;      // number of restrictions imposed
    static constexpr uint16_t powOffsetFromZero = 9;    // which pow of 2 to start restriction from

    static constexpr uint16_t restrictions[numRestrictions] = {
            300, 400, 500
    };

public:
    /**
     * If the @p blockSize is smaller within restriction range, apply restrictions
     * otherwise return the same block
     *
     * @tparam pow current power of 2
     * @tparam blockSize current size of a memory block
     * @return a memory block that is restricted
     */
    template<size_type pow, size32_type blockSize>
    static constexpr inline size32_type apply() {
        return (pow >= powOffsetFromZero &&
                pow < (powOffsetFromZero + numRestrictions)) ? restrictions[pow - powOffsetFromZero]
                                                             : blockSize;
    }
};

static __Allocator *__allocators[MAX_ALLOCATORS];
int MemoryInitDestroy::m_srefCount = 0;

/**
 * Creates and initializes the memory management. It creates the memory based
 * on configurations set up in @code MemoryConfiguration.h @endcode file
 */
void memory_init();

/**
 * Deletes and disposes the allocations done internally. If a pool is used it
 * promises to deallocates any pending allocations even if @code free @endcode
 * is not used
 */
void memory_destroy();

MemoryInitDestroy::MemoryInitDestroy() {
    if (m_srefCount++ == 0) { memory_init(); }
}

MemoryInitDestroy::~MemoryInitDestroy() {
    if (--m_srefCount == 0) { memory_destroy(); }
}

/**
 * Template struct to create allocators. This recursively creates allocators and hence makes
 * a memory pool. This is the recursive step of the process
 *
 * @tparam powStart starting power of 2 for memory of Allocator
 * @tparam from number to start from
 * @tparam to number to end at
 */
template<uint16_t powStart, uint16_t from, uint16_t to>
struct insert {
    /**
     * Insert either static or dynamic memory blocks
     */
    static void apply() {
        constexpr size_type curr_pow = from + powStart;
        constexpr size32_type blockSize = RestrictSize::apply<curr_pow, pow_const<size32_type>(2, curr_pow)>();

#if defined(DYNAMIC_POOL)
        __allocators[from] = new DynamicAllocatorPool<blockSize, NUM_BLOCKS>();
#elif defined(STATIC_POOL)
        __allocators[from] = new StaticAllocatorPool<blockSize, NUM_BLOCKS>();
#endif
        insert<powStart, from + 1, to>::apply();
    }
};

/**
 * Template struct to create allocators. This recursively creates allocators and hence makes
 * a memory pool. This is the terminal step
 *
 * @tparam powStart starting power of 2 for memory of Allocator
 * @tparam from number to start from
 * @tparam to number to end at
 */
template<uint16_t powStart, uint16_t from>
struct insert<powStart, from, from> {
    /**
     * Insert either static or dynamic memory blocks
     */
    static void apply() {
        constexpr size_type curr_pow = from + powStart;
        constexpr size32_type blockSize = RestrictSize::apply<curr_pow, pow_const<size32_type>(2, curr_pow)>();

#if defined(DYNAMIC_POOL)
        __allocators[from] = new DynamicAllocatorPool<blockSize, NUM_BLOCKS>();
#elif defined(STATIC_POOL)
        __allocators[from] = new StaticAllocatorPool<blockSize, NUM_BLOCKS>();
#endif
    }
};

void memory_init() {
    // smallest pow of 2 a block can be created
    constexpr auto powStart = log2_const<size_type>(REQUIRED_EXTRA_BUFFER) + 1;
    insert<powStart, 0, MAX_ALLOCATORS - 1>::apply();
}

void memory_destroy() {
    for (auto &allocator : __allocators) {
        if (allocator == nullptr) {
            break;
        }
        delete allocator;
        allocator = nullptr;
    }
}

/**
 * Return an allocator instance matching the size provided
 *
 * @param size allocator blocks size
 * @return Allocator instance or nullptr if no allocator exists
 */
static inline __Allocator *find_allocator(size32_type size) {
    for (auto &allocator : __allocators) {
        if (allocator == nullptr) {
            break;
        }

#if defined(DYNAMIC_POOL) || defined(STATIC_POOL)
        if (allocator->getBlockSize() >= size) {
            return allocator;
        }
#else
        if (allocator->getBlockSize() == size)
            return allocator;
#endif
    }

    return nullptr;
}

/**
 * Insert an Allocator instance into the array
 *
 * @param new_allocator an Allocator instance
 * @return true or false based on if insertion is successful
 */
static inline bool insert_allocator(__Allocator *new_allocator) {
    for (auto &allocator : __allocators) {
        if (allocator == nullptr) {
            allocator = new_allocator;
            return true;
        }
    }

    return false;
}

/**
 * Stored a pointer to the Allocator instance within the block region
 *
 * @param block a pointer to the raw memory block
 * @param allocator allocator to set
 * @return a pointer to the client's area within the block
 */
static inline void *set_block_allocator(void *block, __Allocator *allocator) {
    // Cast the raw block memory to a Allocator pointer
    auto **pAllocatorInBlock = static_cast<__Allocator **>(block);

    // Write the size into the memory block
    *pAllocatorInBlock = allocator;

    // Advance the pointer past the Allocator* block size and return a pointer to
    // the client's memory region
    return ++pAllocatorInBlock;
}

/**
 * Returns the next higher powers of two. For instance, pass in 12 and
 * the value returned would be 16
 *
 * @tparam T is any class
 * @param k value for which the higher power of two will be returned
 * @return the next higher power of two based on the input k
 */
template<typename T>
T nextHigher(T k) {
    k--;
    for (size_t i = 1; i < sizeof(T) * BYTE_SIZE; i <<= 1) {
        k |= (k >> i);
    }
    return k + 1;
}

/**
 * Get an Allocator instance based upon the client's requested block size.
 * If there is no such Allocator available, create a new one
 *
 * @param size client's requested block size
 * @param anObject specify if the memory will be used object(s) or fundamental types
 * @return an allocator instance that handles the block size
 */
__Allocator *memory_get_allocator(size32_type size, bool anObject) {
    // Based on the size, find the next higher powers of two value.
    // Add REQUIRED_EXTRA_BUFFER to the requested block size to hold the size
    // of an Allocator* within the block memory region. Most blocks are powers of two,
    // however some common allocator block sizes can be explicitly defined
    // to minimize wasted storage. This offers application specific tuning.
    // These restrictions can only imposed if no pool is being used

    size32_type blockSize = size + REQUIRED_EXTRA_BUFFER;

    if (anObject)
        blockSize += OBJECT_INFO_BUFFER;


#if !defined(DYNAMIC_POOL) && !defined(STATIC_POOL)
    // set custom blocks if pool is not used

    if (blockSize > 256 && blockSize <= 396)
        blockSize = 396;
    else if (blockSize > 512 && blockSize <= 768)
        blockSize = 768;
    else
        blockSize = nextHigher<size_t>(blockSize);
#endif

    __Allocator *allocator = find_allocator(blockSize);

#if !defined(DYNAMIC_POOL) && !defined(STATIC_POOL)
    if (allocator == nullptr){
        // Create a new allocator to handle blocks of the size required
        allocator = new __Allocator(blockSize);

        // Insert allocator into array
        if (!insert_allocator(allocator))
            return nullptr;
    }
#endif

    return allocator;
}

/**
 * Allocates a memory block of the requested size. The blocks are created from
 * the fixed block allocators
 *
 * @param size the client's requested size of the block
 * @param anObject specify if the memory will be used object(s) or fundamental types
 * @return a pointer to the memory block
 */
void *__memory_alloc(size32_type size, bool anObject) {
    // Allocate a raw memory block
    __Allocator *allocator = memory_get_allocator(size, anObject);

    // if not enough sizes available
    if (allocator == nullptr) {
        return nullptr;
    }

    void *blockMemoryPtr = allocator->allocate();

    // if not enough quantity of that particular size available
    if (blockMemoryPtr == nullptr) {
        // check if higher sizes have some blocks available and
        // give a block from them if available
        size32_type currBlockSize = allocator->getBlockSize();
        if (currBlockSize < __allocators[MAX_ALLOCATORS - 1]->getBlockSize()) {
            return __memory_alloc(currBlockSize + 1, anObject);
        }

        return nullptr;
    }

    // Set the block Allocator* within the raw memory block region
    void *clientsMemoryPtr = set_block_allocator(blockMemoryPtr, allocator);
    return clientsMemoryPtr;
}

/**
 * Gets the size of the memory block stored within the block
 *
 * @param block a pointer to the client's memory block
 * @return The original allocator instance stored in the memory block
 */
static inline __Allocator *get_block_allocator(void *block) {
    // Cast the client memory to a Allocator pointer
    auto **pAllocatorInBlock = static_cast<__Allocator **>(block);

    // Back up one Allocator* position to get the stored allocator instance
    pAllocatorInBlock--;

    // Return the allocator instance stored within the memory block
    return *pAllocatorInBlock;
}

/**
 * The raw memory block pointer given a client's memory pointer
 *
 * @param block a pointer to the client's memory block
 * @return A pointer to the original raw memory block
 */
static inline void *get_block_ptr(void *block) {
    // Cast the client memory to a Allocator* pointer
    auto **pAllocatorInBlock = static_cast<__Allocator **>(block);

    // Back up one Allocator* position and return the original raw memory block pointer
    return --pAllocatorInBlock;
}

/**
 * Frees a memory block previously allocated with @code __memory_alloc @endcode. The blocks are
 * returned to the fixed block allocator that originally created it
 *
 * @param ptr a pointer to a block created with __memory_alloc
 */
void __memory_free(void *ptr) {
    if (ptr == nullptr) {
        return;
    }

    // Extract the original allocator instance from the caller's block pointer
    __Allocator *allocator = get_block_allocator(ptr);

    // Convert the client pointer into the original raw block pointer
    void *blockPtr = get_block_ptr(ptr);

    // Deallocate the block
    allocator->deallocate(blockPtr);
}

size32_type getTotalMemoryUsed() {
    size32_type totalMemory = 0;

    for (auto &allocator : __allocators) {
        totalMemory += allocator->getNumAllocations() * allocator->getBlockSize();
    }

    return totalMemory;
}

wlp::size32_type getTotalMemoryFree(){
    return getTotalMemoryAvailable() - getTotalMemoryUsed();
}


size32_type getTotalMemoryAvailable() {
    size32_type totalMemory = 0;

    for (auto &allocator : __allocators) {
        totalMemory += allocator->getTotalBlocks() * allocator->getBlockSize();
    }

    return totalMemory;
}

bool isSizeAvailable(size32_type blockSize) {
    for (auto &allocator : __allocators) {
        if (allocator->getBlockSize() == blockSize) {
            return true;
        }
    }

    return false;
}

bool isSizeMemAvailable(size32_type blockSize) {
    for (auto &allocator : __allocators) {
        if (allocator->getBlockSize() == blockSize) {
            uint32_t totalMemAvail = allocator->getTotalBlocks() * allocator->getBlockSize();
            uint32_t totalMemUsed = allocator->getNumAllocations() * allocator->getBlockSize();
            return totalMemAvail - totalMemUsed != 0;
        }
    }
    return false;
}

uint16_t getNumBlocksAvailable(size32_type blockSize) {
    uint16_t numBlockAvail = 0;

    for (auto &allocator : __allocators) {
        if (allocator->getBlockSize() == blockSize) {
            uint32_t totalMemAvail = allocator->getTotalBlocks() * allocator->getBlockSize();
            uint32_t totalMemUsed = allocator->getNumAllocations() * allocator->getBlockSize();
            numBlockAvail += (totalMemAvail - totalMemUsed) / blockSize;
        }
    }

    return numBlockAvail;
}

uint16_t getNumBlocks() {
    return NUM_BLOCKS;
}

uint16_t getMaxAllocations() {
    return MAX_ALLOCATORS;
}

size_type getSmallestBlockSize() {
    return static_cast<size_type>(pow_const(
            static_cast<size_type>(2),
            static_cast<size_type>(log2_const<size_type>(REQUIRED_EXTRA_BUFFER) + 1))
    );
}

size32_type getFixedMemorySize(void *ptr) {
    if (ptr == nullptr)
        return 0;

    __Allocator *allocator = get_block_allocator(ptr);
    return allocator->getBlockSize();
}
