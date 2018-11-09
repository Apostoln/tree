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
    Node *mRoot = nullptr;

public:
    Tree() = default;

    Tree(const Tree &) = default;

    Tree(Tree &&) = default;

    ~Tree() = default;

    void add(const T &value) {
        if (nullptr == mRoot) {
            mRoot = new Node{value};
            return;
        }
        mRoot->add(value);
    }

    T getMax() {
        return mRoot->getMax()->value;
    }

    T getMin() {
        return mRoot->getMin()->value;
    }

    T* find(const T& element) {
        if (nullptr == mRoot) {
            return nullptr;
        }
        return &(mRoot->find(element)->value);
    }

    bool remove(const T& el) {
        return mRoot->remove(el);
    }

    template <class F>
    void bfs(F visit) {
        if (nullptr == mRoot) {
            return;
        }
        mRoot->bfs(visit);
    }

    template <class F>
    void dfs(F visit, Order order = Order::PRE) {
        if (nullptr == mRoot) {
            return;
        }
        mRoot->dfs(visit, order);
    }

    bool isEmpty() {
        return nullptr == mRoot;
    }

    Node* getRoot() {
        return mRoot;
    }
};

template <class T>
using BinaryTree = Tree <T, Node<T>>;

#endif //TREE_H
