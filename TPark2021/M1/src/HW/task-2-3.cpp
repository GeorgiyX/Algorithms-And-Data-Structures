#include <iostream>
#include <cassert>
#include <sstream>

/**
 * Задача 2_3. Даны два массива неповторяющихся целых чисел, упорядоченные по возрастанию.
 * A[0..n-1] и B[0..m-1]. n >> m. Найдите их пересечение.
 * Требования: Время работы: O(m * log k), где k - позиция элемента B[m-1] в массиве A..
 * В процессе поиска очередного элемента B[i] в массиве A пользуйтесь результатом поиска
 * элемента B[i-1]. Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон
 * для бинарного поиска размером порядка k с помощью экспоненциального поиска, а потом уже в нем
 * делать бинарный поиск.
 */

template<class T>
class Stack {
    struct Node {
        T data;
        Node *next = nullptr;
    };
    Node *_head = nullptr;

public:
    Stack() : _head(nullptr){};
    Stack(const Stack &) = delete;
    Stack(Stack &&rhs) : _head(rhs._head) {
        rhs._head = nullptr;
    };
    Stack &operator=(const Stack &) = delete;
    Stack &operator=(Stack &&) = delete;

    void push(const T &data) {
        auto node = new Node{data, _head};
        _head = node;
    }
    T pop() {
        assert(_head);
        auto nextHead = _head->next;
        auto data = _head->data;
        delete _head;
        _head = nextHead;
        return data;
    }

    bool isEmpty() const {
        return _head == nullptr;
    }

    ~Stack() {
        while (_head) {
            auto next = _head->next;
            delete _head;
            _head = next;
        }
    }

    template<class C>
    friend std::ostream &operator<<(std::ostream &out, Stack<C> &stack);
};

template<class C>
std::ostream &operator<<(std::ostream &out, Stack<C> &stack) {
    auto head = stack._head;
    while (head) {
        out << head->data << " ";
        head = head->next;
    }
    return out;
}

int binarySearch(const int *arr, int arrSize, int element) {
    assert(arr);
    int index = -1;
    int left = 0, right = arrSize - 1;
    int midElement = 0;
    int midIndex = 0;

    while (left <= right) {
        midIndex = (left + right) / 2;
        midElement = arr[midIndex];
        if (midElement == element) {
            index = midIndex;
            break;
        } else if (midElement > element) {
            right = midIndex - 1;
        } else {
            left = midIndex + 1;
        }
    }

    return index;
}

int exponentialWithBinarySearch(const int *arr, int arrSize, int element) {
    assert(arr);
    int right = 1;
    int left = 0;

    if (arr[left] == element) {
        return left;
    }
    while (right < arrSize && arr[right] < element) {
        right *= 2;
    }

    left = right / 2;
    auto newArrSize = right >= arrSize ? arrSize - left : right - left + 1;
    auto index = binarySearch(arr + left, newArrSize, element);
    return index != -1 ? left + index : -1;
}

Stack<int> getIntersections(const int *arrA, int sizeA, const int *arrB, int sizeB) {
    assert(arrA && arrB);
    Stack<int> result;
    int right = sizeA;
    int index = 0;

    for (int i = sizeB - 1; i >= 0 ; --i) {
        index = exponentialWithBinarySearch(arrA, right, arrB[i]);
        if (index >= 0) {
            right = index;
            result.push(arrB[i]);
        }
    }

    return result;
}


void testCase(std::ostream &out, std::istream &in) {
    int sizeA = 0, sizeB = 0;
    in >> sizeA >> sizeB;
    assert(sizeA <= 10000 && sizeB <=10000);
    auto arrA = new int[sizeA];
    auto arrB = new int[sizeB];
    for (int i = 0; i < sizeA; ++i) {
        in >> arrA[i];
    }
    for (int i = 0; i < sizeB; ++i) {
        in >> arrB[i];
    }
    auto result = getIntersections(arrA, sizeA, arrB, sizeB);
    out << result;
    delete []arrA;
    delete []arrB;
}


void test() {
    {
        std::stringstream in("5\n4\n1 2 3 4 5\n1 3 4 100");
        std::stringstream  out;
        testCase(out, in);
        assert(out.str() == "1 3 4 ");
    }
    {
        std::stringstream in("8\n8\n1 2 3 4 5 10 20 300\n1 2 3 4 5 10 20 300");
        std::stringstream  out;
        testCase(out, in);
        assert(out.str() == "1 2 3 4 5 10 20 300 ");
    }
}

int main() {
    testCase(std::cout, std::cin);
    return 0;
}
