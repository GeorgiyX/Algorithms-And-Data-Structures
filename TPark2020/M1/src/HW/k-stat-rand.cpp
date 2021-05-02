#include <iostream>
#include <algorithm>
#include <random>
#include <cassert>
#include <sstream>

/// Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
/// Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции
/// с индексом k (0..n-1) в отсортированном массиве.

/// Требования: к дополнительной памяти: O(n). Среднее время работы: O(n). Должна быть отдельно выделенная
/// функция partition. Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.

/// Задание 6_4. Реализуйте стратегию выбора опорного элемента “случайный элемент”.
/// Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.

size_t getRandom(size_t min, size_t max) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<size_t> distribution(min, max);
    return distribution(gen);
}

template<class T>
struct IsLessDefault {
    bool operator()(T &l, T &r) { return l < r; }
};

template<class T, class C = IsLessDefault<T>>
size_t partition(T *arr, size_t l, size_t r, C isLess = C()) {
    std::swap(arr[l], arr[getRandom(l, r)]);
    T pivot = arr[l];
    size_t i = r, j = r;
    while (j > l || (pivot == arr[l] && j != i)) {
        while (isLess(arr[j], pivot)) { --j; }
        (j == l) ? std::swap(arr[j], arr[i]) : std::swap(arr[j--], arr[i--]);
    }
    return i;
}

template<class T, class C = IsLessDefault<T>>
T getKStat(T *arr, size_t l, size_t r, size_t k, C isLess = C()) {
    auto guessK = partition(arr, l, r, isLess);
    while (guessK != k) {
        guessK = (guessK > k) ? partition(arr, l, (r = guessK - 1), isLess) : partition(arr, (l = guessK + 1), r, isLess);
    }
    return arr[k];
}

void testCase(std::istream &in, std::ostream &out) {
    size_t arrSize = 0, k = 0;
    in >> arrSize >> k;
    assert(k < arrSize && "Wrong k");
    int *arr = new int[arrSize];
    for (size_t i = 0; i < arrSize; ++i) {
        in >> arr[i];
    }
    out << getKStat(arr, 0, arrSize - 1, k, IsLessDefault<int>());
    delete[] arr;
}

void test() {
    {
        std::stringstream in, out;
        in << "10 4 1 2 3 4 5 6 7 8 9 10";
        testCase(in, out);
        assert(out.str() == "5" && "Test 1 Failed");
        std::cout << "Success Test1: " << out.str() << std::endl;
    }
    {
        std::stringstream in, out;
        in << "10 0 3 6 5 7 2 9 8 10 4 1";
        testCase(in, out);
        assert(out.str() == "1" && "Test 2 Failed");
        std::cout << "Success Test2: " << out.str() << std::endl;
    }
    {
        std::stringstream in, out;
        in << "10 9 0 0 0 0 0 0 0 0 0 1";
        testCase(in, out);
        assert(out.str() == "1" && "Test 3 Failed");
        std::cout << "Success Test3: " << out.str() << std::endl;
    }
}

void testIO() {
    testCase(std::cin, std::cout);
}

int main() {
    testIO();
    return 0;
}