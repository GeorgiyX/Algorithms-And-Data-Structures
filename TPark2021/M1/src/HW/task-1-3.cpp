#include <iostream>
#include <cassert>

/**
 * 1_3. Даны два строго возрастающих массива целых чисел A[0..n) и B[0..m) и число k.
 * Найти количество таких пар индексов (i, j), что A[i] + B[j] = k. n, m ≤ 100000.
 * Указание. Обходите массив B от конца к началу.
 * Требования:  Время работы O(n + m).
 */


/**
 * @param k - параметр из условия, сумма элементов пары равна k.
 */
size_t getCountOfPair(const int *arrA, size_t arrASize, const int *arrB, size_t arrBSize, const int k) {
    assert(arrA && arrB && "null arguments");
    size_t lastIndexA = 0, pairCount = 0;
    int termA = 0;

    for (int i = arrBSize - 1; i >= 0 && lastIndexA < arrASize; --i) {
        termA = k - arrB[i];
        while (lastIndexA < arrASize && arrA[lastIndexA] < termA) {
            lastIndexA++;
        }
        if (lastIndexA < arrASize && arrA[lastIndexA] == termA) {
            pairCount++;
            lastIndexA++;
        }
    }

    return pairCount;
}

int main() {
    size_t sizeA = 0;
    std::cin >> sizeA;
    assert(sizeA <= 100000);
    auto arrA = new int[sizeA];
    for (size_t i = 0; i < sizeA; ++i) {
        std::cin >> arrA[i];
    }

    size_t sizeB = 0;
    std::cin >> sizeB;
    assert(sizeB <= 100000);
    auto arrB = new int[sizeB];
    for (size_t i = 0; i < sizeB; ++i) {
        std::cin >> arrB[i];
    }

    int k = 0;
    std::cin >> k;
    std::cout << getCountOfPair(arrA, sizeA, arrB, sizeB, k);

    delete[] arrA;
    delete[] arrB;
}
