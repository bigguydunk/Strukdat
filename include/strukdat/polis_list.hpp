#pragma once
#include <string>

struct PolisKlaim {
    std::string namaKlaim;
    int jumlahKlaim;
    PolisKlaim* next;
    PolisKlaim(const std::string& n, int j) : namaKlaim(n), jumlahKlaim(j), next(nullptr) {}
};

struct Polis {
    std::string nomorPolis;
    std::string nama;
    int umur;
    int risiko;
    int totalKlaim;
    PolisKlaim* klaimHead;
    int klaimCount;
    Polis();
    Polis(const Polis& other);
    Polis& operator=(const Polis& other);
    ~Polis();
    void addKlaim(const std::string& namaKlaim, int jumlahKlaim);
    void clearKlaim();
};

class PolisList {
public:
    struct Node {
        Polis data;
        Node* next;
        Node(const Polis& p) : data(p), next(nullptr) {}
    };
    Node* head;
    int count;
    PolisList();
    PolisList(const PolisList& other);
    PolisList& operator=(const PolisList& other);
    ~PolisList();
    void push_back(const Polis& p);
    void clear();
    int size() const;
    bool empty() const;
    Node* begin() const;
};
