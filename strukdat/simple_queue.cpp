#include "../include/strukdat/simple_queue.hpp"
#include <stdexcept>
#include <string>

// Node constructor

// Implementation for SimpleQueue

template <typename T>
SimpleQueue<T>::Node::Node(const T& val) : data(val), next(nullptr) {}

template <typename T>
SimpleQueue<T>::SimpleQueue() : frontPtr(nullptr), backPtr(nullptr), count(0) {}

template <typename T>
SimpleQueue<T>::~SimpleQueue() {
    while (!isEmpty()) dequeue();
}

template <typename T>
void SimpleQueue<T>::enqueue(const T& val) {
    Node* newNode = new Node(val);
    if (!backPtr) {
        frontPtr = backPtr = newNode;
    } else {
        backPtr->next = newNode;
        backPtr = newNode;
    }
    ++count;
}

template <typename T>
void SimpleQueue<T>::dequeue() {
    if (!frontPtr) return;
    Node* temp = frontPtr;
    frontPtr = frontPtr->next;
    if (!frontPtr) backPtr = nullptr;
    delete temp;
    --count;
}

template <typename T>
T& SimpleQueue<T>::front() {
    if (!frontPtr) throw std::runtime_error("Queue is empty");
    return frontPtr->data;
}

template <typename T>
const T& SimpleQueue<T>::front() const {
    if (!frontPtr) throw std::runtime_error("Queue is empty");
    return frontPtr->data;
}

template <typename T>
bool SimpleQueue<T>::isEmpty() const {
    return frontPtr == nullptr;
}

template <typename T>
int SimpleQueue<T>::size() const {
    return count;
}

template <typename T>
void SimpleQueue<T>::clear() {
    while (!isEmpty()) dequeue();
}

template <typename T>
const typename SimpleQueue<T>::Node* SimpleQueue<T>::getFrontNode() const {
    return frontPtr;
}

template class SimpleQueue<std::pair<std::string, std::pair<std::string, int>>>;
