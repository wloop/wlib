/**
 * @file Allocator.h
 * @brief Allocator is a memory management library where allocations are always of fixed size
 *
 * It also allows for memory pool where memory can be reserved in the beginning of the code
 * or provided by the user. The goal is to eliminate memory fragmentation and make allocations
 * faster and much safer
 *
 * @author Deep Dhillon
 * @date October 22, 2017
 * @bug No known bugs
 */

#ifndef C_TEST_ALLOCATOR_H
#define C_TEST_ALLOCATOR_H


#include <stddef.h>
#include <stdint.h>

namespace wlp {
    class Allocator {
    private:
        /*!
         * Block of memory that will be provided to the user
         */
        struct Block {
            Block *pNext;   /*!< linked list to keep track memory pool */
        };

    public:
        /**
         * This is the type of memory being used in memory pool
         */
        enum Type {
            STATIC, /**< this is Stack memory */
            DYNAMIC /**< this is Heap memory */
        };

        /**
         * Allocator used for allocating memory where memory is acquired by the allocator. It supports a pool
         * where pool is created a beginning and then used throughout. If more memory is needed than there is
         * in pool, memory is dynamically acquired and then get used just like pool after that
         *
         * @pre If poolSize is smaller than blockSize, blockSize is the minimum poolSize used. If blockSize does
         *      not evenly divide poolSize then poolSize is adjusted to make it satisfy at least one block
         *
         * @pre Minimum size of block is 4 bytes on 32 bit machine and 8 bytes on 64 bit machine. On Arduino the
         *      minimum size of block is 2 bytes
         *
         * @param blockSize size of memory blocks that can acquired at a time
         * @param poolSize size of memory pool to be created
         */
        explicit Allocator(uint16_t blockSize, uint16_t poolSize = 0);

        /**
         * Allocator used for allocating memory where memory is provided to the allocator. It uses the given
         * memory as a pool and memory blocks given from it. It supports memory for Stack as well as Heap
         *
         * @pre Memory pool provided must be at least as big as blockSize unless unexpected results can be expected.
         *      Some part of pool maybe left unused to make the pool divide evenly into blocks of blockSize
         *
         * @pre Minimum size of block is 4 bytes on 32 bit machine and 8 bytes on 64 bit machine. On Arduino the
         *      minimum size of block is 2 bytes
         *
         * @param blockSize size of memory blocks that can be acquired at a time
         * @param pPool memory pool provided
         * @param poolSize size of the memory pool provided
         * @param type type of memory pool provided (static or dynamic)
         */
        Allocator(uint16_t blockSize, void *pPool, uint16_t poolSize, Type type);

        /**
         * Deletes memory and returns it back to the system
         *
         * @pre Allocator promises to safely delete all the memory defined in a pool regardless of user
         *      de-allocated the memory or not but for dynamic heap blocks gathered in runtime there is
         *      no such promise. User has to call de-allocate on that memory
         */
        ~Allocator();

        /**
         * Allocates memory from internal memory pool/dynamic memory system and gives access to the user
         *
         * @return address to memory of blockSize that is predefined
         */
        void *Allocate();

        /**
         * De-allocates the memory so that it is available if another call for memory is made. It does not
         * return the memory back to the system but rather holds on to it
         *
         * @pre Only the memory that is borrowed from Allocator should be de-allocated. If other memory
         *      addresses are provided, results are undefined
         *
         * @param pBlock address to memory block that needs de-allocation
         */
        void Deallocate(void *pBlock);

        /**
         * Gives user indication if the memory block they have belongs to the pool or it is some other dynamic
         * memory
         *
         * @param pBlockVoid memory block address being verified
         * @return true or false based on if the given block belongs to memory pool
         */
        inline bool IsPoolBlock(void *pBlockVoid) {
            auto *pBlock = (Block *) pBlockVoid;

            if (!m_pPool)return false;
            return ((char *) pBlock >= (char *) m_pPool &&
                    (char *) pBlock <= (char *) m_pPool + m_blockSize * (m_totalBlockCount - 1));
        }

        /**
         * Gives access to the size of block that Allocator is using
         *
         * @return size of memory block
         */
        inline size_t GetBlockSize() {
            return m_blockSize;
        }

        /**
         * Gives access to the size of pool that Allocator is using
         *
         * @return size of pool
         */
        inline size_t GetPoolSize() {
            return m_poolSize;
        }

        /**
         * Gives access to the number of blocks available in pool
         *
         * @return number of memory blocks available in the pool
         */
        inline uint16_t GetNumPoolBlocksAvail() {
            return m_poolCurrBlockCnt;
        }

        /**
         * Gives access to total number of blocks in pool
         *
         * @return number of memory blocks in total in the pool
         */
        inline uint16_t GetTotalPoolBlocks() {
            return m_poolTotalBlockCnt;
        }

        /**
         * Gives access to total number of blocks there are
         *
         * @return number of memory blocks in total in Allocator
         */
        inline uint16_t GetTotalBlocks() {
            return m_totalBlockCount;
        }

        /**
         * Gives access to total number of allocations occurred so for
         *
         * @return the number of allocations
         */
        inline uint16_t GetNumAllocations() {
            return m_allocations;
        }

        /**
         * Gives access to total number of de-allocations occured so far
         *
         * @return the number of de-allocations
         */
        inline uint16_t GetNumDeallocations() {
            return m_deallocations;
        }

    private:
        /**
         * Private constructor creates Allocator based on the calls made by other constructor. For more
         * information about construction of Allocator, look at docs for other constructors
         *
         * @param blockSize size of block of memory that will be give when allocated
         * @param poolSize size of memory pool
         * @param allocationType type of memory in memory pool
         * @param pPool address to memory provided
         */
        explicit Allocator(uint16_t blockSize, uint16_t poolSize, Allocator::Type allocationType, void *pPool);


        const Type m_poolType;
        size_t m_blockSize;
        Block *m_pHead;
        Block *m_pPool;
        size_t m_poolSize;
        uint16_t m_poolTotalBlockCnt;
        uint16_t m_poolCurrBlockCnt;
        uint16_t m_totalBlockCount;
        uint16_t m_allocations;
        uint16_t m_deallocations;
    };
}


#endif //C_TEST_ALLOCATOR_H
