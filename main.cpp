#include <iostream>
#include "Tree.h"
#include "TreapNode.h"

void test() {
    // TreapNode<int> tt;
    // BinaryTree<int> t;
    Tree<int, TreapNode<int>> t;
    t.add(42);
    t.add(33);
    t.add(56);
    t.add(556);
    t.add(4);
    t.add(557);
    t.add(558);
    t.add(559);
    t.add(40);

    auto print = [&] {
        t.bfs([](const auto& node) {
            std::cout << *node << std::endl;
        });
    };

    print();

    t.remove(33);
    std::cout << std::endl;

    print();
}


int main() {
    test();

    return 0;
}