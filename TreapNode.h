#ifndef TREE_TREAPNODE_H
#define TREE_TREAPNODE_H

#include <random>
#include "Node.h"

template <class T>
class TreapNode : public INode <T> {
public:
    int mPriority;

    using typename INode<T>::UniqNode;
    using typename INode<T>::ObserveNode;

    using BaseNode = INode<T>;

    TreapNode(const T& val) : INode<T>{val} {
        std::random_device rd;
        std::mt19937 engine(rd());
        std::uniform_int_distribution<> distr(0, 1 << 10);
        mPriority = distr(engine);
    }

    static UniqNode merge(UniqNode left, UniqNode right) { // left < right
        if (nullptr == left || nullptr == right) {
            return nullptr == left? std::move(right) : std::move(left);
        }

        if (left->mPriority > right->mPriority) {
            left->mRight = merge(left->mRight, right); //todo move?
            if (nullptr != left->mRight) {
                left->mRight->mParent.reset(left);
            }
            return left;
        }
        else {
            right->mLeft = merge(left, right->mLeft);
            if (nullptr != right->mLeft) {
                right->mLeft->mParent.reset(right);
            }
            return right;
        }
    }

    static void split(UniqNode node, const T& key, UniqNode& left, UniqNode& right) {
        if (nullptr == node) {
            left = nullptr;
            right = nullptr;
        }
        else if (node->mValue <= key) {
            split(std::move(node->mRight), key, node->mRight, right);
            node->mParent = left->mParent;
            left = std::move(node); //move
        }
        else {
            split(std::move(node->mLeft), key, left, node->mLeft);
            node->mParent = right->mParent;
            right = std::move(node); //move
        }
    }

    UniqNode add(const T& value) override {
        UniqNode left, right;
        split(std::move(UniqNode(this)), value, left, right);
        UniqNode node{new TreapNode{value}};
        UniqNode tmpResult = merge(std::move(left), std::move(node));
        return merge(std::move(tmpResult), std::move(right));

    }

    BaseNode *getMin() { return this;}

    BaseNode *getMax() { return this;}

    BaseNode *find() { return this;}

    bool remove(const T &) { return true;}

};
#endif //TREE_TREAPNODE_H
