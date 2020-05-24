//
// Created by adminu on 05.03.2020.
//

#ifndef SEM1_QUEUE_H
#define SEM1_QUEUE_H

template <class T>
class Queue {
public:
    Queue();
    ~Queue();

public:
    /// Положить в очередь
    void enquaue(const T &data);
    /// Извлечь из очереди
    T dequeue();
    bool isEmpty() const; // Тк ничего не меняет

private:
    struct Node {
        Node() : Next(nullptr) {} // Тк реализщация в одну строку
        T Data;
        Node* Next;
    };

    Node* tail;
    Node* head;
};
extern int x;
// Head and Tail - указатели на говолу и хвост

//
// Created by adminu on 05.03.2020.
//

#include <assert.h>
#include "Queue.h"

template<class T>
Queue<T>::Queue() : tail(nullptr), head(nullptr) {}

template<class T>
void Queue<T>::enquaue(const T &data) {
    Node *newNode = new Node();
    newNode->Data = data;

    if (this->isEmpty()) {
        this->head = newNode;
    } else {
        this->tail->Next = newNode;
    }
    this->tail = newNode;
}

template<class T>
bool Queue<T>::isEmpty() const {
    return this->tail == nullptr;
}

template<class T>
T Queue<T>::dequeue() {
    assert( !this->isEmpty());
    Node* tmpNode = this->head;
    T tmpData = this->head->Data;
    if (this->head == nullptr) {
        this->tail = nullptr;
    }
    delete tmpNode;
    return tmpData;
}

template<class T>
Queue<T>::~Queue() {
    while (!this->isEmpty()) {
        this->dequeue();
    }
}


#endif //SEM1_QUEUE_H
