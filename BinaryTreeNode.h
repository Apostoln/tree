#ifndef TREE_BINARYTREENODE_H
#define TREE_BINARYTREENODE_H

#include <iostream>
#include <memory>
#include <cassert>
#include <queue>
#include <experimental/memory>

#include "Order.h"

template <class T>
class Node : public INode<T> {
protected:
    using typename INode<T>::UniqNode;
    using typename INode<T>::ObserveNode;

protected:
    T mValue;
    ObserveNode mParent = nullptr;
    UniqNode mLeft = nullptr;
    UniqNode mRight = nullptr;

    UniqNode& getChild(const T &other) {
        return other < mValue ? mLeft : mRight;
    }

    UniqNode& getNullChild() {
        static UniqNode nullUniqNode;
        if (nullptr == mLeft ) {
            return mLeft;
        }
        if (nullptr == mRight) {
            return mRight;
        }
        return nullUniqNode;
    }

    UniqNode& getChildOfParent() {
        static UniqNode nullUniqNode;
        if (nullptr == mParent) {
            return nullUniqNode;
        }
        return mParent->mLeft.get() == this? mParent->mLeft : mParent->mRight;
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
            node = node->mLeft.get();
        }
    }

    Node* getMax() {
        Node* node = this;
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
                node = node->mLeft.get();
            } else {
                if (nullptr == node->mRight) {
                    return nullptr;
                }
                node = node->mRight.get();
            }
        }
    }

    virtual UniqNode add(const T& value) {
        Node* node = this;
        if (UniqNode& child = node->getChild(value); nullptr == child) { // Указатель на указатель на нужного потомка
            child.reset(new Node{value, node});
            return {this};
        } else {
            child->add(value);
        }
        return {this};
    }

    bool remove(const T& value) {
        Node* node = this;
        Node* el = node->find(value);
        if (nullptr == el) {
            return false;
        }

        if (nullptr == el->mRight &&
            nullptr == el->mLeft ) {
            UniqNode& childOfParent = el->getChildOfParent();
            childOfParent.reset();
        }
        else if (nullptr == el->mRight ) {
            UniqNode& childOfParent = el->getChildOfParent();
            childOfParent = std::move(el->mLeft);
            childOfParent->mParent = el->mParent;
        }
        else if (nullptr == el->mLeft ) {
            UniqNode& childOfParent = el->getChildOfParent();
            childOfParent = std::move(el->mRight);
            childOfParent->mParent = el->mParent;
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
            Node* node = q.front();
            q.pop();
            visit(node);
            if (nullptr != node->mLeft) {
                q.push(node->mLeft.get());
            }
            if (nullptr != node->mRight) {
                q.push(node->mRight.get());
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
            << node.mParent.get() << " "
            << node.mLeft.get() << " "
            << node.mRight.get() << std::endl;
        return out;
    }
};

#endif //TREE_BINARYTREENODE_H
