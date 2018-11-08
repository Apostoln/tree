#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include <queue>
#include <vector>

enum class Order { PRE, POST, IN };

template <class T>
class Tree {
public:
    struct Node {
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

        template <class K>
        friend std::ostream& operator << (std::ostream& out,  typename Tree<K>::Node node);
    };

private:
    Node *root = nullptr;

    void add(Node *root, const T &value) {
        if (Node **child = root->getChild(value); nullptr == *child) { // Указатель на указатель на нужного потомка
            *child = new Node{value, root, nullptr, nullptr};
            return;
        } else {
            add(*child, value);
        }
    }

public:
    Tree() = default;

    Tree(const Tree &) = default;

    Tree(Tree &&) = default;

    ~Tree() = default;

    void add(const T &value) {
        if (nullptr == root) {
            root = new Node{value, nullptr, nullptr, nullptr};
            return;
        }
        add(root, value);
    }


    T getMax() {
        auto node = root;
        while(true) {
            if(nullptr == node->right) {
                return node->value;
            }
            node = node->right;
        }
    }

    Node* getMinImpl(Node* node) {
        while(true) {
            if(nullptr == node->left) {
                return node;
            }
            node = node->left;
        }
    }

    T getMin() {
        return getMinImpl(root)->value;

    }

    Node* findImpl(const T& element, Node* node) {
        if (nullptr == root) {
            return nullptr;
        }

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

    T* find(const T& element) {
        return &(findImpl(element, root)->value);

    }

    bool removeImpl( const T& value, Node * node) {
        auto el = findImpl(value, node);
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
            Node* minElementInRightSubtree = getMinImpl(el->right);
            el->value = minElementInRightSubtree->value;
            removeImpl(minElementInRightSubtree->value, el->right);

        }
    }

    bool remove(const T& el) {
        return removeImpl(el, root);
    }

    template <class F>
    void bfs(F visit) {
        std::queue<Node*> q;
        q.push(root);
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
    void dfs(F visit, Node* node, Order order) {
        if (nullptr == node) {
            return;
        }

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

    template <class F>
    void dfs(F visit, Order order = Order::PRE) {
        dfs(visit, root, order);
    }

};

template <class T>
using Node = typename Tree<T>::Node;

int main() {
    Tree<int> t;
    t.add(42);
    t.add(33);
    t.add(56);
    t.add(556);
    t.add(4);
    t.add(557);
    t.add(558);
    t.add(559);
    t.add(40);
    auto print = [&] { t.bfs([](const auto& node) {
                        //std::cout << *node << std::endl;
                        std::cout << node << " "
                            << node->value << " "
                            << node->parent << " "
                            << node->left << " "
                            << node->right << std::endl;
                        });
    };

    print();

    t.remove(33);
    std::cout << std::endl;

    print();

    return 0;
}