#ifndef TEMPLATE_DEFS_H
#define TEMPLATE_DEFS_H

#include "Types.h"

#include "stl/ChainMap.h"
#include "stl/OpenMap.h"
#include "stl/ArrayHeap.h"

namespace wlp {
    template
    struct Pair<uint16_t, const char *>;

    template
    class ChainHashMap<StaticString<16>, StaticString<16>>;

    template
    class ChainHashMap<uint16_t, uint16_t>;

    template
    struct Pair<ChainHashMap<uint16_t, uint16_t>::iterator, bool>;

    template
    struct Pair<ChainHashMap<StaticString<16>, StaticString<16>>::iterator, bool>;

    template
    struct ChainHashMapIterator<
            StaticString<16>,
            StaticString<16>,
            Hash<StaticString<16>, uint16_t>,
            Equal<StaticString<16>>>;

    template
    struct ChainHashMapIterator<
            uint16_t,
            uint16_t,
            Hash<uint16_t, uint16_t>,
            Equal<uint16_t>>;

    template
    struct ChainHashMapConstIterator<
            StaticString<16>,
            StaticString<16>,
            Hash<StaticString<16>, uint16_t>,
            Equal<StaticString<16>>>;

    template
    struct ChainHashMapConstIterator<
            uint16_t,
            uint16_t,
            Hash<uint16_t, uint16_t>,
            Equal<uint16_t>>;

    template
    struct Equal<StaticString<8>>;

    template
    struct Equal<uint16_t>;

    template
    struct Hash<StaticString<8>, uint16_t>;

    template
    struct Hash<char *, uint16_t>;

    template
    struct Hash<uint16_t, uint16_t>;

    template
    class OpenHashMap<StaticString<16>, StaticString<16>>;

    template
    class OpenHashMap<uint16_t, uint16_t>;

    template
    struct Pair<OpenHashMap<uint16_t, uint16_t>::iterator, bool>;

    template
    struct Pair<OpenHashMap<StaticString<16>, StaticString<16>>::iterator, bool>;

    template
    struct OpenHashMapIterator<
            StaticString<16>,
            StaticString<16>,
            Hash<StaticString<16>, uint16_t>,
            Equal<StaticString<16>>>;

    template
    struct OpenHashMapIterator<
            uint16_t,
            uint16_t,
            Hash<uint16_t, uint16_t>,
            Equal<uint16_t>>;

    template
    struct OpenHashMapConstIterator<
            StaticString<16>,
            StaticString<16>,
            Hash<StaticString<16>, uint16_t>,
            Equal<StaticString<16>>>;

    template
    struct OpenHashMapConstIterator<
            uint16_t,
            uint16_t,
            Hash<uint16_t, uint16_t>,
            Equal<uint16_t>>;

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
}

#endif // TEMPLATE_DEFS_H
