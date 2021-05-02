#include <iostream>
#include <cstring>
#include <cassert>
#include <sstream>

/**
 * Требование для всех вариантов Задачи 5
 * Во всех задачах данного раздела необходимо реализовать и использовать сортировку слиянием.
 * Решение должно поддерживать передачу функции сравнения снаружи.
 * Общее время работы алгоритма O(n log n).
 *
 * 5_3. Закраска прямой 1.
 * На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов каждого отрезка
 * (Li и Ri). Найти длину окрашенной части числовой прямой.
 */


template<class T>
struct isLessFunctor {
    bool operator()(const T &lhs, const T &rhs) {
        return lhs < rhs;
    }
};

template<class T, class C = isLessFunctor<T>>
void mergeSort(T *arr, size_t size) {

}

int main() {

}