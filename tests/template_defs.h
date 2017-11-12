#ifndef TEMPLATE_DEFS_H
#define TEMPLATE_DEFS_H

#include <stdint.h>
#include "stl/ChainMap.h"
#include "stl/OpenMap.h"

namespace wlp {
	template class Pair<uint16_t, const char*>;
	template class ChainHashMap<StaticString<16>, StaticString<16>>;
	template class ChainHashMap<uint16_t, uint16_t>;
	template class Pair<ChainHashMap<uint16_t, uint16_t>::iterator, bool>;
	template class Pair<ChainHashMap<StaticString<16>, StaticString<16>>::iterator, bool>;
	template class ChainHashMapIterator<
			StaticString<16>,
			StaticString<16>,
			hash<StaticString<16>, uint16_t>,
			equals<StaticString<16>>>;
	template class ChainHashMapIterator<
			uint16_t,
			uint16_t,
			hash<uint16_t, uint16_t>,
			equals<uint16_t>>;
	template class ChainHashMapConstIterator<
			StaticString<16>,
			StaticString<16>,
			hash<StaticString<16>, uint16_t>,
			equals<StaticString<16>>>;
	template class ChainHashMapConstIterator<
			uint16_t,
			uint16_t,
			hash<uint16_t, uint16_t>,
			equals<uint16_t>>;
	template class equals<StaticString<8>>;
	template class equals<uint16_t>;
	template class hash<StaticString<8>, uint16_t>;
	template class hash<char*, uint16_t>;
	template class hash<uint16_t, uint16_t>;
	template class OpenHashMap<StaticString<16>, StaticString<16>>;
	template class OpenHashMap<uint16_t, uint16_t>;
	template class Pair<OpenHashMap<uint16_t, uint16_t>::iterator, bool>;
	template class Pair<OpenHashMap<StaticString<16>, StaticString<16>>::iterator, bool>;
	template class OpenHashMapIterator<
			StaticString<16>,
			StaticString<16>,
			hash<StaticString<16>, uint16_t>,
			equals<StaticString<16>>>;
	template class OpenHashMapIterator<
			uint16_t,
			uint16_t,
			hash<uint16_t, uint16_t>,
			equals<uint16_t>>;
	template class OpenHashMapConstIterator<
			StaticString<16>,
			StaticString<16>,
			hash<StaticString<16>, uint16_t>,
			equals<StaticString<16>>>;
	template class OpenHashMapConstIterator<
			uint16_t,
			uint16_t,
			hash<uint16_t, uint16_t>,
			equals<uint16_t>>;
    template class StaticString<8>;
    template class StaticString<16>;
}

#endif // TEMPLATE_DEFS_H
