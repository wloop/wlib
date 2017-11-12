/**
 * @file Allocator.cpp
 * @brief Implementation of Allocator
 *
 * @author Deep Dhillon
 * @date October 22, 2017
 * @bug No known bugs
 */

#include <string.h>
#include <math.h>

#include "Allocator.h"

#include "../Types.h"

#include "../stl/Tmp.h"


wlp::Allocator::Allocator(uint16_t blockSize, uint16_t poolSize, wlp::Allocator::Type allocationType, void *pPool) :
        m_poolType{allocationType},
        m_blockSize{blockSize},
        m_pHead{nullptr},
        m_pPool{nullptr},
        m_poolSize{poolSize},
        m_poolTotalBlockCnt{0},
        m_poolCurrBlockCnt{0},
        m_totalBlockCount{0},
        m_allocations{0},
        m_deallocations{0} {
    // lowest size of a block will be the size of Block ptr
    if (m_blockSize < sizeof(wlp::Allocator::Block *)) m_blockSize = sizeof(wlp::Allocator::Block *);

    // if pool size is provided, we will use pool instead of dynamic heap allocations
    if (m_poolSize) {
        m_poolSize = max(m_blockSize, poolSize);

        // find the closest round number that describes the number of blocks
        m_poolTotalBlockCnt = (uint16_t) roundf(m_poolSize / (float) m_blockSize);
        m_poolCurrBlockCnt = m_poolTotalBlockCnt;
        m_totalBlockCount = m_poolTotalBlockCnt;

        // based on number of blocks adjust the size of pool
        m_poolSize = m_blockSize * m_totalBlockCount;

        // If caller provided an external memory pool
        if (pPool) {
            m_pPool = (wlp::Allocator::Block *) pPool;
        } else {
            // create a pool
            m_pPool = (wlp::Allocator::Block *) new char[m_poolSize];
        }

        memset(m_pPool, 0, m_poolSize);

        // Fill m_pPool with m_poolSize blocks
        wlp::Allocator::Block *pBlock = m_pPool;
        for (size_type i = 1; i < m_poolTotalBlockCnt; i++) {
            pBlock = pBlock->pNext = (wlp::Allocator::Block *) ((char *) pBlock + m_blockSize);
        }

        // Initially, all in Deallocate'd state
        m_pHead = m_pPool;
    }
}

wlp::Allocator::Allocator(Allocator &&allocator)
        : m_poolType(move(allocator.m_poolType)),
          m_blockSize(move(allocator.m_blockSize)),
          m_poolSize(move(allocator.m_poolSize)),
          m_pHead(move(allocator.m_pHead)),
          m_pPool(move(allocator.m_pPool)),
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

wlp::Allocator &wlp::Allocator::operator=(Allocator &&allocator) {
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

wlp::Allocator::Allocator(uint16_t blockSize, uint16_t poolSize) :
        wlp::Allocator(blockSize, poolSize, DYNAMIC, nullptr) {}

wlp::Allocator::Allocator(uint16_t blockSize, void *pPool, uint16_t poolSize, Type type) :
        wlp::Allocator(blockSize, poolSize, type, pPool) {}

wlp::Allocator::~Allocator() {
    // delete non pool blocks
    if (m_totalBlockCount > m_poolTotalBlockCnt) {
        wlp::Allocator::Block *pBlock = nullptr;
        while (m_pHead) {
            pBlock = m_pHead;
            if (pBlock) {
                m_pHead = m_pHead->pNext;
                if (!IsPoolBlock(pBlock)) {
                    delete[] (char *) pBlock;
                    --m_totalBlockCount;
                }
            }
        }
    }

    // only delete the pool if it is not static
    if (m_poolType != Type::STATIC) {
        // destroy the pool
        if (m_pPool) delete[] (char *) m_pPool;
    }
}

void *wlp::Allocator::Allocate() {
    // Pop one free block, if any.
    wlp::Allocator::Block *pBlock = m_pHead;

    if (pBlock) {
        m_pHead = m_pHead->pNext;
        --m_poolCurrBlockCnt;
    } else {
        // Otherwise, get a 'new' one from heap.
        pBlock = (wlp::Allocator::Block *) new char[m_blockSize];
        ++m_totalBlockCount;
    }

    ++m_allocations;

    return pBlock;
}

void wlp::Allocator::Deallocate(void *pBlock) {
    if (IsPoolBlock(pBlock)) ++m_poolCurrBlockCnt;

    auto pBlock1 = (wlp::Allocator::Block *) pBlock;
    pBlock1->pNext = m_pHead;
    m_pHead = pBlock1;
    ++m_deallocations;
}

