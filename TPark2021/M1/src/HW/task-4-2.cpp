#include <iostream>
#include <cstring>
#include <cassert>
#include <sstream>

/**
 * Решение всех задач данного раздела предполагает использование кучи, реализованной в виде класса.
 * Решение должно поддерживать передачу функции сравнения снаружи.
 *
 * Для сложения чисел используется старый компьютер. Время, затрачиваемое
 * на нахождение суммы двух чисел равно их сумме.
 * Таким образом для нахождения суммы чисел 1,2,3 может потребоваться разное время,
 * в зависимости от порядка вычислений.
 * ((1+2)+3) -> 1+2 + 3+3 = 9
 * ((1+3)+2) -> 1+3 + 4+2 = 10
 * ((2+3)+1) -> 2+3 + 5+1 = 11
 * Требуется написать программу, которая определяет минимальное время, достаточное
 * для вычисления суммы заданного набора чисел.
 *
 * Формат входных данных. Вначале вводится n - количество чисел. Затем вводится n
 * строк - значения чисел (значение каждого числа не превосходит 10^9, сумма всех
 * чисел не превосходит 2*10^9).
 * Формат выходных данных. Натуральное число - минимальное время.
 *
 * Что будет на вершине кучи - min или max зависит от функции сравнения.
 * Про кучу известно: верхних элемент или min или max. Все элементы больше или равны/ мненьше или равны.
 */

const size_t RESIZE_MULTIPLIER = 2;
const size_t DEFAULT_SIZE = 2;

/**
 * Немного измененный вектор из задания 3_3.
 */
template<class T>
class Vector {
    size_t _nextPushIndex;
    size_t _bufSize;
    T *_buf;

    void resize() {
        auto oldBuf = _buf;
        _bufSize = _bufSize * RESIZE_MULTIPLIER;
        _buf = new T[_bufSize];
        std::memcpy(_buf, oldBuf, size() * sizeof(T));
        delete[] oldBuf;
    };

public:
    Vector(size_t size, T defaultValue) : _nextPushIndex(size), _bufSize(size), _buf(new T[_bufSize]) {
        for (size_t i = 0; i < _bufSize; ++i) {
            _buf[i] = defaultValue;
        }
    };

    Vector() : _nextPushIndex(0), _bufSize(DEFAULT_SIZE), _buf(new T[_bufSize]) {};

    Vector(Vector &&rhs) : _nextPushIndex(rhs._nextPushIndex),
                           _bufSize(rhs._bufSize), _buf(rhs._buf) {
        rhs._bufSize = DEFAULT_SIZE;
        rhs._buf = new T[rhs._bufSize];
        rhs._nextPushIndex = 0;
    };

    ~Vector() {
        delete[] _buf;
    };

    Vector(const Vector &rhs) = delete;

    Vector &operator=(const Vector &rhs) = delete;

    Vector &operator=(Vector &&rhs) = delete;

    T &operator[](size_t index) {
        assert(index < size());
        return _buf[index];
    };

    const T &operator[](size_t index) const {
        assert(index < size());
        return _buf[index];
    };

    T &last() {
        assert(size());
        return _buf[size() - 1];
    }

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
bool isLessOrEqual(const T &lhs, const T &rhs) {
    return lhs < rhs;
}
template<class T>
struct isLessOrEqualFunctor {
    bool operator()(const T &lhs, const T &rhs) {
        return lhs <= rhs;
    }
};

template<class T, class C = isLessOrEqualFunctor<T>>
class Heap {
    Vector<T> _arr;
    C _compare;

    void siftUp(size_t index) {
        size_t parent = (index - 1) / 2; // Может уйти за 0, но ничего страшного
        while (true) {
            if (index == 0 || _compare(_arr[parent], _arr[index])) { break; }
            std::swap(_arr[index], _arr[parent]);
            index = parent;
            parent = (index - 1) / 2;
        }
    }

    void siftDown(size_t index) {
        size_t leftChild = 0, rightChild = 0, lowestChild = 0;
        while (true) {
            leftChild = index * 2 + 1;
            rightChild = index * 2 + 2;
            lowestChild = rightChild;
            if (leftChild >= _arr.size()) { break; }
            if (rightChild >= _arr.size() || _compare(_arr[leftChild], _arr[rightChild])) {
                lowestChild = leftChild;
            }
            if (_compare(_arr[index], _arr[lowestChild])) { break; }
            std::swap(_arr[index], _arr[lowestChild]);
            index = lowestChild;
        }
    }

    void buildHeap() {
        for (int i = _arr.size(); i >= 0; --i) {
            siftDown(i);
        }
    };

public:
    Heap() : _arr(), _compare() { }

    Heap(Vector<T> &&arr, const C &compare = C()) : _arr(std::move(arr)), _compare(compare) {
        buildHeap();
    }
    Heap(const Heap &) = delete;
    Heap &operator=(const Heap &) = delete;
    Heap &operator=(Heap &&) = delete;
    ~Heap() = default;

    const T &top() const {
        assert(size() > 0);
        return _arr[0];
    }

    void popTop() {
        assert(size() > 0);
        _arr[0] = _arr.last();
        _arr.popBack();
        siftDown(0);
    }

    void push(const T &data) {
        _arr.pushBack(data);
        siftUp(_arr.size() - 1);
    }

    bool isEmpty() {
        return !_arr.size();
    }

    size_t size() const {
        return _arr.size();
    }
};

void testHeap() {
    {
        Vector<int> arr;
        for (int i = 100; i >= 0; --i) {
            arr.pushBack(i);
        }
        Heap<int, bool(*)(const int &, const int&)> heap(std::move(arr), isLessOrEqual);

        int prev = heap.top();
        heap.popTop();
        while (!heap.isEmpty()) {
            assert(heap.top() >= prev);
            heap.popTop();
        }
    }

    {
        Heap<int> heap;
        for (int i = 100; i > 0; --i) {
            heap.push(i);
        }
        for (int i = 100; i > 0; --i) {
            heap.push(i);
        }
        heap.push(1110);
        heap.push(0);

        int prev = heap.top();
        heap.popTop();
        while (!heap.isEmpty()) {
            assert(heap.top() >= prev);
            heap.popTop();
        }
    }
}

int getBestSumTime(Vector<int> &&arr) {
    Heap heap(std::move(arr));
    int time = 0, sum = 0, rhs = 0, lhs = 0;
    while (heap.size() > 1) {
        lhs = heap.top();
        heap.popTop();
        rhs = heap.top();
        heap.popTop();
        sum = lhs + rhs;
        time += sum;
        heap.push(sum);
    }
    return time;
}

void testCase(std::istream &in, std::ostream &out) {
    size_t n = 0;
    in >> n;
    Vector<int> arr(n, 0);
    for (size_t i = 0; i < n; ++i) {
        in >> arr[i];
    }
    out << getBestSumTime(std::move(arr));
}

void test() {
    {
        std::stringstream in("5\n5 2 3 4 6");
        std::stringstream out;
        testCase(in, out);
        assert(out.str() == "45");
    }
    {
        std::stringstream in("5\n3 7 6 1 9");
        std::stringstream out;
        testCase(in, out);
        assert(out.str() == "56");
    }
}

int main() {
    testCase(std::cin, std::cout);
    return 0;
}
