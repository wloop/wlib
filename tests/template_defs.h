#ifndef TEMPLATE_DEFS_H
#define TEMPLATE_DEFS_H

#include <wlib/Types.h>

#include <wlib/strings/String.h>
#include <wlib/utility/Utility.h>
#include <wlib/stl/HashMap.h>
#include <wlib/stl/OpenMap.h>
#include <wlib/stl/ArrayHeap.h>
#include <wlib/stl/LinkedList.h>
#include <wlib/stl/UniquePtr.h>
#include <wlib/stl/SharedPtr.h>

namespace wlp {
    template
    struct Pair<uint16_t, const char *>;

    template
    class HashMap<String16, String16>;

    template
    class HashMap<uint16_t, uint16_t>;

    template
    struct Pair<HashMap<uint16_t, uint16_t>::iterator, bool>;

    template
    struct Pair<HashMap<String16, String16>::iterator, bool>;

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
    class array_heap<int>;

    template
    class array_heap<const char *>;

    template
    class array_list<int>;

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
