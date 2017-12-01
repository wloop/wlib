/**
 * @file StaticAllocatorPool.h
 * @brief Template class to create static memory pools
 *
 * This class is a generalization of the @code Allocator @endcode class and can be used for
 * convenience
 *
 * @author Deep Dhillon
 * @date November 11, 2017
 * @bug No known bugs
 */

#ifndef FIXED_MEMORY_STATIC_ALLOCATORPOOL_H
#define FIXED_MEMORY_STATIC_ALLOCATORPOOL_H

#include "Allocator.h"

namespace wlp {
    template<size32_type tblockSize, size32_type tnumBlocks>
    class StaticAllocatorPool : public __Allocator {
    public:
        StaticAllocatorPool() : __Allocator(tblockSize, m_memory, tblockSize * tnumBlocks, __Allocator::STATIC) {}

    private:
        char m_memory[tblockSize * tnumBlocks];
    };
}


#endif //FIXED_MEMORY_STATIC_ALLOCATORPOOL_H
