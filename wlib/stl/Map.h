/**
 * @file Map.h
 * @brief Hash map implementation.
 *
 * @author Jeff Niu
 * @date November 1, 2017
 * @bug No known bugs
 */

#ifndef CORE_STL_MAP_H
#define CORE_STL_MAP_H

#include "Hash.h"
#include "Equal.h"

template <class Key,
          class Val,
          class Hash = hash<Key, uint16_t>,
          class Equal = equals<Key>,
          class Alloc>
class hash_map;

#endif //CORE_STL_MAP_H
