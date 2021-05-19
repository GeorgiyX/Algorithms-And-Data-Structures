#include <iostream>
#include <sstream>
#include <cassert>
#include <functional>
#include <stack>
/// Parent не используем - лиший overhead по памяти
/// Для вывода на печать - функтор.

template<class T>
struct Compare {
    bool operator()(const T &lhs, const T &rhs) {
        return lhs <= rhs;
    };
};

template<class T, class C = Compare<T>>
class BinaryTree {
public:
    struct Node {
        T data;
        Node *left = nullptr;
        Node *right = nullptr;
    };

    explicit BinaryTree(C comparator = C()) : _root(nullptr), _compare(comparator) {};

    ~BinaryTree();

    BinaryTree(const BinaryTree &rhs) = delete;

    BinaryTree &operator=(const BinaryTree &rhs) = delete;

    BinaryTree(BinaryTree &&rhs) = delete;

    BinaryTree &operator=(BinaryTree &&rhs) = delete;

    void push(const T &data);

    /**
     * Обход дерева в порядке post-order (порядок обхода: левое п/дерево, правое п/дерево, текущий узел).
     * @param callback - обработчик узла
     */
    void traversePostOrder(const std::function<void(Node *)> &callback);

    /**
     * Обход дерева в порядке pre-order (порядок обхода: текущий узел, левое п/дерево, правое п/дерево).
     * @param callback - обработчик узла
     */
    void traversePreOrder(const std::function<void(Node *)> &callback);


private:
    Node *_root;
    C _compare;
};

template<class T, class C>
void BinaryTree<T, C>::traversePostOrder(const std::function<void(Node *)> &callback) {
    if (!_root) { return; }
    std::stack<Node *> firstStack;
    std::stack<Node *> secondStack;
    firstStack.push(_root);
    Node *current = nullptr;
    while (!firstStack.empty()) {
        current = firstStack.top();
        firstStack.pop();
        if (current->left) { firstStack.push(current->left); }
        if (current->right) { firstStack.push(current->right); }
        secondStack.push(current);
    }
    while (!secondStack.empty()) {
        callback(secondStack.top());
        secondStack.pop();
    }
}

template<class T, class C>
void BinaryTree<T, C>::traversePreOrder(const std::function<void(Node *)> &callback) {
    if (!_root) { return; }
    std::stack<Node *> stack;
    stack.push(_root);
    Node *current = nullptr;
    while (!stack.empty()) {
        current = stack.top();
        stack.pop();
        if (current->right) { stack.push(current->right); }
        if (current->left) { stack.push(current->left); }
        callback(current);
    }
}

template<class T, class C>
BinaryTree<T, C>::~BinaryTree() {
    traversePostOrder([](Node *node) { delete node; });
}

template<class T, class C>
void BinaryTree<T, C>::push(const T &data) {
    auto ptrToCurrent = &_root;  // Указатель на указатель на текущий элемент в его родителе.
    auto current = _root;
    while (current) {
        if (_compare(current->data, data)) {
            ptrToCurrent = &current->right;
            current = current->right;
        } else {
            ptrToCurrent = &current->left;
            current = current->left;
        }
    }
    *ptrToCurrent = new Node{data, nullptr, nullptr};
}

void testCase(std::istream &in, std::ostream &out) {
    int size = 0, currentData = 0;
    in >> size;
    BinaryTree<int> tree;
    for (int i = 0; i < size; ++i) {
        in >> currentData;
        tree.push(currentData);
    }
    tree.traversePreOrder([&out](BinaryTree<int>::Node *node){out << node->data << " ";});
}

void test() {
    {
        std::stringstream in, out;
        in << "7 3 4 8 2 1 11 6";
        testCase(in, out);
        assert(out.str() == "3 2 1 4 8 6 11 ");
    }
}

int main() {
    testCase(std::cin, std::cout);
}