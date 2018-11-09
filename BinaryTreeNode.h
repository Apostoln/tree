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
    T mValue;
    Node *mParent = nullptr;
    Node *mLeft = nullptr;
    Node *mRight = nullptr;

    Node **getChild(const T &other) {
        return other < mValue ? &mLeft : &mRight;
    }

    Node **getChildOfParent() {
        if (mParent == nullptr) {
            return nullptr;
        }
        return mParent->mLeft == this? &(mParent->mLeft) : &(mParent->mRight);
    }

public:
    Node() = delete;

    Node(const T& val) : mValue(val) {}

    Node(const T& val, Node* parentNode): mValue(val), mParent(parentNode) {}

    Node* getMin() {
        Node* node = this;
        while(true) {
            if(nullptr == node->mLeft) {
                return node;
            }
            node = node->mLeft;
        }
    }

    Node* getMax() {
        auto node = this;
        while(true) {
            if(nullptr == node->mRight) {
                return node;
            }
            node = node->mRight;
        }
    }

    Node* find(const T& element) {
        Node* node = this;

        while (true) {
            if (element == node->mValue) {
                return node;
            }
            if (element < node->mValue) {
                if (nullptr == node->mLeft ) {
                    return nullptr;
                }
                node = node->mLeft;
            } else {
                if (nullptr == node->mRight) {
                    return nullptr;
                }
                node = node->mRight;
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

        if (nullptr == el->mRight &&
            nullptr == el->mLeft ) {
            auto childOfParent = el->getChildOfParent();
            *childOfParent = nullptr;
            delete (el);
        }
        else if (nullptr == el->mRight ) {
            auto childOfParent = el->getChildOfParent();
            *childOfParent = el->mLeft;
            el->mLeft->mParent = el->mParent;
            delete (el);
        }
        else if (nullptr == el->mLeft ) {
            auto childOfParent = el->getChildOfParent();
            *childOfParent = el->mRight;
            el->mRight->mParent = el->mParent;
            delete (el);
        }
        else {
            Node* minNodeInRightSubtree = el->mRight->getMin();
            el->mValue = minNodeInRightSubtree->mValue;
            el->mRight->remove(minNodeInRightSubtree->mValue);
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
                dfs(visit, node->mLeft, order);
                dfs(visit, node->mRight, order);
                return;
            }
            case Order::POST: {
                dfs(visit, node->mLeft, order);
                dfs(visit, node->mRight, order);
                visit(node);
                return;
            }
            case Order::IN: {
                dfs(visit, node->mLeft, order);
                visit(node);
                dfs(visit, node->mRight, order);
                return;
            }
            default: {
                assert(false);
            }
        }
    }

    friend std::ostream& operator << (std::ostream& out, const Node& node) {
        out << &node << " "
            << node.mValue << " "
            << node.mParent << " "
            << node.mLeft << " "
            << node.mRight << std::endl;
        return out;
    }
};

#endif //TREE_BINARYTREENODE_H
