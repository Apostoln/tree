#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <memory>
#include <experimental/memory>

template <class T>
class INode {
public:
    using UniqNode = std::unique_ptr<INode<T>>;
    using ObserveNode = std::experimental::fundamentals_v2::observer_ptr<INode<T>>;

    T mValue;
    ObserveNode mParent = nullptr;
    UniqNode mLeft = nullptr;
    UniqNode mRight = nullptr;


public:
    INode(const T& value): mValue{value} {}

    virtual INode *getMin() = 0;

    virtual INode *getMax() = 0;

    virtual INode *find() = 0;

    virtual UniqNode add(const T &) = 0;

    virtual bool remove(const T &) = 0;

    template<class F>
    void bfs(F);

    template<class F>
    void dfs(F);
};
#endif //TREE_NODE_H
