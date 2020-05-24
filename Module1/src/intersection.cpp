#include <iostream>

/// 2_3. Даны два массива неповторяющихся целых чисел, упорядоченные по возрастанию.
/// A[0..n-1] и B[0..m-1]. n >> m. Найдите их пересечение.
/// Требования: Время работы: O(m * log k), где k - позиция элемента B[m-1] в массиве A..
/// В процессе поиска очередного элемента B[i] в массиве A пользуйтесь результатом поиска элемента B[i-1].
/// Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон для бинарного поиска размером порядка k,
/// а потом уже в нем делать бинарный поиск.

struct Array {
    int *arr = nullptr;
    int len = 0;
};

class LinkedList {
private:
    struct Node {
        int data = 0;
        Node *next = nullptr;
    };

    Node *_head;
    Node *_tail;

public:
    LinkedList() : _head(nullptr), _tail(nullptr) {};

    LinkedList(const LinkedList &linkedList) = delete;

    LinkedList &operator=(const LinkedList &linkedList) = delete;

    void pushBack(const int &val) {
        auto node = new Node{.data = val, .next = nullptr};
        if (_head == nullptr) {
            _head = node;
        } else {
            _tail->next = node;
        }
        _tail = node;
    };

    virtual ~LinkedList() {
        auto nodeToRemove = _head;
        while (nodeToRemove) {
            auto tempNode = nodeToRemove->next;
            delete nodeToRemove;
            nodeToRemove = tempNode;
        }
    }

    friend std::ostream &operator<<(std::ostream &out, LinkedList *linkedList);
};

std::ostream &operator<<(std::ostream &out, LinkedList *linkedList) {
    auto nodeToPrint = linkedList->_head;
    while (nodeToPrint) {
        out << nodeToPrint->data << " ";
        nodeToPrint = nodeToPrint->next;
    }
    return out;
}

int binarySearch(int *arr, int leftBorder, int rightBorder, int element) {
    if (leftBorder == rightBorder) {
        return arr[leftBorder] == element ? leftBorder : -1;
    }
    int middle = leftBorder + (rightBorder - leftBorder) / 2;
    return arr[middle] >= element ? binarySearch(arr, leftBorder, middle, element) : binarySearch(arr, middle + 1,
                                                                                                  rightBorder, element);
}

LinkedList *getIntersect(const Array &arrA, const Array &arrB) {
    auto linkedList = new LinkedList;
    int leftBorder = 0, rightBorder = 1;
    for (int i = 0; i < arrB.len; ++i) {
        while (rightBorder < arrA.len && arrA.arr[rightBorder] < arrB.arr[i]) {
            rightBorder *= 2;
        }
        leftBorder = rightBorder / 2;
        rightBorder = (rightBorder >= arrA.len) ? arrA.len - 1 : rightBorder;
        int index = binarySearch(arrA.arr, leftBorder, rightBorder, arrB.arr[i]);
        if (index != -1) {
            linkedList->pushBack(arrB.arr[i]);
            rightBorder = index + 1;
        }
    }
    return linkedList;
}

int main() {
    Array arrA, arrB;
    std::cin >> arrA.len >> arrB.len;
    arrA.arr = new int[arrA.len];
    for (int i = 0; i < arrA.len; ++i) {
        std::cin >> arrA.arr[i];
    }
    arrB.arr = new int[arrB.len];
    for (int i = 0; i < arrB.len; ++i) {
        std::cin >> arrB.arr[i];
    }
    auto linkedList = getIntersect(arrA, arrB);
    std::cout << linkedList;
    delete linkedList;
    delete[] arrA.arr;
    delete[] arrB.arr;
    return 0;
}
