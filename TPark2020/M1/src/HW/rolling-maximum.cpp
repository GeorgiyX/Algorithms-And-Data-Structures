#include <cstring>
#include <cassert>
#include <iostream>
#include <sstream>

/// Требование для всех вариантов Задачи 4
/// Решение всех задач данного раздела предполагает использование кучи, реализованной в виде класса.
/// Решение должно поддерживать передачу функции сравнения снаружи.
/// Куча должна быть динамической.

/// 4_4. Скользящий максимум.
/// Дан массив натуральных чисел A[0..n), n не превосходит 10^8. Так же задан размер некоторого окна
/// (последовательно расположенных элементов массива) в этом массиве k, k<=n.

/// Требуется для каждого положения окна (от 0 и до n-k) вывести значение максимума в окне.
/// Требования: Скорость работы O(n log n), память O(n).
/// Формат входных данных. Вначале вводится n - количество элементов массива.
/// Затем вводится n строк со значением каждого элемента. Затем вводится k  - размер окна.

/// Формат выходных данных. Разделенные пробелом значения максимумов для каждого положения окна.

template<class T>
class DArray {
public:
    explicit DArray(T value = T(), size_t count = 0) : _bufferSize(count + 1), _nextIndex(count),
                                                       _buffer(new T[_bufferSize]) {
        for (size_t i = 0; i < count; ++i) {
            _buffer[i] = value;
        }
    };

    DArray(const DArray &array) :_bufferSize(array._bufferSize), _nextIndex(array._nextIndex), _buffer(new T[_bufferSize]) {
        for (int i = 0; i < array._nextIndex; ++i) {
            _buffer[i] = array._buffer[i];
        }
    }

    DArray &operator=(const DArray &array) = delete;

    T &operator[](size_t index) {
        assert(index <= _nextIndex && "Wrong index");
        return _buffer[index];
    }

    /// Вставляет элемент и возвращает его индекс
    size_t pushBack(T element) {
        if (isFull()) { resize(); }
        _buffer[_nextIndex] = element;
        return _nextIndex++;
    }

    size_t getSize() {
        return _nextIndex;
    }

    T &getLast() {
        assert(_nextIndex != 0 && "DArray in empty!");
        return _buffer[_nextIndex - 1];
    }

    /// "Удаляет" элемент и возвращает индекс последнего элемента
    size_t deleteLast() {
        assert(_nextIndex != 0 && "DArray in empty!");
        return (--_nextIndex - 1);
    }

    ~DArray() {
        delete[] _buffer;
    }

private:
    bool isFull() {
        return _nextIndex == _bufferSize;
    }

    void resize() {
        auto oldBuffer = _buffer;
        _buffer = new T[_bufferSize * 2];
        memcpy(_buffer, oldBuffer, _bufferSize * sizeof(T));
        _bufferSize *= 2;
        delete[] oldBuffer;
    }

    size_t _bufferSize;
    size_t _nextIndex;
    T *_buffer;
};

template<class T>
struct IsLessDefault {
    bool operator()(T &l, T &r) { return l < r; }
};

template<class T, class C = IsLessDefault<T>>
class Heap {
public:

    Heap() : _arr(), _isLess(C()) {}

    explicit Heap(const DArray<T> &arr, C isLess = C()) : _arr(arr), _isLess(isLess) {}

    Heap(const Heap &heap) = delete;

    Heap &operator=(const Heap &heap) = delete;

    virtual void insert(T element) {
        siftUp(_arr.pushBack(element));
    }

    virtual T extractTop() {
        assert(!isEmpty());
        auto top = _arr[0];
        _arr[0] = _arr.getLast();
        _arr.deleteLast();
        if (!isEmpty()) { siftDown(0); }
        return top;
    }

    bool isEmpty() {
        return _arr.getSize() == 0;
    }

    T getTop() {
        assert(!isEmpty());
        return _arr[0];
    }

    virtual ~Heap() = default;

