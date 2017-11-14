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

#ifndef FIXED_MEMORY_ALLOCATORPOOL_H
#define FIXED_MEMORY_ALLOCATORPOOL_H

#include "Allocator.h"

template<uint16_t tblockSize, uint16_t tnumBlocks>
class StaticAllocatorPool : public wlp::Allocator {
public:
    StaticAllocatorPool() : wlp::Allocator(tblockSize, m_memory, tblockSize * tnumBlocks, Allocator::STATIC){}
private:
    char m_memory[tblockSize * tnumBlocks];
};

#endif //FIXED_MEMORY_ALLOCATORPOOL_H
