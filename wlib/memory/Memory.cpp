/**
 * @file Memory.cpp
 * @brief Implementation of Memory functions
 *
 * @author Deep Dhillon
 * @date October 22, 2017
 * @bug No known bugs
 */

#include <string.h>
#include "Memory.h"
#include "Allocator.h"

#ifndef CHAR_BIT
#define CHAR_BIT	8
#endif

#define MAX_ALLOCATORS 10

using namespace wlp;

static Allocator* _allocators[MAX_ALLOCATORS];

int MemoryInitDestroy::m_srefCount = 0;

MemoryInitDestroy::MemoryInitDestroy() {
    if (m_srefCount++ == 0) memory_init();
}

MemoryInitDestroy::~MemoryInitDestroy() {
    if (--m_srefCount == 0) memory_destroy();
}

/**
 * Returns the next higher powers of two. For instance, pass in 12 and
 * the value returned would be 16
 *
 * @tparam T is any class
 * @param k value for which the higher power of two will be returned
 * @return the next higher power of two based on the input k
 */
template <class T>
T nextHigher(T k)
{
    k--;
    for (size_t i=1; i <sizeof(T)*CHAR_BIT; i<<=1)
        k |= (k >> i);
    return k+1;
}

void memory_init(){}

extern "C" void memory_destroy() {
    for (auto &_allocator : _allocators) {
        if (_allocator == nullptr)
            break;
        delete _allocator;
        _allocator = nullptr;
    }

}

/**
 * Return an allocator instance matching the size provided
 * @param size allocator blocks size
 * @return Allocator instance or nullptr if no allocator exists
 */
static inline Allocator* find_allocator(size_t size)
{
    for (auto &_allocator : _allocators) {
        if (_allocator == nullptr)
            break;

        if (_allocator->GetBlockSize() == size)
            return _allocator;
    }

    return nullptr;
}

/**
 * Inster an Allocator instance into the array
 * @param allocator an Allocator instance
 */
static inline void insert_allocator(Allocator* allocator)
{
    for (auto &_allocator : _allocators) {
        if (_allocator == nullptr)
        {
            _allocator = allocator;
            return;
        }
    }
}

/**
 * Stored a pointer to the Allocator instance within the block region
 * @param block a pointer to the raw memory block
 * @param allocator allocator to set
 * @return a pointer to the client's area within the block
 */
static inline void *set_block_allocator(void* block, Allocator* allocator)
{
    // Cast the raw block memory to a Allocator pointer
    auto ** pAllocatorInBlock = static_cast<Allocator**>(block);

    // Write the size into the memory block
    *pAllocatorInBlock = allocator;

    // Advance the pointer past the Allocator* block size and return a pointer to
    // the client's memory region
    return ++pAllocatorInBlock;
}

/**
 * Get an Allocator instance based upon the client's requested block size.
 * If there is no such Allocator available, create a new one
 * @param size client's requested block size
 * @return an allocator instance that handles the block size
 */
extern "C" Allocator* memory_get_allocator(size_t size)
{
    // Based on the size, find the next higher powers of two value.
    // Add sizeof(Allocator*) to the requested block size to hold the size
    // within the block memory region. Most blocks are powers of two,
    // however some common allocator block sizes can be explicitly defined
    // to minimize wasted storage. This offers application specific tuning.
    size_t blockSize = size + sizeof(Allocator*);
    if (blockSize > 256 && blockSize <= 396)
        blockSize = 396;
    else if (blockSize > 512 && blockSize <= 768)
        blockSize = 768;
    else
        blockSize = nextHigher<size_t>(blockSize);

    Allocator* allocator = find_allocator(blockSize);

    // If there is not an allocator already created to handle this block size
    if (allocator == nullptr)
    {
        // Create a new allocator to handle blocks of the size required
        allocator = new Allocator(blockSize);

        // Insert allocator into array
        insert_allocator(allocator);
    }

    return allocator;
}

/**
 * Allocates a memory block of the requested size. The blocks are created from
 * the fixed block allocators
 * @param size the client's requested size of the block
 * @return a pointer to the memory block
 */
extern "C" void *memory_alloc(size_t size) {
    // Allocate a raw memory block
    Allocator* allocator = memory_get_allocator(size);
    void* blockMemoryPtr = allocator->Allocate();

    // Set the block Allocator* within the raw memory block region
    void* clientsMemoryPtr = set_block_allocator(blockMemoryPtr, allocator);
    return clientsMemoryPtr;
}

/**
 * Gets the size of the memory block stored within the block
 * @param block a pointer to the client's memory block
 * @return The original allocator instance stored in the memory block
 */
static inline Allocator* get_block_allocator(void* block)
{
    // Cast the client memory to a Allocator pointer
    auto ** pAllocatorInBlock = static_cast<Allocator**>(block);

    // Back up one Allocator* position to get the stored allocator instance
    pAllocatorInBlock--;

    // Return the allocator instance stored within the memory block
    return *pAllocatorInBlock;
}

/**
 * The raw memory block pointer given a client's memory pointer
 * @param block a pointer to the client's memory block
 * @return A pointer to the original raw memory block
 */
static inline void *get_block_ptr(void* block)
{
    // Cast the client memory to a Allocator* pointer
    auto ** pAllocatorInBlock = static_cast<Allocator**>(block);

    // Back up one Allocator* position and return the original raw memory block pointer
    return --pAllocatorInBlock;
}

/**
 * Frees a memory block previously allocated with memory_alloc. The blocks are
 * returned to the fixed block allocator that originally created it
 * @param ptr a pointer to a block created with memory_alloc
 */
extern "C" void memory_free(void* ptr)
{
    if (ptr == nullptr)
        return;

    // Extract the original allocator instance from the caller's block pointer
    Allocator* allocator = get_block_allocator(ptr);

    // Convert the client pointer into the original raw block pointer
    void* blockPtr = get_block_ptr(ptr);

    // Deallocate the block
    allocator->Deallocate(blockPtr);
}

/**
 * Reallocates a memory block previously allocated with memory_alloc
 * @param oldMem a pointer to a block created with memory_alloc
 * @param size the client requested block size
 * @return pointer to new memory block
 */
extern "C" void *memory_realloc(void *oldMem, size_t size)
{
    if (oldMem == nullptr)
        return memory_alloc(size);

    if (size == 0)
    {
        memory_free(oldMem);
        return nullptr;
    }
    else
    {
        // Create a new memory block
        void* newMem = memory_alloc(size);
        if (newMem != nullptr)
        {
            // Get the original allocator instance from the old memory block
            Allocator* oldAllocator = get_block_allocator(oldMem);
            size_t oldSize = oldAllocator->GetBlockSize() - sizeof(Allocator*);

            // Copy the bytes from the old memory block into the new (as much as will fit)
            memcpy(newMem, oldMem, (oldSize < size) ? oldSize : size);

            // Free the old memory block
            memory_free(oldMem);

            // Return the client pointer to the new memory block
            return newMem;
        }
        return nullptr;
    }
}