    // Мб стоит сделать фабричный метод. Т.к. вызывать buildHead в конструкторе не вариант из-за п/класса.
    void buildHead() {
        for (int i = _arr.getSize() / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
    }

protected:

    virtual void siftDown(size_t index) {
        while (true) {
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;
            size_t maxChild = index;
            if (left < _arr.getSize() && _isLess(_arr[index], _arr[left])) { maxChild = left; }
            if (right < _arr.getSize() && _isLess(_arr[maxChild], _arr[right])) { maxChild = right; }
            if (maxChild == index) {
                break;
            }
            std::swap(_arr[maxChild], _arr[index]);
            index = maxChild;
        }
    }

    virtual void siftUp(size_t index) {
        while (index) {
            size_t parent = (index - 1) / 2;
            if (!_isLess(_arr[parent], _arr[index])) { break; }
            std::swap(_arr[index], _arr[parent]);
            index = parent;
        }
    }

    DArray<T> _arr;
    C _isLess;
};



/// Код для скользящего окна:

template<class T>
struct ElementWithIndex {
    T val;
    size_t index;
};

template<class T>
struct IsLessElementWithIndex {
    bool operator()(ElementWithIndex<T> *l, ElementWithIndex<T> *r) {
        return l->val < r->val;
    }
};

template<class T, class C = IsLessElementWithIndex<T>>
class HeapWithIndexes : public Heap<ElementWithIndex<T> *, C> {
public:
    HeapWithIndexes() : Heap<ElementWithIndex<T> *, C>() {}

    explicit HeapWithIndexes(const DArray<ElementWithIndex<T> *> &arr, C isLess = C()) :
        Heap<ElementWithIndex<T> *, C>(arr, isLess) {}

    HeapWithIndexes(const HeapWithIndexes &heapWithIndexes) = delete;

    HeapWithIndexes &operator=(const HeapWithIndexes &heapWithIndexes) = delete;

    ~HeapWithIndexes() override = default;

    void insert(ElementWithIndex<T> *element) override {
        element->index = _arr.getSize();
        siftUp(_arr.pushBack(element));
    }

    ElementWithIndex<T> *extractTop() override {
        assert(!isEmpty());
        auto top = _arr[0];
        _arr[0] = _arr.getLast();
        _arr[0]->index = 0;
        _arr.deleteLast();
        if (!isEmpty()) { siftDown(0); }
        return top;
    }

    void changeElement(ElementWithIndex<T> *oldElement, ElementWithIndex<T> *newElement) {
        newElement->index = oldElement->index;
        _arr[oldElement->index] = newElement;
        _isLess(oldElement, newElement) ? siftUp(newElement->index) : siftDown(newElement->index);
    }

    using Heap<ElementWithIndex<T> *, C>::isEmpty;

private:
    void siftDown(size_t index) override {
        while (true) {
            size_t left = 2 * index + 1;
            size_t right = 2 * index + 2;
            size_t maxChild = index;
            if (left < _arr.getSize() && _isLess(_arr[maxChild], _arr[left])) { maxChild = left; }
            if (right < _arr.getSize() && _isLess(_arr[maxChild], _arr[right])) { maxChild = right; }
            if (maxChild == index) {
                break;
            }
            std::swap(_arr[maxChild], _arr[index]);
            _arr[maxChild]->index = maxChild;
            _arr[index]->index = index;
            index = maxChild;
        }
    }

    void siftUp(size_t index) override {
        while (index) {
            size_t parent = (index - 1) / 2;
            if (!_isLess(_arr[parent], _arr[index])) { break; }
            std::swap(_arr[index], _arr[parent]);
            _arr[parent]->index = parent;
            _arr[index]->index = index;
            index = parent;
        }
    }

    using Heap<ElementWithIndex<T> *, C>::_isLess;
    using Heap<ElementWithIndex<T> *, C>::_arr;
};

template<class T, class C = IsLessElementWithIndex<T>>
class SlidingWindow {
public:
    SlidingWindow(ElementWithIndex<T> *array, size_t elementCnt, size_t windowSize) : _array(array),
                                                                                      _arraySize(elementCnt),
                                                                                      _windowSize(windowSize),
                                                                                      _windowPosition(0),
                                                                                      _heap() {
        assert(_windowSize <= _arraySize);
        for (int i = 0; i < _windowSize; ++i) {
            _heap.insert(&_array[i]);
        }
    }

    SlidingWindow(const SlidingWindow &slidingWindow) = delete;

    SlidingWindow &operator=(const SlidingWindow &slidingWindow) = delete;

    T getCurrentMaximum() {
        return _heap.getTop()->val;
    }

