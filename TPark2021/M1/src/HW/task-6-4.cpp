#include <iostream>
#include <cstring>
#include <cassert>
#include <sstream>
#include <random>
#include <algorithm>

/**
 * Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
 * Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло
 * на позиции с индексом k (0..n-1) в отсортированном массиве.
 *
 * Требования: к дополнительной памяти: O(n). Среднее время работы: O(n). Должна быть отдельно
 * выделенная функция partition. Рекурсия запрещена. Решение должно поддерживать передачу
 * функции сравнения снаружи. Функцию Partition следует реализовывать методом прохода двумя
 * итераторами в одном направлении.
 *
 * 6_4. Реализуйте стратегию выбора опорного элемента “случайный элемент”. Функцию Partition
 * реализуйте методом прохода двумя итераторами от конца массива к началу.
 */

const size_t RANDOM_SEED = 225;

size_t getRandom(size_t max) {
    static std::mt19937 gen(RANDOM_SEED);
    std::uniform_int_distribution<size_t> distribution(0, max);
    return distribution(gen);
}

template<class T>
struct isHighFunctor {
    bool operator()(const T &lhs, const T &rhs) {
        return lhs > rhs;
    }
};

template<class T, class C = isHighFunctor<T>>
size_t partition(T *arr, size_t size, C compare = C()) {
    assert(arr && size);
    size_t iterNew = size - 1, iterHigh = size;
    std::swap(arr[getRandom(size - 1)], arr[0]);
    auto pivot = arr[0];
    while (iterNew > 0) {
        if (compare(arr[iterNew], pivot)) {
            std::swap(arr[--iterHigh], arr[iterNew]);
        }
        --iterNew;
    }
    --iterHigh;
    std::swap(arr[0], arr[iterHigh]);
    return iterHigh;
}

void testPartition() {
    {
        size_t arrSize = 20;
        auto arr = new int[arrSize];
        for (int i = 0; i < arrSize; ++i) {
            arr[i] = getRandom(50);
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "kstat: " << arr[partition(arr, arrSize)] << std::endl;
        for (int i = 0; i < arrSize; ++i) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
        delete[] arr;
    }
}

template<class T, class C = isHighFunctor<T>>
T &getKStatistic(T *arr, size_t size, size_t expectedStat, C compare = C()) {
    assert(arr && size && expectedStat < size);
    size_t randomStat = partition(arr, size, compare);
    size_t leftBorder = 0, rightBorder = size;
    while (randomStat != expectedStat) {
        if (randomStat > expectedStat) {
            rightBorder = randomStat;
        } else {
            leftBorder = randomStat + 1;
        }
        randomStat = leftBorder + partition(arr + leftBorder, rightBorder - leftBorder, compare);
    }
    return arr[expectedStat];
}

void testCase(std::istream &in, std::ostream &out) {
    size_t size = 0, statistic = 0;
    in >> size >> statistic;
    auto arr = new int[size];
    for (size_t i = 0; i < size; ++i) {
        in >> arr[i];
    }
    out << getKStatistic(arr, size, statistic);
    delete[] arr;
}

void test() {
    {
        std::stringstream in("10 4\n1 2 3 4 5 6 7 8 9 10");
        std::stringstream out;
        testCase(in,out);
        assert(out.str() == "5");
    }
    {
        std::stringstream in("10 5\n100 6 1 4 5 7 7 8 -10 0");
        std::stringstream out;
        testCase(in,out);
        assert(out.str() == "6");
    }
}


int main() {
    testCase(std::cin, std::cout);
    return 0;
}
