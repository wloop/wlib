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

#include <string.h>

#include "wlib/utility/Utility.h"
#include "wlib/Types.h"

#include "wlib/utility/Tmp.h"
#include "New.h"

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
void *__memory_alloc(uint32_t size, bool anObject);

/**
 * Used internally not for client's use
 * Use the @code free @endcode function
 */
void __memory_free(void *ptr);

/**
 * This allocates memory of the size of the type provided. Memory allocated could be greater
 * than what has been asked in order to accommodate fixed memory allocations. If there is not a sufficient memory
 * available, it returns a @code nullptr @endcode
 *
 * This is only used for creating single block of memory and not for array
 *
 * @pre malloc creates number of blocks of given type and it does not allocate memory by providing it
 * number of bytes
 *
 * @tparam T pointer type
 * @tparam Args Argument types for non-fundamental Types
 * @param args the arguments for @code Args @end types
 * @return address to memory allocated
 */
template<
        typename T,
        typename... Args,
        typename = typename wlp::enable_if<!wlp::is_array<T>::value, bool>::type>
T *malloc(Args... args) {
    if (wlp::is_fundamental<typename wlp::remove_extent<T>::type>::value) {
        void *memory = __memory_alloc(static_cast<wlp::size32_type>(sizeof(T)), false);
        return new(memory) T(wlp::forward<Args>(args)...);
    }

    void *memory = __memory_alloc(static_cast<wlp::size32_type>(sizeof(T)), true);

    uint16_t *objInfo = static_cast<uint16_t *>(memory);
    *objInfo = static_cast<uint16_t>(1);

    return new(reinterpret_cast<char *>(++objInfo)) T(wlp::forward<Args>(args)...);
}

/**
 * This allocates memory of the size of the type provided. Memory allocated could be greater
 * than what has been asked in order to accommodate fixed memory allocations. If there is not a sufficient memory
 * available, it returns a @code nullptr @endcode
 *
 * This is only used for creating multiple block of memory and hence should be used for arrays. This
 * only supports default Ctors for objects
 *
 * @pre malloc creates number of blocks of given type and it does not allocate memory by providing it
 * number of bytes
 *
 * @tparam T pointer array type
 * @tparam NonArrayType pointer non array type
 * @param num number of elements needed
 * @return address to memory allocated
 */
template<
        typename T,
        typename NonArrayType = typename wlp::remove_extent<T>::type,
        typename = typename wlp::enable_if<wlp::is_array<T>::value, bool>::type>
NonArrayType *malloc(wlp::size_type num) {
    if (wlp::is_fundamental<typename wlp::remove_extent<T>::type>::value) {
        void *memory = __memory_alloc(static_cast<wlp::size32_type>(sizeof(NonArrayType)) * num, false);
        return static_cast<NonArrayType *>(memory);
    }

    void *memory = __memory_alloc(static_cast<wlp::size32_type>(sizeof(NonArrayType)) * num, true);

    uint16_t *objInfo = static_cast<uint16_t *>(memory);
    *objInfo = static_cast<uint16_t>(num);

    char *pointer = reinterpret_cast<char *>(++objInfo);
    for (wlp::size_type i = 0; i < num; ++i) {
        new(pointer) NonArrayType();
        pointer += sizeof(NonArrayType);
    }

    return reinterpret_cast<NonArrayType *>(objInfo);
};

/**
 * This allocates memory of the size of the type provided. Memory allocated could be greater
 * than what has been asked in order to accommodate fixed memory allocations. If there is not a sufficient memory
 * available, it returns a @code nullptr @endcode
 *
 * This is only used for creating multiple block of memory and hence should be used for arrays. This
 * is a special case of @code malloc @endcode which supports creation of an array of objects where you can
 * use non default ctor
 *
 * @pre this creates number of blocks of given type and it does not allocate memory by providing it
 * number of bytes
 *
 * @warning should be avoided and used only where this is must
 *
 * @tparam T pointer array type
 * @tparam Args Argument types for non-fundamental Types
 * @param args the arguments for @code Args @end types
 * @tparam NonArrayType pointer non array type
 * @param num number of elements needed
 * @return address to memory allocated
 */
template<
        typename T,
        typename... Args,
        typename NonArrayType = typename wlp::remove_extent<T>::type,
        typename = typename wlp::enable_if<wlp::is_array<T>::value, bool>::type>
NonArrayType *arg_array_malloc(wlp::size_type num, Args... args) {
    void *memory = __memory_alloc(static_cast<wlp::size32_type>(sizeof(NonArrayType)) * num, true);

    uint16_t *objInfo = static_cast<uint16_t *>(memory);
    *objInfo = static_cast<uint16_t>(num);

    char *pointer = reinterpret_cast<char *>(++objInfo);
    for (wlp::size_type i = 0; i < num; ++i) {
        new(pointer) NonArrayType(wlp::forward<Args>(args)...);
        pointer += sizeof(NonArrayType);
    }

    return reinterpret_cast<NonArrayType *>(objInfo);
};

