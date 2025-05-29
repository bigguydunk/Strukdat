#pragma once
#include <stdexcept>
#include <string>

template <typename T>
class SimpleQueue {
    struct Node {
        T data;
        Node* next;
        Node(const T& val);
    };
    Node* frontPtr;
    Node* backPtr;
    int count;
public:
    SimpleQueue();
    SimpleQueue(const SimpleQueue& other);
    ~SimpleQueue();
    SimpleQueue& operator=(const SimpleQueue& other);
    void enqueue(const T& val);
    void dequeue();
    T& front();
    const T& front() const;
    bool isEmpty() const;
    int size() const;
    void clear();
    const Node* getFrontNode() const;
};
