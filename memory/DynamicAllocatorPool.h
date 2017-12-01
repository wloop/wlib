/**
 * @file DynamicAllocatorPool.h
 * @brief Template class to create Dynamic memory pools
 * 
 * This class is a generalization of the @code Allocator @endcode class and can be used for
 * convenience
 *
 * @author Deep Dhillon
 * @date November 11, 2017
 * @bug No known bugs
 */

#ifndef FIXED_MEMORY_DYNAMIC_ALLOCATORPOOL_H
#define FIXED_MEMORY_DYNAMIC_ALLOCATORPOOL_H

#include "Allocator.h"

namespace wlp {
    template<size32_type tblockSize, size32_type tnumBlocks>
    class DynamicAllocatorPool : public __Allocator {
    public:
        DynamicAllocatorPool() : __Allocator(tblockSize, (tblockSize * tnumBlocks)) {}
    };
}

#endif //FIXED_MEMORY_DYNAMIC_ALLOCATORPOOL_H
