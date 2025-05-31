#pragma once
#include <stdexcept>
#include "polis_list.hpp"

// Forward declaration for Polis
struct Polis;

template <typename T>
class StrukdatStack {
    struct Node {
        T data;
        Node* next;
        Node(const T& val);
    };
    Node* topPtr;
    int count;
public:
    StrukdatStack();
    ~StrukdatStack();
    void push(const T& val);
    void pop();
    T& top();
    const T& top() const;
    bool isEmpty() const;
    int size() const;
    void clear();
};
