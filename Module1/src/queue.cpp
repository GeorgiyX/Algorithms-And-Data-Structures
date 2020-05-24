#include <iostream>
#include <cassert>
#include <sstream>
#include <cstring>

#define BUFFER_SIZE 2

/// 3_1. Реализовать очередь с динамическим зацикленным буфером.
/// Требования: Очередь должна быть реализована в виде класса.
template<class T>
class Queue {
public:
    explicit Queue(T buffer = BUFFER_SIZE);

    ~Queue();

    void enqueue(T data);

    T dequeue();

    bool isEmpty();

    bool isFull();

    void resize();

private:
    size_t _head;
    size_t _tail;
    T *_buffer;
    size_t _bufferSize;
};

template<class T>
Queue<T>::Queue(T buffer) : _head(0), _tail(0), _buffer(new T[buffer]), _bufferSize(buffer) {}

template<class T>
Queue<T>::~Queue() {
    delete[] _buffer;
}

template<class T>
void Queue<T>::enqueue(T data) {
    if (isFull()) {
        resize();
    }
    _buffer[_tail] = data;
    _tail = (_tail + 1) % _bufferSize;
}

template<class T>
T Queue<T>::dequeue() {
    assert(!isEmpty() && "buffer is empty");
    auto data = _buffer[_head];
    _head = (_head + 1) % _bufferSize;
    return data;
}

template<class T>
bool Queue<T>::isEmpty() {
    return _tail == _head;
}

template<class T>
bool Queue<T>::isFull() {
    return (_tail + 1) % _bufferSize == _head;
}

template<class T>
void Queue<T>::resize() {
    auto oldBuffer = _buffer;
    _buffer = new T[_bufferSize * 2];
    memmove(_buffer, oldBuffer, _tail * sizeof(T));
    if (_tail < _head) {
        size_t newHead = _bufferSize * 2 - (_bufferSize - _head);
        memmove(_buffer + newHead, oldBuffer + _head, (_bufferSize - _head) * sizeof(T));
        _head = newHead;
    }
    _bufferSize *= 2;
    delete[] oldBuffer;
}

void testCase(std::istream &in, std::ostream &out, size_t bufferSize = BUFFER_SIZE) {
    Queue<int> queue(bufferSize);
    size_t commandCnt = 0;
    in >> commandCnt;
    assert(commandCnt <= 1000000);
    bool result = true;
    for (int i = 0; i < commandCnt && result; ++i) {
        result = false;
        int currentCommand = 0, value = 0;
        in >> currentCommand >> value;
        switch (currentCommand) {
            case 2:  // pop front
                if ((queue.isEmpty() && value == -1) || value == queue.dequeue()) {
                    result = true;
                }
                break;
            case 3:  // push back
                queue.enqueue(value);
                result = true;
                break;
            default:
                assert (false && "command incorrect");
        }
    }
    result ? out << "YES" : out << "NO";
}

void test() {
    {
        std::cout << "test 1" << std::endl;
        std::stringstream in, out;
        in << "9 3 5 3 4 3 6 3 4 2 5 2 4 2 6 2 4 2 -1";
        testCase(in, out);
        assert(out.str() == "YES" && "Test: x8 push, x8 pop, x1 empty pop");
    }
    {
        std::cout << "test 2" << std::endl;
        std::stringstream in, out;
        in << "12 3 5 3 4 3 6 2 5 3 8 2 4 3 10 3 11 2 6 2 8 2 10 2 11";
        testCase(in, out, 2);
        assert(out.str() == "YES" &&
               "Test: x1 push (5), x1 resize push (4), x1 push (6), x1 pop (5), x1 push (8), x1 pop (4), x1 push (10), x1 resize push (11), x4 pop (6, 8, 10, 11) ");
    }
    {
        std::cout << "test 3" << std::endl;
        std::stringstream in, out;
        in << "2 3 44 2 66";
        testCase(in, out);
        assert(out.str() == "NO" && "Test 3");
    }
}


int main() {
    testCase(std::cin, std::cout);
    return 0;
}
