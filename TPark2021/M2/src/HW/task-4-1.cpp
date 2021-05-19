#include <iostream>
#include <sstream>
#include <cassert>
#include <random>
#include <algorithm>

template<class T>
struct Compare {
    enum Comparison {
        LESS,
        EQUAL,
        MORE
    };

    Comparison operator()(const T &lhs, const T &rhs) {
        if (lhs < rhs) { return LESS; }
        if (lhs > rhs) { return MORE; }
        return EQUAL;
    };
};

template<class K, class D, class C = Compare<K>>
class AVLTree {
    struct Node {
        K key;
        D data;
        Node *left;
        Node *right;
        char height;
        size_t elementCount;

        Node(const K &k, const D &d) : key(k), data(d), left(nullptr), right(nullptr), height(1), elementCount(1) {}

        ~Node() {
            delete left;
            delete right;
        }

        Node(const Node &rhs) = delete;

        Node &operator=(const Node &rhs) = delete;

        Node(Node &&rhs) = delete;

        Node &operator=(Node &&rhs) = delete;
    };

public:
    explicit AVLTree(const C &compare = C());

    ~AVLTree() { delete _root; }

    AVLTree(const Node &rhs) = delete;

    AVLTree &operator=(const AVLTree &rhs) = delete;

    AVLTree(AVLTree &&rhs) = delete;

    AVLTree &operator=(AVLTree &&rhs) = delete;

    D *find(const K &key);

    void insert(const K &key, const D &data);

    void erase(const K &key);

    size_t size();

    bool isEmpty();

protected:
    Node *findAUX(const K &key, Node *node);

    Node *insertAUX(const K &key, const D &data, Node *node);

    Node *eraseAUX(const K &key, Node *node);

    Node *balance(Node *node);

    Node *findAndRemoveMin(Node *node, Node **minNode);

    void fixHeight(Node *node);

    void fixElementCount(Node *node);

    int balanceFactor(Node *node);

    char height(Node *node);

    Node *rotateLeft(Node *node);

    Node *rotateRight(Node *node);

    C _compare;
    Node *_root;
    size_t _itemCount;
};

template<class K, class D, class C>
size_t AVLTree<K, D, C>::size() {
    return _itemCount;
}

template<class K, class D, class C>
bool AVLTree<K, D, C>::isEmpty() {
    return !_itemCount;
}

