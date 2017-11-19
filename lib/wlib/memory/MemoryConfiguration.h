/**
 * @file Memory.cpp
 * @brief Configuration properties for how memory is created and how much is created
 *
 * @author Deep Dhillon
 * @date November 19, 2017
 * @bug No known bugs
 */

#ifndef EMBEDDEDCPLUSPLUS_MEMORYCONFIGURATION_H
#define EMBEDDEDCPLUSPLUS_MEMORYCONFIGURATION_H

// Number of allocators (how many sizes of memory blocks)
#define MAX_ALLOCATORS 6u
// Number of block each allocator will contain
#define NUM_BLOCKS 10u

/*
 * Type of memory available. Available types:
 * STATIC_POOL  -> Static memory will be used and all the memory will be reserved at start
 * DYNAMIC_POOL -> Dynamic memory will be used and all the memory will be reserved at start
 * NO_POOL      -> Dynamic memory will be used and Allocators will be expanded at run time
 *
 * If you don't define any of these then no memory will created at the beginning and
 * everything will be expanded at run time
 */
#define DYNAMIC_POOL

#endif //EMBEDDEDCPLUSPLUS_MEMORYCONFIGURATION_H
