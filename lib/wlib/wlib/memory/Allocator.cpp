/**
 * @file Allocator.cpp
 * @brief Implementation of Allocator
 *
 * @author Deep Dhillon
 * @date November 19, 2017
 * @bug No known bugs
 */

#include <string.h> // memset

#include "Allocator.h"

#include "wlib/utility/Math.h"
#include "wlib/utility/Utility.h"

using namespace wlp;

__Allocator::__Allocator(
        size32_type blockSize,
        size32_type poolSize,
        __Allocator::Type allocationType,
        void *pPool)
        : m_poolType{allocationType},
          m_pHead{nullptr},
          m_pPool{nullptr},
          m_blockSize{blockSize},
          m_poolSize{poolSize},
          m_poolTotalBlockCnt{0},
          m_poolCurrBlockCnt{0},
          m_totalBlockCount{0},
          m_allocations{0},
          m_deallocations{0} {
    // lowest size of a block will be the size of Block ptr
    if (m_blockSize < sizeof(__Allocator::Block *)) { m_blockSize = sizeof(__Allocator::Block *); }

    // if pool size is provided, we will use pool instead of dynamic heap allocations
    if (m_poolSize) {
        m_poolSize = MAX(m_blockSize, poolSize);

        // find the closest round number that describes the number of blocks
        m_poolTotalBlockCnt = int_div_round<size_type>(
                static_cast<size_type>(m_poolSize),
                static_cast<size_type>(m_blockSize)
        );
        m_poolCurrBlockCnt = m_poolTotalBlockCnt;
        m_totalBlockCount = m_poolTotalBlockCnt;

        // based on number of blocks adjust the size of pool
        m_poolSize = m_blockSize * m_totalBlockCount;

        // If caller provided an external memory pool
        if (pPool) {
            m_pPool = (__Allocator::Block *) pPool;
        } else {
            // create a pool
            m_pPool = (__Allocator::Block *) new char[m_poolSize];
        }

        memset(m_pPool, 0, m_poolSize);

        // Fill m_pPool with m_poolSize blocks
        __Allocator::Block *pBlock = m_pPool;
        for (size_type i = 1; i < m_poolTotalBlockCnt; i++) {
            pBlock = pBlock->pNext = reinterpret_cast<__Allocator::Block *>(
                    reinterpret_cast<char *>(pBlock) + m_blockSize
            );
        }

        // Initially, all in Deallocate'd state
        m_pHead = m_pPool;
    }
}

__Allocator::__Allocator(__Allocator &&allocator)
        : m_poolType(move(allocator.m_poolType)),
          m_pHead(move(allocator.m_pHead)),
          m_pPool(move(allocator.m_pPool)),
          m_blockSize(move(allocator.m_blockSize)),
          m_poolSize(move(allocator.m_poolSize)),
          m_poolTotalBlockCnt(move(allocator.m_poolTotalBlockCnt)),
          m_poolCurrBlockCnt(move(allocator.m_poolCurrBlockCnt)),
          m_totalBlockCount(move(allocator.m_totalBlockCount)),
          m_allocations(move(allocator.m_allocations)),
          m_deallocations(move(allocator.m_deallocations)) {
    allocator.m_pHead = nullptr;
    allocator.m_pPool = nullptr;
    allocator.m_allocations = 0;
    allocator.m_deallocations = 0;
    allocator.m_totalBlockCount = 0;
    allocator.m_poolCurrBlockCnt = 0;
    allocator.m_poolTotalBlockCnt = 0;
}

__Allocator &__Allocator::operator=(__Allocator &&allocator) {
    if (m_totalBlockCount > m_poolTotalBlockCnt) {
        __Allocator::Block *pBlock = nullptr;
        while (m_pHead) {
            pBlock = m_pHead;
            if (pBlock) {
                m_pHead = m_pHead->pNext;
                if (!isPoolBlock(pBlock)) {
                    delete[] (char *) pBlock;
                    --m_totalBlockCount;
                }
            }
        }
    }

    if (m_poolType != Type::STATIC && m_pPool) {
        delete[] (char *) m_pPool;
    }

    m_poolType = move(allocator.m_poolType);
    m_blockSize = move(allocator.m_blockSize);
    m_poolSize = move(allocator.m_poolSize);
    m_pHead = move(allocator.m_pHead);
    m_pPool = move(allocator.m_pPool);
    m_poolTotalBlockCnt = move(allocator.m_poolTotalBlockCnt);
    m_poolCurrBlockCnt = move(allocator.m_poolCurrBlockCnt);
    m_totalBlockCount = move(allocator.m_totalBlockCount);
    m_allocations = move(allocator.m_allocations);
    m_deallocations = move(allocator.m_deallocations);
    allocator.m_pHead = nullptr;
    allocator.m_pPool = nullptr;
    allocator.m_allocations = 0;
    allocator.m_deallocations = 0;
    allocator.m_totalBlockCount = 0;
    allocator.m_poolCurrBlockCnt = 0;
    allocator.m_poolTotalBlockCnt = 0;
    return *this;
}

__Allocator::__Allocator(size32_type blockSize, size32_type poolSize) :
        __Allocator(blockSize, poolSize, DYNAMIC, nullptr) {}

__Allocator::__Allocator(size32_type blockSize, void *pPool, uint32_t poolSize, Type type) :
        __Allocator(blockSize, poolSize, type, pPool) {}

__Allocator::~__Allocator() {
    // delete non pool blocks
    if (m_totalBlockCount > m_poolTotalBlockCnt) {
        __Allocator::Block *pBlock = nullptr;
        while (m_pHead) {
            pBlock = m_pHead;
            if (pBlock) {
                m_pHead = m_pHead->pNext;
                if (!isPoolBlock(pBlock)) {
                    delete[] (char *) pBlock;
                    --m_totalBlockCount;
                }
            }
        }
    }

    // only delete the pool if it is not static
    if (m_poolType != Type::STATIC) {
        // destroy the pool
        if (m_pPool) { delete[] (char *) m_pPool; }
    }
}

void *__Allocator::allocate() {
    // Pop one free block, if any.
    __Allocator::Block *pBlock = m_pHead;

    if (pBlock) {
        m_pHead = m_pHead->pNext;
        --m_poolCurrBlockCnt;
    } else {
        // If we are using a pool we ran out of memory
        if (m_poolSize > 0) {
            return nullptr;
        }

        // Otherwise, get a 'new' one from heap.
        pBlock = (__Allocator::Block *) new char[m_blockSize];
        ++m_totalBlockCount;
    }

    ++m_allocations;

    return pBlock;
}

void __Allocator::deallocate(void *pBlock) {
    if (isPoolBlock(pBlock)) { ++m_poolCurrBlockCnt; }

    auto pBlock1 = (__Allocator::Block *) pBlock;
    pBlock1->pNext = m_pHead;
    m_pHead = pBlock1;
    ++m_deallocations;

    m_allocations <= 0 ? m_allocations : --m_allocations;
}