template<class K, class D, class C>
D *AVLTree<K, D, C>::find(const K &key) {
    auto node = findAUX(key, _root);
    return node ? &node->data : nullptr;
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::findAUX(const K &key, AVLTree::Node *node) {
    if (!node) { return nullptr; }
    auto compResult = _compare(key, node->key);
    switch (compResult) {
        case C::LESS:
            return findAUX(key, node->left);
        case C::EQUAL:
            return node;
        case C::MORE:
            return findAUX(key, node->right);
        default:
            assert(false);
    }
}

template<class K, class D, class C>
AVLTree<K, D, C>::AVLTree(const C &compare) : _compare(compare), _root(nullptr), _itemCount(0) {}

template<class K, class D, class C>
void AVLTree<K, D, C>::insert(const K &key, const D &data) {
    _root = insertAUX(key, data, _root);
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::insertAUX(const K &key, const D &data, AVLTree::Node *node) {
    if (!node) {
        _itemCount++;
        return new Node(key, data);
    }
    auto compResult = _compare(key, node->key);
    switch (compResult) {
        case C::LESS:
            node->left = insertAUX(key, data, node->left);
            break;
        case C::EQUAL:
            break;
        case C::MORE:
            node->right = insertAUX(key, data, node->right);
            break;
    }
    return balance(node);
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::eraseAUX(const K &key, AVLTree::Node *node) {
    if (!node) { return nullptr; }
    auto compResult = _compare(key, node->key);
    switch (compResult) {
        case C::LESS:
            node->left = eraseAUX(key, node->left);
            break;
        case C::EQUAL: {
            _itemCount--;
            auto left = node->left;
            auto right = node->right;
            node->left = nullptr;
            node->right = nullptr;
            delete node;

            if (!right) { return left; }
            Node *minNode = nullptr;
            auto treeWithoutMinNode = findAndRemoveMin(right, &minNode);
            minNode->right = treeWithoutMinNode;
            minNode->left = left;
            node = minNode;
            break;
        }
        case C::MORE:
            node->right = eraseAUX(key, node->right);
            break;
    }
    return balance(node);
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::findAndRemoveMin(AVLTree::Node *node, AVLTree::Node **minNode) {
    if (!node->left) {
        *minNode = node;
        return node->right;
    }
    node->left = findAndRemoveMin(node->left, minNode);
    return balance(node);
}

template<class K, class D, class C>
int AVLTree<K, D, C>::balanceFactor(Node *node) {
    return height(node->right) - height(node->left);
}

template<class K, class D, class C>
char AVLTree<K, D, C>::height(AVLTree::Node *node) {
    if (!node) { return 0; }
    return node->height;
}

template<class K, class D, class C>
void AVLTree<K, D, C>::fixHeight(AVLTree::Node *node) {
    if (node) { return; }
    node->height = std::max(node->left->height, node->right->height) + 1;
}

template<class K, class D, class C>
void AVLTree<K, D, C>::erase(const K &key) {
    _root = eraseAUX(key, _root);
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::balance(AVLTree::Node *node) {
    fixHeight(node);
    fixElementCount(node);
    auto balance = balanceFactor(node);
    if (balance == 2) {
        if (balanceFactor(node->right) < 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    } else if (balance == -2) {
        if (balanceFactor(node->left) > 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    return node;
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::rotateLeft(AVLTree::Node *node) {
    auto right = node->right;
    node->right = right->left;
    right->left = node;
    fixHeight(node);
    fixHeight(right);
    fixElementCount(node);
    fixElementCount(right);
    return right;
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::rotateRight(AVLTree::Node *node) {
    auto left = node->left;
    node->left = left->right;
    left->right = node;
    fixHeight(node);
    fixHeight(left);
    fixElementCount(node);
    fixElementCount(left);
    return left;
}

template<class K, class D, class C>
void AVLTree<K, D, C>::fixElementCount(AVLTree::Node *node) {
    if (node) { return; }
    node->height = std::max(node->left->elementCount, node->right->elementCount) + 1;
}


template<class K, class D, class C = Compare<K>>
class SolderAVLTree : public AVLTree<K, D, C> {
public:
    SolderAVLTree();

    void eraseOnPosition(size_t pos);

    size_t insert(const K &key);

private:
    using AVLTree<K, D, C>::eraseAUX;
    using AVLTree<K, D, C>::insertAUX;
    using AVLTree<K, D, C>::_root;
    using AVLTree<K, D, C>::_compare;
};

template<class K, class D, class C>
size_t SolderAVLTree<K, D, C>::insert(const K &) {
    return 0;
}


size_t getRandom(size_t max) {
    static std::mt19937 gen(22);
    std::uniform_int_distribution<size_t> distribution(0, max);
    return distribution(gen);
}

void testAVLTree() {
    AVLTree<int, int> tree;
    std::uniform_int_distribution<int> distribution(0, 100);
    std::mt19937 gen(22);

    for (int i = 0, key = 0; i < 20; ++i) {
        key = distribution(gen);
        tree.insert(key, i);
    }

    gen.seed(22);
    int *found = nullptr;
    for (int i = 0, key = 0; i < 20; ++i) {
        key = distribution(gen);
        found = tree.find(key);
        assert(*found == i);
        tree.erase(key);
        assert(!tree.find(key));
    }
    assert(tree.isEmpty());
}

int main() {
    testAVLTree();
    return 0;
}
