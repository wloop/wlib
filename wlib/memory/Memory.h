/**
 * @file Memory.h
 * @brief Memory is a class that provides dynamic fixed size memory
 *
 * It does not impose any restrictions on how many blocks can be borrowed and of
 * what size can it be borrowed
 *
 * @author Deep Dhillon
 * @date October 22, 2017
 * @bug No known bugs
 */

#ifndef FIXED_MEMORY_MEMORY_H
#define FIXED_MEMORY_MEMORY_H

#include <stddef.h>

/**
 * @brief Helper for initializing and destroying memory management
 *
 * If there is a C++ translation unit, create a static instance of MemoryInitDestroy.
 * Any C++ file including Memory.h will have the memoryInitDestroy instance declared first
 * within the translation unit and thus will be constructed first. Destruction will occur
 * in the reverse order so memoryInitDestroy is called last. This way any static user object
 * relying on Memory will be destroyed first before destroy function is called.
 */
class MemoryInitDestroy {
public:
    MemoryInitDestroy();
    ~MemoryInitDestroy();

private:
    static int m_srefCount;
};

static MemoryInitDestroy g_smemoryInitDestroy;

extern "C"{


/**
 * This function should and must be called exactly one time before application starts.
 * On C++ client code this is done automatically using MemoryInitDestroy
 */
void memory_init();

/**
 * This function should and must be called exactly one time when the application exits. This should never
 * be called manually when using C++
 */
void memory_destroy();

/**
 * This allocates memory of the size provided. Memory allocated could be greater than what has been
 * asked in order to accommodate fixed memory allocations
 *
 * @param size size of the block to allocate
 * @return address to memory allocated
 */
void *memory_alloc(size_t size);

/**
 * This frees the memory allocated. Only memory allocated using Memory will be freed and if another
 * type of memory is provided, results are undefined
 *
 * @param ptr address to memory that will be freed
 */
void memory_free(void *ptr);

/**
 * This reallocates the memory to accommodate the new size provided. The memory address has to be
 * allocation from Memory otherwise results are undefined
 *
 * @param ptr address to memory to be reallocated
 * @param size of new memory block
 * @return address to new memory address
 */
void *memory_realloc(void *ptr, size_t size);

#define MEMORY_OVERLOAD \
    public: \
        void *operator new(size_t size){ \
            return memory_alloc(size); \
        }; \
        void operator delete(void *pObject){ \
            memory_free(pObject); \
        } \

};

#endif //FIXED_MEMORY_MEMORY_H
