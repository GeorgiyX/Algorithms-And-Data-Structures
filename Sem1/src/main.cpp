//
// Created by adminu on 05.03.2020.
//
#include <iostream>
#include <assert.h>
#include "Queue.h"

// 2_1 походу
int main() {
    std::cout << x;
    x= 2;
    Queue<int> queue;
    int num = 0;
    std::cin >> num;
    bool result = true;
    for (int i = 0; i < num; ++i) {
        int command = 0;
        int data = 0;
        std::cin >> command >> data;
        switch  (command) {
            case 2:
                if (queue.isEmpty())
                    result = result && data == -1;
                else
                    result = result && data == queue.dequeue();
                break;
            case 3:
                queue.enquaue(data);
                break;
            default:
                assert(false);
                break;
        }
    }

    std::cout << (result ? "YES" : "NO");
    return 0;
}

// Тесты с ипользование stringstram - эмулирующим std::cin.. .