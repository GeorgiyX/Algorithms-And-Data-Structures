#include <iostream>
#include <cstring>

// Задача 6. Поиск k-ой статистики. Решили немного так как требует условие). Алгоритм взят из книжки Кормана.
int partition(int *arr, int l, int r) {
    // Нужно чтобы опорный элемент был максимально близок к медиане (по сему брать например крайний влоб не стоит)
    int pivot = arr[l];
    int i = l + 1; // Типо опорный должен быть в стороне (если мы бы выбрали рандомный - то его нужно былобы свопнуть в край к-либо)
    int j = r;
    while (true) {
        while (i <= r && arr[i] < pivot)
            i++;
        while
        if (i >= j) {
            std::swap(arr[i], arr[j]);
        }
    }

}

int findKStat(int *arr, int l, int r, int k) {
    int pivotPos = partition(arr, l, r);
    if (pivotPos == k) // Если повезло, и спервого раза нашли
        return arr[pivotPos];
    // В ДЗ рекусрсией нельзя. Пожалуйста, тк глубина 10000 и рекусрсия это дорого - копируются параметры возврат значения, пересоздавать лок. значения
    return k < pivotPos ? findKStat(arr, l, pivotPos - 1, k) : findKStat(arr, pivotPos + 1, r, k);
}

// Сортировка подсчетом. Если количество элементов слишком много то это не очень эффективно (тк походу если элементы -100 10 50 - будет массив 160 элементов и 160 сравнений чтото типо этого).
// Нестабильная сортировка:
void contSort(int *data, int n) {
    int minValue = data[0];
    int maxValue = data[0];
    for (int i = 0; i < n; ++i) {
        minValue = std::min(minValue, data[i]);
        maxValue = std::max(maxValue, data[i]);
    }
    int valuesCount = maxValue - minValue + 1;
    int *c= new int[valuesCount];
    memset(c, 0, valuesCount * sizeof(int));
    for (int j = 0; j < n; ++j) {
        c[data[j]-minValue]++; // Круто считаем количество элементов
    }
    int index = 0;
    for (int k = 0; k < valuesCount; ++k) {
        for (int i = 0; i < c[k]; ++i) {
            data[index] = minValue + i;
            index++;
        }
    }
}

void contSort(int *data, int n) {
    int minValue = data[0];
    int maxValue = data[0];
    for (int i = 0; i < n; ++i) {
        minValue = std::min(minValue, data[i]);
        maxValue = std::max(maxValue, data[i]);
    }
    int valuesCount = maxValue - minValue + 1;
    int *c= new int[valuesCount];
    memset(c, 0, valuesCount * sizeof(int));
    for (int j = 0; j < n; ++j) {
        c[data[j]-minValue]++; // Круто считаем количество элементов
    }

    for (int k = 0; k < valuesCount; ++k) {
        c[k] += c[k - 1];
    }
    int
}


int main() {
    int n = 0;
    int k = 0;
    std::cin >> n >> k;
    int *arr = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];

    }
    std::cout << findKStat(arr, 0, n - 1, k);
    return 0;
}
