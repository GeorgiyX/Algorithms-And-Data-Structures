#include <iostream>
#include <assert.h>

/// 1_2. Вычислить площадь выпуклого n-угольника, заданного координатами своих вершин.
/// Вначале вводится количество вершин, затем последовательно целочисленные координаты всех вершин в порядке обхода
/// против часовой стрелки.
/// n < 1000, координаты < 10000.
/// Указание. Для вычисления площади n-угольника можно посчитать сумму ориентированных площадей трапеций под
/// каждой стороной многоугольника.
/// Требования: Координата должна быть реализована в виде структуры.

struct Point {
    double x;
    double y;
};

double square(Point *arr, size_t len) {
    double square = 0;
    for (int i = 0; i < len; ++i) {
        auto point1 = arr[i];
        auto point2 = (i == len - 1) ? arr[0] : arr[i + 1];
        square += 0.5 * (point1.y + point2.y) * (point1.x - point2.x);
    }
    return std::abs(square);
}

int main() {
    size_t n = 0;
    std::cin >> n;
    assert(!(n >= 1000 || n < 3) && "Vertex count error!");

    auto arr = new Point[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i].x >> arr[i].y;
        assert(!(arr[i].x >= 10000 || arr[i].y >= 10000) && "Coordinate must be < 10000!");
    }
    std::cout << square(arr, n);
    delete[] arr;
    return 0;
}
