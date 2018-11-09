#ifndef TREE_BINARYTREENODE_H
#define TREE_BINARYTREENODE_H

#include <iostream>
#include <memory>
#include <cassert>
#include <queue>

#include "Order.h"

template <class T>
class Node {
private:
    T value;
    Node *parent = nullptr;
    Node *left = nullptr;
    Node *right = nullptr;

    Node **getChild(const T &other) {
        return other < value ? &left : &right;
    }

    Node **getChildOfParent() {
        if (parent == nullptr) {
            return nullptr;
        }
        return parent->left == this? &(parent->left) : &(parent->right);
    }

public:
    Node() = delete;

    Node(const T& val) : value(val) {}

    Node(const T& val, Node* parentNode): value(val), parent(parentNode) {}

    Node* getMin() {
        Node* node = this;
        while(true) {
            if(nullptr == node->left) {
                return node;
            }
            node = node->left;
        }
    }

    Node* getMax() {
        auto node = this;
        while(true) {
            if(nullptr == node->right) {
                return node;
            }
            node = node->right;
        }
    }

    Node* find(const T& element) {
        Node* node = this;

        while (true) {
            if (element == node->value) {
                return node;
            }
            if (element < node->value) {
                if (nullptr == node->left ) {
                    return nullptr;
                }
                node = node->left;
            } else {
                if (nullptr == node->right) {
                    return nullptr;
                }
                node = node->right;
            }
        }
    }

    void add(const T& value) {
        Node* node = this;
        if (Node **child = node->getChild(value); nullptr == *child) { // Указатель на указатель на нужного потомка
            *child = new Node{value, node};
            return;
        } else {
            (*child)->add(value);
        }
    }

    bool remove(const T& value) {
        Node* node = this;
        auto el = node->find(value);
        if (nullptr == el) {
            return false;
        }

        if (nullptr == el->right &&
            nullptr == el->left ) {
            auto childOfParent = el->getChildOfParent();
            *childOfParent = nullptr;
            delete (el);
        }
        else if (nullptr == el->right ) {
            auto childOfParent = el->getChildOfParent();
            *childOfParent = el->left;
            el->left->parent = el->parent;
            delete (el);
        }
        else if (nullptr == el->left ) {
            auto childOfParent = el->getChildOfParent();
            *childOfParent = el->right;
            el->right->parent = el->parent;
            delete (el);
        }
        else {
            Node* minNodeInRightSubtree = el->right->getMin();
            el->value = minNodeInRightSubtree->value;
            el->right->remove(minNodeInRightSubtree->value);
        }
    }

    template <class F>
    void bfs(F visit) {
        std::queue<Node*> q;
        q.push(this);
        while (!q.empty()) {
            auto node = q.front();
            q.pop();
            visit(node);
            if (nullptr != node->left) {
                q.push(node->left);
            }
            if (nullptr != node->right) {
                q.push(node->right);
            }
        }
    }

    template <class F>
    void dfs(F visit, Order order) {
        Node* node = this;

        switch (order) {
            case Order::PRE: {
                visit(node);
                dfs(visit, node->left, order);
                dfs(visit, node->right, order);
                return;
            }
            case Order::POST: {
                dfs(visit, node->left, order);
                dfs(visit, node->right, order);
                visit(node);
                return;
            }
            case Order::IN: {
                dfs(visit, node->left, order);
                visit(node);
                dfs(visit, node->right, order);
                return;
            }
            default: {
                assert(false);
            }
        }
    }

    friend std::ostream& operator << (std::ostream& out, const Node& node) {
        out << &node << " "
            << node.value << " "
            << node.parent << " "
            << node.left << " "
            << node.right << std::endl;
        return out;
    }
};

#endif //TREE_BINARYTREENODE_H
