#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include <queue>

#include "Order.h"
#include "BinaryTreeNode.h"


template <class T, class Node = Node<T>>
class Tree {
    Node *root = nullptr;

public:
    Tree() = default;

    Tree(const Tree &) = default;

    Tree(Tree &&) = default;

    ~Tree() = default;

    void add(const T &value) {
        if (nullptr == root) {
            root = new Node{value};
            return;
        }
        root->add(value);
    }

    T getMax() {
        return root->getMax()->value;
    }

    T getMin() {
        return root->getMin()->value;
    }

    T* find(const T& element) {
        if (nullptr == root) {
            return nullptr;
        }
        return &(root->find(element)->value);
    }

    bool remove(const T& el) {
        return root->remove(el);
    }

    template <class F>
    void bfs(F visit) {
        if (nullptr == root) {
            return;
        }
        root->bfs(visit);
    }

    template <class F>
    void dfs(F visit, Order order = Order::PRE) {
        if (nullptr == root) {
            return;
        }
        root->dfs(visit, order);
    }

    bool isEmpty() {
        return nullptr == root;
    }

    Node* getRoot() {
        return root;
    }
};

template <class T>
using BinaryTree = Tree <T, Node<T>>;

#endif //TREE_H