/**
 * Works exactly like malloc but initializes the memory by filling it with 0. This is the non
 * array version of @code calloc @endcode
 *
 * @tparam Type pointer type
 * @param num number of blocks of size @p Type
 * @return address to memory allocated
 */
template<
        typename T,
        typename = typename wlp::enable_if<!wlp::is_pointer<T>::value, bool>::type,
        typename = typename wlp::enable_if<wlp::is_arithmetic<T>::value, bool>::type>
T *calloc() {
    T *memory = malloc<T>();

    for (wlp::size32_type i = 0; i < sizeof(T); ++i) {
        memory[i] = 0;
    }

    return memory;
};

/**
 * Works exactly like malloc but initializes the memory by filling it with 0. This is the
 * array version of @code calloc @endcode
 *
 * @tparam Type pointer type
 * @param num number of blocks of size @p Type
 * @return address to memory allocated
 */
template<
        typename T,
        typename NonArrayType = typename wlp::remove_extent<T>::type,
        typename = typename wlp::enable_if<!wlp::is_pointer<NonArrayType>::value, bool>::type,
        typename = typename wlp::enable_if<wlp::is_arithmetic<NonArrayType>::value, bool>::type,
        typename = typename wlp::enable_if<wlp::is_array<T>::value, bool>::type>
NonArrayType *calloc(wlp::size_type num = 1) {
    NonArrayType *memory = malloc<T>(num);

    for (wlp::size32_type i = 0; i < num; ++i) {
        memory[i] = 0;
    }

    return memory;
};

/**
 * This frees the memory allocated. Only memory allocated using Memory will be freed and if another
 * type of memory is provided, results are undefined
 *
 * If an array of Objects is to be freed then provide free with a number along with the pointer to specify
 * how many objects
 *
 * @tparam Type pointer type
 * @param ptr address to memory that will be freed
 */
template<typename Type>
void free(Type *&ptr) {
    if (!wlp::is_fundamental<Type>::value) {
        uint16_t *objInfo = reinterpret_cast<uint16_t *>(ptr);
        wlp::size_type numObjects = *(--objInfo);

        Type *pointer = ptr + numObjects - 1;
        for (wlp::size_type i = 0; i < numObjects; ++i) {
            pointer->~Type();
            --pointer;
        }

        __memory_free(reinterpret_cast<Type *>(objInfo));
    } else {
        __memory_free(ptr);
    }

    ptr = nullptr;
}

/**
 * Overload for rvalue pointers.
 *
 * @see free<Type>
 * @tparam Type
 * @param ptr
 */
template<typename Type>
void free(Type *&&ptr) {
    if (!wlp::is_fundamental<Type>::value) {
        uint16_t *objInfo = reinterpret_cast<uint16_t *>(ptr);
        wlp::size_type numObjects = *(--objInfo);

        Type *pointer = ptr + numObjects - 1;
        for (wlp::size_type i = 0; i < numObjects; ++i) {
            pointer->~Type();
            --pointer;
        }

        __memory_free(reinterpret_cast<Type *>(objInfo));
    } else {
        __memory_free(ptr);
    }
}

/**
 * Returns the size of fixed sized block that was provided for @p ptr
 *
 * @param ptr memory address for which memory block is being searched
 * @return the size of fixed size block
 */
wlp::size32_type getFixedMemorySize(void *ptr);

/**
 * Returns the smallest block of memory that is given
 *
 * @return the smallest block of memory
 */
wlp::size_type getSmallestBlockSize();

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
template<
        typename Type,
        typename = typename wlp::enable_if<wlp::is_fundamental<Type>::value, bool>::type>
Type *realloc(Type *ptr, wlp::size_type num = 1) {
    if (num == 0) {
        free<Type>(ptr);
        return ptr;
    }

    wlp::size32_type oldMemSize = getFixedMemorySize(ptr);
    wlp::size32_type newMemSize = static_cast<wlp::size32_type>(sizeof(Type)) * num;

    if (oldMemSize >= newMemSize + getSmallestBlockSize()) {
        return ptr;
    }

    Type *newPtr = malloc<Type[]>(num);

    wlp::size32_type copySize = (oldMemSize < newMemSize) ? oldMemSize : newMemSize;
    memcpy(newPtr, ptr, copySize);

    free<Type>(ptr);

    return newPtr;
}

/**
 * Returns the total memory being used by the program (in bytes)
 *
 * @return the total memory usage
 */
wlp::size32_type getTotalMemoryUsed();

/**
 * Returns the total memory that is free for use(in bytes)
 *
 * @return the total memory usage
 */
wlp::size32_type getTotalMemoryFree();

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


#endif //FIXED_MEMORY_MEMORY_H
