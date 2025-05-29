#include "../include/strukdat/simple_stack.hpp"
#include <stdexcept>
#include <vector>
#include <string>
#include "../include/management/polis.hpp"

// Node constructor
template <typename T>
SimpleStack<T>::Node::Node(const T& val) : data(val), next(nullptr) {}

template <typename T>
SimpleStack<T>::SimpleStack() : topPtr(nullptr), count(0) {}

template <typename T>
SimpleStack<T>::~SimpleStack() { clear(); }

template <typename T>
void SimpleStack<T>::push(const T& val) {
    Node* newNode = new Node(val);
    newNode->next = topPtr;
    topPtr = newNode;
    ++count;
}

template <typename T>
void SimpleStack<T>::pop() {
    if (!topPtr) throw std::runtime_error("Stack is empty");
    Node* temp = topPtr;
    topPtr = topPtr->next;
    delete temp;
    --count;
}

template <typename T>
T& SimpleStack<T>::top() {
    if (!topPtr) throw std::runtime_error("Stack is empty");
    return topPtr->data;
}

template <typename T>
const T& SimpleStack<T>::top() const {
    if (!topPtr) throw std::runtime_error("Stack is empty");
    return topPtr->data;
}

template <typename T>
bool SimpleStack<T>::isEmpty() const { return topPtr == nullptr; }

template <typename T>
int SimpleStack<T>::size() const { return count; }

template <typename T>
void SimpleStack<T>::clear() {
    while (!isEmpty()) pop();
}

// Explicit instantiation for the types used in your project
template class SimpleStack<std::vector<Polis>>;
