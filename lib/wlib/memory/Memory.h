/**
 * @file Memory.h
 * @brief Memory is a class that provides dynamic fixed size memory
 *
 * It does not impose any restrictions on how many blocks can be borrowed and of
 * what size can it be borrowed. This has an option to use a memory pool which has
 * restrictions on how much memory we can use. When pool is filled, any call for
 * memory will return @code nullptr @endcode
 *
 * @pre Number of memory block sizes will depend on @code MAX_ALLOCATORS @endcode
 *
 * @author Deep Dhillon
 * @date November 19, 2017
 * @bug No known bugs
 */

#ifndef FIXED_MEMORY_MEMORY_H
#define FIXED_MEMORY_MEMORY_H

#include <new>

#include "../Types.h"

/**
 * @brief Helper for initializing and destroying memory management
 *
 * If there is a C++ translation unit, create a static instance of MemoryInitDestroy.
 * Any C++ file including Memory.h will have the memoryInitDestroy instance declared first
 * within the translation unit and thus will be constructed first. Destruction will occur
 * in the reverse order so memoryInitDestroy is called last. This way any static user object
 * relying on Memory will be destroyed first before destroy function is called.
 *
 * This code should never be called and/or touched. It is used internally
 */
class MemoryInitDestroy {
public:
    MemoryInitDestroy();

    ~MemoryInitDestroy();

private:
    static int m_srefCount;
};

/**
 * This code should never be called and/or touched. It is used internally
 */
static MemoryInitDestroy __g_smemoryInitDestroy;

/**
 * Used internally not for client's use
 * Use the @code malloc @endcode function
 */
void *__memory_alloc(uint32_t size);

/**
 * Used internally not for client's use
 * Use the @code realloc @endcode function
 */
void *__memory_realloc(void *ptr, uint32_t size);

/**
 * Used internally not for client's use
 * Use the @code free @endcode function
 */
void __memory_free(void *ptr);

/**
 * This allocates memory of the size provided. Memory allocated could be greater than what has been
 * asked in order to accommodate fixed memory allocations. If there is not a sufficient memory
 * available, it returns a @code nullptr @endcode
 *
 * @pre this can also act a @code new @endcode keyword to create objects so there is no need to use
 * new keyword to create an object
 *
 * @pre malloc creates number of blocks of given type and it does not allocate memory by proving it
 * number of bytes
 *
 * @tparam Type pointer type
 * @param num number of blocks of size @p Type
 * @return address to memory allocated
 */
template<typename Type>
Type *malloc(wlp::size32_type num = 1) {
    void *memory = __memory_alloc(static_cast<wlp::size32_type>(sizeof(Type)) * num);

    return new(memory) Type;
}

/**
 * This reallocates the memory to accommodate the new size provided. The memory address has to be
 * allocation from Memory otherwise results are undefined. If there is not a sufficient memory
 * available, it returns a @code nullptr @endcode
 *
 * @pre realloc creates number of blocks of given type and it does not allocate memory by proving it
 * number of bytes
 *
 * @tparam Type pointer type
 * @param ptr address to memory to be reallocated
 * @param size number of blocks of size @p Type
 * @return address to new memory address
 */
template<typename Type>
Type *realloc(Type *ptr, wlp::size32_type num = 1) {
    return static_cast<Type *>(__memory_realloc(ptr, static_cast<wlp::size32_type>(sizeof(Type)) * num));
}

/**
 * This frees the memory allocated. Only memory allocated using Memory will be freed and if another
 * type of memory is provided, results are undefined
 *
 * @tparam Type pointer type
 * @param ptr address to memory that will be freed
 */
template<class Type>
void free(Type *ptr) {
    ptr->~Type();
    __memory_free(ptr);
}

/**
 * Returns the total memory being used by the program (in bytes)
 *
 * @return the total memory usage
 */
wlp::size32_type getTotalMemoryUsed();

/**
 * Returns the total memory available to use (in bytes). If no pool is
 * being used, this number can grow as more memory is borrowed otherwise
 * this is the constant number
 *
 * @return the total memory available
 */
wlp::size32_type getTotalMemoryAvailable();

/**
 * Returns if this @p blockSize is available as a size that can be borrowed.
 * If no pool is being used, this size will be created even if it does not exist
 * but if a pool is used, this size's availability is fixed.
 *
 * @pre note if this size does not exist in a pool a higher size can be
 * provided
 *
 * @param blockSize size of the block being queried
 * @return true or false based on if the block exists
 */
bool isSizeAvailable(wlp::size32_type blockSize);

/**
 * Returns if this @p blockSize is available as a size that can be borrowed but it
 * also has some blocks left that the user can borrow. Rest it functions the same as
 * @code isSizeAvailable() @endcode
 *
 * @param blockSize size of the block being queried
 * @return true or false based on if the block exists and has some blocks left
 */
bool isSizeMemAvailable(wlp::size32_type blockSize);

/**
 * Returns the total number of blocks that are available for @p blockSize. if @p blockSize
 * does not exist then number will be 0
 *
 * @param blockSize blockSize size of the block being queried
 * @return the number blocks available for @p blockSize
 */
uint16_t getNumBlocksAvailable(wlp::size32_type blockSize);

/**
 * Returns the number of blocks per Allocator if a pool is used
 *
 * @return the number of blocks per Allocator
 */
uint16_t getNumBlocks();

/**
 * Returns the number Allocators (sizes) there are available in total
 *
 * @return the number of allocators (sizes)
 */
uint16_t getMaxAllocations();

wlp::size_type getSmallestBlockSize();

#endif //FIXED_MEMORY_MEMORY_H
