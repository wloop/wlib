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

#ifndef FIXED_MEMORY_ALLOCATORPOOL_H
#define FIXED_MEMORY_ALLOCATORPOOL_H

#include "Allocator.h"

namespace wlp{
    template<uint16_t tblockSize, uint16_t tnumBlocks>
    class DynamicAllocatorPool : public Allocator {
    public:
        DynamicAllocatorPool() : Allocator(tblockSize, tblockSize * tnumBlocks) {}
    };
}

#endif //FIXED_MEMORY_ALLOCATORPOOL_H
