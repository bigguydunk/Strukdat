#include "strukdat/strukdat_queue.hpp"
#include <stdexcept>
#include <string>

// node constructor

template <typename T>
StrukdatQueue<T>::Node::Node(const T& val) : data(val), next(nullptr) {}

template <typename T>
StrukdatQueue<T>::StrukdatQueue() : frontPtr(nullptr), backPtr(nullptr), count(0) {}

template <typename T>
StrukdatQueue<T>::~StrukdatQueue() {
    while (!isEmpty()) dequeue();
}

template <typename T>
void StrukdatQueue<T>::enqueue(const T& val) {
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
void StrukdatQueue<T>::dequeue() {
    if (!frontPtr) return;
    Node* temp = frontPtr;
    frontPtr = frontPtr->next;
    if (!frontPtr) backPtr = nullptr;
    delete temp;
    --count;
}

template <typename T>
T& StrukdatQueue<T>::front() {
    if (!frontPtr) throw std::runtime_error("Queue is empty");
    return frontPtr->data;
}

template <typename T>
const T& StrukdatQueue<T>::front() const {
    if (!frontPtr) throw std::runtime_error("Queue is empty");
    return frontPtr->data;
}

template <typename T>
bool StrukdatQueue<T>::isEmpty() const {
    return frontPtr == nullptr;
}

template <typename T>
int StrukdatQueue<T>::size() const {
    return count;
}

template <typename T>
void StrukdatQueue<T>::clear() {
    while (!isEmpty()) dequeue();
}

template <typename T>
const typename StrukdatQueue<T>::Node* StrukdatQueue<T>::getFrontNode() const {
    return frontPtr;
}

template <typename T>
StrukdatQueue<T>::StrukdatQueue(const StrukdatQueue<T>& other) : frontPtr(nullptr), backPtr(nullptr), count(0) {
    Node* curr = other.frontPtr;
    while (curr) {
        enqueue(curr->data);
        curr = curr->next;
    }
}

template <typename T>
StrukdatQueue<T>& StrukdatQueue<T>::operator=(const StrukdatQueue<T>& other) {
    if (this != &other) {
        clear();
        Node* curr = other.frontPtr;
        while (curr) {
            enqueue(curr->data);
            curr = curr->next;
        }
    }
    return *this;
}

// header

template class StrukdatQueue<std::pair<std::string, std::pair<std::string, int>>>;
