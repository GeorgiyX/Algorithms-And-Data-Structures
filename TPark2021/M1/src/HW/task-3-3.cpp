#include <iostream>
#include <sstream>
#include <cassert>
#include <cstring>

/**
 * 3_3. Реализовать очередь с помощью двух стеков. Использовать стек, реализованный
 * с помощью динамического буфера. Требования: Очередь должна быть реализована в виде
 * класса. Стек тоже должен быть реализован в виде класса.
 */

const size_t RESIZE_MULTIPLIER = 2;
const size_t DEFAULT_SIZE = 2;

template<class T>
class Vector {
    size_t _nextPushIndex;
    size_t _bufSize;
    T *_buf;
    void resize() {
        std::cout << "resize!" << std::endl;
        auto oldBuf = _buf;
        _bufSize = _bufSize * RESIZE_MULTIPLIER;
        _buf = new T[_bufSize];
        for (int i = 0; i < _nextPushIndex; ++i) {
            _buf[i] = oldBuf[i];
        }
        delete[] oldBuf;
    };
public:
    Vector(size_t size, T defaultValue) : _nextPushIndex(size), _bufSize(size ? size : DEFAULT_SIZE),
                                          _buf(new T[_bufSize]) {
        for (size_t i = 0; i < size; ++i) {
            _buf[i] = defaultValue;
        }
    };
    Vector() : _nextPushIndex(0), _bufSize(DEFAULT_SIZE), _buf(new T[_bufSize]) {};
    ~Vector() {
        delete[] _buf;
    };
    Vector(const Vector& rhs) = delete;
    Vector &operator=(const Vector& rhs) = delete;
    Vector(Vector&& rhs) = delete;
    Vector &operator=(Vector&& rhs) = delete;

    T &operator[](size_t index) {
        assert(index < size());
        return _buf[index];
    };

    void pushBack(const T &value) {
        if (_nextPushIndex == _bufSize) {
            resize();
        }
        _buf[_nextPushIndex] = value;
        _nextPushIndex++;
    };

    /** Удаляет последний элемент */
    void popBack() {
        assert(size() > 0);
        _nextPushIndex--;
    }

    /**
     * @return число элементов в векторе
     */
    size_t size() const {
        return _nextPushIndex;
    };
};

template<class T>
class Stack {
    Vector<T> _vector;
public:
    Stack() : _vector() {};
    Stack(const Stack& rhs) = delete;
    Stack &operator=(const Stack& rhs) = delete;
    Stack(Stack&& rhs) = delete;
    Stack &operator=(Stack&& rhs) = delete;

    /**
     * Добавляет в стек
     * @param data - элемент для помещения в стек
     */
    void push(const T &data) {
        _vector.pushBack(data);
    };

    /** Удаляет вершину стека */
    void pop() {
        assert(!isEmpty());
        _vector.popBack();
    };

    /** Возвращает ссылку на вершину стека */
    T &top() {
        assert(!isEmpty());
        return _vector[_vector.size() - 1];
    };

    bool isEmpty() {
        return _vector.size() == 0;
    }
};

template<class T>
class Queue {
    Stack<T> _inStack;
    Stack<T> _outStack;

    /** Перемещает элементы из inStack в outStack */
    void transfer() {
        while (!_inStack.isEmpty()) {
            _outStack.push(_inStack.top());
            _inStack.pop();
        }
    };
public:
    Queue() : _inStack(), _outStack() {};
    ~Queue() = default;
    Queue(const Queue& rhs) = delete;
    Queue &operator=(const Queue& rhs) = delete;
    Queue(Queue&& rhs) = delete;
    Queue &operator=(Queue&& rhs) = delete;

    /** Помещает элемент в очередь */
    void push(const T& data) {
        _inStack.push(data);
    }

    /** Удаляет "first in" элемент */
    void pop() {
        assert(!isEmpty());
        _outStack.pop();
    };

    bool isEmpty() {
        if (_outStack.isEmpty()) {
            transfer();
        }
        return _outStack.isEmpty();
    }

    /** Ссылка на "first in" элемент */
    T &front() {
        assert(!isEmpty());
        return _outStack.top();
    }
};


void testCase(std::istream &in, std::ostream &out) {
    size_t commandCount = 0;
    in >> commandCount;
    assert(commandCount <= 1000000);

    size_t command = 0;
    int value = 0;
    Queue<int> queue;
    bool isOk = true;
    for (size_t i = 0; i < commandCount; ++i) {
        in >> command >> value;
        switch (command) {
            case 2:
                if (value == -1) {
                    if (!queue.isEmpty()) {
                        isOk = false;
                    }
                } else {
                    if (value != queue.front()) {
                        isOk = false;
                    }
                    queue.pop();
                }
                break;
            case 3:
                queue.push(value);
                break;
            default:
                assert(false);
        }
    }
    out << (isOk ? "YES" : "NO");
}

void test() {
    {
        std::stringstream in("3\n3 44\n3 50\n2 44");
        std::stringstream  out;
        testCase(in, out);
        assert(out.str() == "YES");
    }
    {
        std::stringstream in("2\n2 -1\n3 10");
        std::stringstream  out;
        testCase(in, out);
        assert(out.str() == "YES");
    }
    {
        std::stringstream in("2\n3 44\n2 66");
        std::stringstream  out;
        testCase(in, out);
        assert(out.str() == "NO");
    }
}

int main() {
    testCase(std::cin, std::cout);
    return 0;
}