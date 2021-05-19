#include <iostream>
#include <sstream>
#include <cassert>

template<class K, class D, class C>
class AVLTree;

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

        Node(const K &k, const D &d) : key(k), data(d), left(nullptr), right(nullptr), height(1) {}

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

private:
    D *findAUX(const K &key, Node *node);

    D *insertAUX(const K &key, const D &data, Node *node);

    Node *eraseAUX(const K &key, Node *node);

    Node *balance(Node *node);

    Node *findAndRemoveMin(Node *node, Node **minNode);

    void fixHeight(Node *node);
    int balanceFactor(Node *node);
    char height(Node *node);

    void rotateLeft(Node *node);
    void rotateRight(Node *node);

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
    if (isEmpty()) { return nullptr; }
    return findAUX(key, _root);
}

template<class K, class D, class C>
D *AVLTree<K, D, C>::findAUX(const K &key, AVLTree::Node *node) {
    if (node) { return nullptr; }
    auto compResult = _compare(key, node->key);
    switch (compResult) {
        case C::LESS:
            return findAUX(key, node->left);
        case C::EQUAL:
            return &node->data;
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
    insertAUX(key, data, _root);
}

template<class K, class D, class C>
D *AVLTree<K, D, C>::insertAUX(const K &key, const D &data, AVLTree::Node *node) {
    if (!node) {
        _itemCount++;
        return new Node(key, data);
    }
    auto compResult = _compare(key, node->key);
    switch (compResult) {
        case C::LESS:
            node->left = insertAUX(key, node->left);
        case C::EQUAL:
            break;
        case C::MORE:
            node->right = insertAUX(key, node->right);
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
        case C::EQUAL: {
            _itemCount--;
            auto left = node->left;
            auto right = node->right;
            delete node;

            if (!right) { return left; }
            Node *minNode = nullptr;
            auto treeWithoutMinNode = findAndRemoveMin(right, &minNode);
            minNode->right = treeWithoutMinNode;
            minNode->left = left;
            return balance(minNode);
        }
        case C::MORE:
            node->right = eraseAUX(key, node->right);
    }
    return balance(node);
}

template<class K, class D, class C>
typename AVLTree<K, D, C>::Node *AVLTree<K, D, C>::findAndRemoveMin(AVLTree::Node *node, AVLTree::Node **minNode) {
    if(!node->left) {
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
    eraseAUX(key, _root);
}

int main() {
    AVLTree<int, int> tree;
    tree.find(10);
    return 0;
}
