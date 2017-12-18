#ifndef TEMPLATE_DEFS_H
#define TEMPLATE_DEFS_H

#include "Types.h"

#include "strings/String.h"
#include "utility/Utility.h"
#include "stl/ChainMap.h"
#include "stl/OpenMap.h"
#include "stl/ArrayHeap.h"
#include "stl/LinkedList.h"
#include "stl/UniquePtr.h"
#include "stl/SharedPtr.h"

namespace wlp {
    template
    struct Pair<uint16_t, const char *>;

    template
    class ChainHashMap<String16, String16>;

    template
    class ChainHashMap<uint16_t, uint16_t>;

    template
    struct Pair<ChainHashMap<uint16_t, uint16_t>::iterator, bool>;

    template
    struct Pair<ChainHashMap<String16, String16>::iterator, bool>;

    template
    struct ChainHashMapIterator<
            String16,
            String16,
            String16 &,
            String16 *,
            Hash<String16, uint16_t>,
            Equal<String16>>;

    template
    struct ChainHashMapIterator<
            uint16_t,
            uint16_t,
            uint16_t &,
            uint16_t *,
            Hash<uint16_t, uint16_t>,
            Equal<uint16_t>>;

    template
    struct ChainHashMapIterator<
            String16,
            String16,
            const String16 &,
            const String16 *,
            Hash<String16, uint16_t>,
            Equal<String16>>;

    template
    struct ChainHashMapIterator<
            uint16_t,
            uint16_t,
            const uint16_t &,
            const uint16_t *,
            Hash<uint16_t, uint16_t>,
            Equal<uint16_t>>;

    template
    struct Equal<String8>;

    template
    struct Equal<uint16_t>;

    template
    struct Hash<String8, uint16_t>;

    template
    struct Hash<char *, uint16_t>;

    template
    struct Hash<uint16_t, uint16_t>;

    template
    class OpenHashMap<String16, String16>;

    template
    class OpenHashMap<uint16_t, uint16_t>;

    template
    struct Pair<OpenHashMap<uint16_t, uint16_t>::iterator, bool>;

    template
    struct Pair<OpenHashMap<String16, String16>::iterator, bool>;

    template
    class ArrayHeap<int>;

    template
    class ArrayHeap<const char *>;

    template
    class ArrayList<int>;

    template
    class StaticString<8>;

    template
    class StaticString<16>;

    template
    class ArrayListIterator<int, int &, int *>;

    template
    class ArrayListIterator<int, const int &, const int *>;

    template
    class ArrayListIterator<const char *, const char *&, const char **>;

    template
    class ArrayListIterator<const char *, const char *const &, const char *const *>;

    template
    struct Comparator<int>;

    template
    struct ReverseComparator<int>;

    template
    struct Comparator<String8>;

    template
    class LinkedList<int>;

    template
    struct LinkedListIterator<int, int &, int *>;

    template
    struct LinkedListIterator<int, const int &, const int *>;

    template
    class UniquePtr<int>;


    template class SharedPtr<int>;

    template class WeakPtr<int>;

}

#endif // TEMPLATE_DEFS_H
