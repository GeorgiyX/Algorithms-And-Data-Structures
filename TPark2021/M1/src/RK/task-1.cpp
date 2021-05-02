#include <iostream>
#include <cassert>

/**
 * Сдвинутый массив
 * Дан массив, который получен путем нескольких циклических сдвигов исходного отсортированного массива.
 * Нужно найти индекс элемента, с которого начинался исходный массив.
 * Примеры:
 * [12, 14, 16, 17, 1, 3, 5, 7] (исходный массив [1, 3, 5, 7, 12, 14, 16, 17])
 * Ответ: 4
 * [5, 5, 5, 1] (исходный массив [1, 5, 5, 5])
 * Ответ: 3
 * Требование: сложность O(logN)
 */


int getSmallest(const int *arr, size_t arrSize) {
    assert(arr && arrSize && "arr is empty");

    size_t rightBorder = arrSize / 2;
    size_t leftBorder = 0;
    int lowestIndex = 0;
    size_t lastRightBorder = arrSize - 1;

    while (leftBorder != rightBorder) {
        if (arr[leftBorder] > arr[rightBorder]) {
            lastRightBorder = rightBorder;
            rightBorder = leftBorder + (rightBorder - leftBorder) / 2;
            lowestIndex = rightBorder + 1;
        } else {
            leftBorder = rightBorder;
            rightBorder = lastRightBorder;
        }
    }
    return lowestIndex;

}

int main() {
    size_t arrSize = 0;
    std::cin >> arrSize;
    int *arr = new int[arrSize];
    for (int i = 0; i < arrSize; ++i) {
        std::cin >> arr[i];
    }
    std::cout << getSmallest(arr, arrSize);
    delete[] arr;
}