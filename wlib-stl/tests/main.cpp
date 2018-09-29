#include <wlib/new>
#include <wlib/tree_map>

using namespace wlp;

typedef dynamic_string string;

template<typename K, typename V>
using map = tree_map<K, V>;


struct data_init {
    static constexpr unsigned POOL_SIZE = 4096;
    static char memory[POOL_SIZE];
    static int s_initCount;
    data_init() {
        if (0 == s_initCount) {
            ++s_initCount;
            mem::init(memory, POOL_SIZE);
        }
    }
    map<string, string> data;
};

char data_init::memory[POOL_SIZE];
int data_init::s_initCount = 0;

static data_init s;

void setup() {
}

void loop() {
}
