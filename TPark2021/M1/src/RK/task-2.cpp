#include <cassert>
#include <iostream>

/**
 * Правильная скобочная последовательность.
 * ВНИМАНИЕ! Для решения данной задачи нужно использовать структуру данных стек!
 * Дана последовательность, состоящая из символов '(' и ')'. Требуется определить,
 * является ли данная последовательность правильной скобочной последовательностью.
 * Длина исходной последовательности ≤ 800000.
 *
 * Формат ввода:
 * Строка, содержащая символы '(' и ')'.
 * Формат вывода:
 * YES или NO, в зависимости от того, является ли скобочная последовательность правильной.
 * Пример 1.
 * Ввод: () Вывод: YES
 * Пример 2.
 * Ввод: )(	Вывод: NO
 */

template<class T>
class Stack {
    struct StackNode {
        T data;
        StackNode *next;
    };
    StackNode *top;
public:
    T pop() {
        assert(top);
        auto data = top->data;
        auto next = top->next;
        delete top;
        top = next;
        return data;
    };
    void push(T data) {
        auto node = new StackNode{data, top};
        top = node;
    };

    bool isEmpty() const {
        return top == nullptr;
    };

    Stack() : top(nullptr) {
    };
    ~Stack() {
        while (top) {
            auto next = top->next;
            delete top;
            top = next;
        }
    }
};

template <class T>
bool isValidBracketsSequence(Stack<T> &stack) {
    size_t openBracketsCnt = 0;
    size_t closeBracketsCnt = 0;
    bool result = true;
    while (result && !stack.isEmpty()) {
        auto bracket = stack.pop();
        bracket == ')' ? closeBracketsCnt++ : openBracketsCnt++;
        if (bracket == '(' && openBracketsCnt > closeBracketsCnt) {
            result = false;
        }
    }
    return result;
}

void testCase(std::istream &in, std::ostream &out) {
    Stack<char> stack;
    char ch = '\0';
    while (std::cin >> ch) {
        assert(ch == ')' || ch == '(');
        stack.push(ch);
    }
    out << (isValidBracketsSequence(stack) ? "YES" : "NO");
}

int main() {
    testCase(std::cin, std::cout);
}
