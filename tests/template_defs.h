#ifndef TEMPLATE_DEFS_H
#define TEMPLATE_DEFS_H

#include <wlib/strings/String.h>
#include <wlib/stl/HashMap.h>
#include <wlib/stl/OpenMap.h>
#include <wlib/stl/ArrayHeap.h>
#include <wlib/stl/LinkedList.h>
#include <wlib/stl/UniquePtr.h>
#include <wlib/stl/SharedPtr.h>
#include <wlib/stl/Array2D.h>

namespace wlp {
    template
    class array2d<int, int>;

    template
    struct pair<uint16_t, const char *>;

    template
    class hash_map<String16, String16>;

    template
    class hash_map<uint16_t, uint16_t>;

    template
    struct pair<hash_map<uint16_t, uint16_t>::iterator, bool>;

    template
    struct pair<hash_map<String16, String16>::iterator, bool>;

    template
    struct equals<String8>;

    template
    struct equals<uint16_t>;

    template
    struct hash<wlp::static_string<8u>, uint16_t>;

    template
    struct hash<char *, uint16_t>;

    template
    struct hash<uint16_t, uint16_t>;

    template
    class open_map<String16, String16>;

    template
    class open_map<uint16_t, uint16_t>;

    template
    struct pair<open_map<uint16_t, uint16_t>::iterator, bool>;

    template
    struct pair<open_map<String16, String16>::iterator, bool>;

    template
    class array_heap<int>;

    template
    class array_heap<const char *>;

    template
    class array_list<int>;

    template
    class static_string<8>;

    template
    class static_string<16>;

    template
    class ArrayListIterator<int, int &, int *>;

    template
    class ArrayListIterator<int, const int &, const int *>;

    template
    class ArrayListIterator<const char *, const char *&, const char **>;

    template
    class ArrayListIterator<const char *, const char *const &, const char *const *>;

    template
    struct comparator<int>;

    template
    struct reverse_comparator<int>;

    template
    struct comparator<String8>;

    template
    class linked_list<int>;

    template
    struct LinkedListIterator<int, int &, int *>;

    template
    struct LinkedListIterator<int, const int &, const int *>;

    template
    class unique_ptr<int>;


    template class shared_ptr<int>;

    template class weak_ptr<int>;

}

#endif // TEMPLATE_DEFS_H