    T moveWindow() {
        assert(_windowPosition + _windowSize <= _arraySize);
        auto newElement = _array + _windowPosition + _windowSize;
        auto removeElement = _array + _windowPosition;
        _heap.changeElement(removeElement, newElement);
        _windowPosition++;
        return getCurrentMaximum();
    }


private:
    ElementWithIndex<T> *_array;
    size_t _arraySize;
    size_t _windowSize;
    size_t _windowPosition;
    HeapWithIndexes<T, C> _heap;
};

void testDArrray() {
    {
        DArray<int> arr;
        assert(arr.getSize() == 0 && "now size == 0");
        arr.pushBack(10);
        assert(arr.pushBack(20) == 1 && "return index == 1");
        assert(arr.getSize() == 2 && "now size == 2");
        assert(arr[0] == 10 && arr[1] == 20);
    }
    {
        DArray<int> arr(0, 1);
        assert(arr.getSize() == 1 && "now size == 1");
        arr.pushBack(10);
        arr.pushBack(20);
        assert(arr.getSize() == 3 && "now size == 3");
        assert(arr[0] == 0 && arr[2] == 20);
        assert(arr.getLast() == 20);
    }
}

void testHeap() {
    {
        DArray<int> arr;
        for (int i = 0; i < 15; ++i) {
            arr.pushBack(i);
        }
        Heap<int> heap(arr);
        heap.buildHead();
        assert(heap.getTop() == 14);
        for (int j = 14; j >= 0; --j) {
            assert(heap.extractTop() == j);
        }
    }
    {
        DArray<int> arr;
        for (int i = 0; i < 15; ++i) {
            arr.pushBack(i);
        }
        Heap<int, IsLessDefault<int>> heap(arr, IsLessDefault<int>());
        heap.buildHead();
        heap.insert(100);
        assert(heap.extractTop() == 100);
        for (int j = 14; j >= 0; --j) {
            assert(heap.extractTop() == j);
        }
        assert(heap.isEmpty());
    }
    {
        Heap<int> heap;
        heap.buildHead();
        for (int i = 0; i < 5; ++i) {
            heap.insert(i);
        }
        assert(heap.extractTop() == 4);
        heap.insert(10);
        heap.insert(100);
        assert(heap.extractTop() == 100);
    }
}

void testElementWithIndexHeap() {
    DArray<ElementWithIndex<int> *> dArray;
    auto arr = new ElementWithIndex<int>[15];
    for (int i = 0; i < 15; ++i) {
        arr[i].val = i + 1;
        arr[i].index = 100 + i;
        dArray.pushBack(&arr[i]);
    }
    HeapWithIndexes<int> heap(dArray);
    heap.buildHead();

    assert(heap.getTop()->val == 15);
    assert(heap.getTop()->index == 0);

    auto el1 = new ElementWithIndex<int>;
    auto el2 = new ElementWithIndex<int>;
    el1->index = 555;
    el1->val = 200;
    el2->index = 555;
    el2->val = 100;
    heap.insert(el1);
    heap.insert(el2);
    assert(heap.getTop()->val == 200);
    assert(heap.getTop()->index == 0);
    assert(el2->index == 1);

    assert(heap.extractTop()->val == 200);
    assert(heap.getTop()->val == 100);
    assert(heap.extractTop()->val == 100);

    heap.changeElement(&arr[0], el1);
    assert(heap.extractTop()->val == 200);


    for (int j = 14; j >= 1; --j) {
        assert(heap.extractTop()->val == j + 1);
    }
    assert(heap.isEmpty());
    delete[] arr;
    delete el1;
    delete el2;
};

void testCaseSlidingWindow(std::istream &in, std::ostream &out) {
    size_t arrSize = 0;
    in >> arrSize;
    auto arr = new ElementWithIndex<int>[arrSize];
    for (int i = 0; i < arrSize; ++i) {
        in >> arr[i].val;
        arr[i].index = i;
    }
    size_t windowSize = 0;
    in >> windowSize;

    SlidingWindow<int> slidingWindow(arr, arrSize, windowSize);
    out << slidingWindow.getCurrentMaximum() << " ";
    for (int j = 0; j < arrSize - windowSize; ++j) {
        slidingWindow.moveWindow();
        out << slidingWindow.getCurrentMaximum() << " ";
    }
    delete[] arr;
}

void testSlidingWindow() {
    {
        std::stringstream in, out;
        in << "3 1 2 3 2";
        testCaseSlidingWindow(in, out);
        assert(out.str() == "2 3 ");
    }
    {
        std::stringstream in, out;
        in << "9 0 7 3 8 4 5 10 4 6 4";
        testCaseSlidingWindow(in, out);
        assert(out.str() == "8 8 8 10 10 10 ");
    }
    {
        std::stringstream in, out;
        in << "9 0 7 3 8 4 5 10 4 6 9";
        testCaseSlidingWindow(in, out);
        assert(out.str() == "10 ");
    }
    {
        std::stringstream in, out;
        in << "9 0 7 3 8 4 5 10 4 6 1";
        testCaseSlidingWindow(in, out);
        assert(out.str() == "0 7 3 8 4 5 10 4 6 ");
    }
}

void testSlidingWindowIO() {
    testCaseSlidingWindow(std::cin, std::cout);
}


int main() {
    testSlidingWindow();
}
