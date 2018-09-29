#include <wlib/tree_map>
#include <wlib/memory>
#include <iostream>

using namespace wlp;

static constexpr unsigned POOL_SIZE = 16400;
static char memory[POOL_SIZE];

int main(void) {
    if (!mem::init(memory, POOL_SIZE)) {
        std::cout << "Failed to init memory" << std::endl;
        return -1;
    }

    tree_map<dynamic_string, dynamic_string> map;
    dynamic_string key("hello");
    dynamic_string val("goodbye");
    auto ret = map.insert(key, move(val));
    if (!ret.second()) {
        std::cout << "Insert failed" << std::endl;
        return -1;
    }
    auto it = map.find(key);
    if (it == map.end()) {
        std::cout << "Find failed" << std::endl;
        return -1;
    }
    std::cout << it->c_str() << std::endl;
}
