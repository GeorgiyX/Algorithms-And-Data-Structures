#include <iostream>
#include <stack>
#include <unordered_map>

template<class T>
struct Node {
    T data;
    Node *left;
    Node *right;
};

template<class T>
bool isTreeDataSame(Node<T> *root) {
    if (!root) { return true; }
    auto data = root->data;
    std::stack<Node<T>*> stack;
    stack.push(root);
    while (!stack.empty()) {
        auto node = stack.top();
        stack.pop();
        if (node->data != data) { return false; }
        if (node->right) { stack.push(node->right); }
        if (node->left) { stack.push(node->left); }
    }
    return true;
}

struct Person {
    std::string FirstName;
    std::string LastName;
    int ChildCount;
};




struct isLess {
    bool operator()(const Person &lhs, const Person &rhs) {
        return lhs.ChildCount < rhs.ChildCount || lhs.FirstName < rhs.FirstName || lhs.LastName < rhs.LastName;
    }
};


int main() {
    Node<int> node{100, nullptr, nullptr};
    isTreeDataSame(&node);
    return 0;
}
