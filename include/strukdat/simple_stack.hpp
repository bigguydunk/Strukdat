#pragma once
#include <stdexcept>
#include <vector>
#include <string>

// Forward declaration for Polis
struct Polis;

template <typename T>
class SimpleStack {
    struct Node {
        T data;
        Node* next;
        Node(const T& val);
    };
    Node* topPtr;
    int count;
public:
    SimpleStack();
    ~SimpleStack();
    void push(const T& val);
    void pop();
    T& top();
    const T& top() const;
    bool isEmpty() const;
    int size() const;
    void clear();
};
