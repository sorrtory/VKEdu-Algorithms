// Федуков Александр WEB-13.
// orlando20056@yandex.ru
//
// Problem: 
/*
Нужно кастомизировать std::unordered_set для операций со структурой данных

struct Node {
    std::string product_name;
    int color;
    int size;
};

Для этого нужно сделать:
- специализацию std::hash для Node
- определить оператор ==

Как сделать специализацию std::hash:

namespace std {
    template<>
        struct hash<Node> {
                std::size_t operator()(const Node& node) const noexcept {
                    // ...
                }
        };
}
*/

#include <iostream>
#include <unordered_set>
#include <string>
using namespace std;

struct Node {
    string product_name;
    int color;
    int size;

    bool operator==(const Node& other) const {
        return product_name == other.product_name &&
               color == other.color &&
               size == other.size;
    }
};

namespace std {
    template<>
    struct hash<Node> {
        size_t operator()(const Node& node) const noexcept {
            size_t seed = 0;
            hash_combine(seed, hash<string>()(node.product_name));
            hash_combine(seed, hash<int>()(node.color));
            hash_combine(seed, hash<int>()(node.size));
            return seed;
        }
    private:
        static void hash_combine(size_t& seed, size_t hash_value) {
            seed ^= hash_value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    };
}

int main() {
    unordered_set<Node> nodes;
    char op;
    string name;
    int color, size;

    while (cin >> op >> name >> color >> size) {
        Node n{name, color, size};

        if (op == '+') {
            nodes.insert(n);
            cout << "OK" << endl;
        } else if (op == '-') {
            auto it = nodes.find(n);
            if (it != nodes.end()) {
                nodes.erase(it);
                cout << "OK" << endl;
            } else {
                cout << "FAIL" << endl;
            }
        } else if (op == '?') {
            if (nodes.find(n) != nodes.end())
                cout << "OK" << endl;
            else
                cout << "FAIL" << endl;
        }
    }

    return 0;
}
