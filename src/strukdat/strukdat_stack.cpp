#include "strukdat/strukdat_stack.hpp"
#include "strukdat/polis_list.hpp"
#include <stdexcept>
#include "polis/polis.hpp"

// node constructor
template <typename T>
StrukdatStack<T>::Node::Node(const T& val) : data(val), next(nullptr) {}

template <typename T>
StrukdatStack<T>::StrukdatStack() : topPtr(nullptr), count(0) {}

template <typename T>
StrukdatStack<T>::~StrukdatStack() { clear(); }

template <typename T>
void StrukdatStack<T>::push(const T& val) {
    Node* newNode = new Node(val);
    newNode->next = topPtr;
    topPtr = newNode;
    ++count;
}

template <typename T>
void StrukdatStack<T>::pop() {
    if (!topPtr) throw std::runtime_error("Stack is empty");
    Node* temp = topPtr;
    topPtr = topPtr->next;
    delete temp;
    --count;
}

template <typename T>
T& StrukdatStack<T>::top() {
    if (!topPtr) throw std::runtime_error("Stack is empty");
    return topPtr->data;
}

template <typename T>
const T& StrukdatStack<T>::top() const {
    if (!topPtr) throw std::runtime_error("Stack is empty");
    return topPtr->data;
}

template <typename T>
bool StrukdatStack<T>::isEmpty() const { return topPtr == nullptr; }

template <typename T>
int StrukdatStack<T>::size() const { return count; }

template <typename T>
void StrukdatStack<T>::clear() {
    while (!isEmpty()) pop();
}

// buat header
template class StrukdatStack<PolisList>;
template class StrukdatStack<AsuransiState>;
