#pragma once
#include <stdexcept>
#include <string>

template <typename T>
class StrukdatQueue {
    struct Node {
        T data;
        Node* next;
        Node(const T& val);
    };
    Node* frontPtr;
    Node* backPtr;
    int count;
public:
    StrukdatQueue();
    StrukdatQueue(const StrukdatQueue& other);
    ~StrukdatQueue();
    StrukdatQueue& operator=(const StrukdatQueue& other);
    void enqueue(const T& val);
    void dequeue();
    T& front();
    const T& front() const;
    bool isEmpty() const;
    int size() const;
    void clear();
    const Node* getFrontNode() const;
};
