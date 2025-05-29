#include "../include/strukdat/polis_list.hpp"
#include <cstring>

Polis::Polis() : nomorPolis(), nama(), umur(0), risiko(0), totalKlaim(0), klaimHead(nullptr), klaimCount(0) {}

Polis::Polis(const Polis& other)
    : nomorPolis(other.nomorPolis), nama(other.nama), umur(other.umur), risiko(other.risiko), totalKlaim(other.totalKlaim), klaimHead(nullptr), klaimCount(0) {
    PolisKlaim* curr = other.klaimHead;
    while (curr) {
        addKlaim(curr->namaKlaim, curr->jumlahKlaim);
        curr = curr->next;
    }
}

Polis& Polis::operator=(const Polis& other) {
    if (this != &other) {
        nomorPolis = other.nomorPolis;
        nama = other.nama;
        umur = other.umur;
        risiko = other.risiko;
        totalKlaim = other.totalKlaim;
        clearKlaim();
        PolisKlaim* curr = other.klaimHead;
        while (curr) {
            addKlaim(curr->namaKlaim, curr->jumlahKlaim);
            curr = curr->next;
        }
    }
    return *this;
}

Polis::~Polis() { clearKlaim(); }

void Polis::addKlaim(const std::string& namaKlaim, int jumlahKlaim) {
    PolisKlaim* newKlaim = new PolisKlaim(namaKlaim, jumlahKlaim);
    if (!klaimHead) {
        klaimHead = newKlaim;
    } else {
        PolisKlaim* curr = klaimHead;
        while (curr->next) curr = curr->next;
        curr->next = newKlaim;
    }
    ++klaimCount;
}

void Polis::clearKlaim() {
    while (klaimHead) {
        PolisKlaim* temp = klaimHead;
        klaimHead = klaimHead->next;
        delete temp;
    }
    klaimCount = 0;
}

PolisList::PolisList() : head(nullptr), count(0) {}

PolisList::PolisList(const PolisList& other) : head(nullptr), count(0) {
    Node* curr = other.head;
    while (curr) {
        push_back(curr->data);
        curr = curr->next;
    }
}

PolisList& PolisList::operator=(const PolisList& other) {
    if (this != &other) {
        clear();
        Node* curr = other.head;
        while (curr) {
            push_back(curr->data);
            curr = curr->next;
        }
    }
    return *this;
}

PolisList::~PolisList() { clear(); }

void PolisList::push_back(const Polis& p) {
    Node* newNode = new Node(p);
    if (!head) {
        head = newNode;
    } else {
        Node* curr = head;
        while (curr->next) curr = curr->next;
        curr->next = newNode;
    }
    ++count;
}

void PolisList::clear() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    count = 0;
}

int PolisList::size() const { return count; }

bool PolisList::empty() const { return head == nullptr; }

PolisList::Node* PolisList::begin() const { return head; }
