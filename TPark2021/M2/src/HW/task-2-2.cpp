#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>
#include <functional>
#include <stack>
/// Parent не используем - лиший overhead по памяти
/// Для вывода на печать - функтор.

template<class T>
class BinaryTree {
public:
    struct Node {
        T data;
        Node *left = nullptr;
        Node *right = nullptr;
    };

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
    ~BinaryTree();

private:
    Node *_root;
};

template<class T>
void BinaryTree<T>::traversePostOrder(const std::function<void(Node *)> &callback) {
    if (!_root) { return; }
    std::stack<Node*> firstStack{_root};
    std::stack<Node*> secondStack;
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

template<class T>
BinaryTree<T>::~BinaryTree() {
    traverse([](Node *node){ delete node; });
}

template<class T>
void BinaryTree<T>::traversePreOrder(const std::function<void(Node *)> &callback) {
    if (!_root) { return; }
    std::stack<Node*> stack{_root};
    Node *current = nullptr;
    while (!stack.empty()) {
        current = stack.top();
        stack.pop();
        if (current->right) { stack.push(current->right); }
        if (current->left) { stack.push(current->left); }
        callback(current);
    }
}

int main() {
}