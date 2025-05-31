#include "../include/strukdat/polis_list_sort.hpp"

// ini bubble sort
void UmurSort(PolisList& list) {
    if (!list.head || !list.head->next) return;
    bool swapped;
    do {
        swapped = false;
        PolisList::Node* prev = nullptr;
        PolisList::Node* curr = list.head;
        while (curr && curr->next) {
            PolisList::Node* next = curr->next;
            if (curr->data.umur > next->data.umur) {

                if (prev) prev->next = next;
                else list.head = next;
                curr->next = next->next;
                next->next = curr;
                swapped = true;
                prev = next;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    } while (swapped);
}

void RisikoSort(PolisList& list) {
    if (!list.head || !list.head->next) return;
    bool swapped;
    do {
        swapped = false;
        PolisList::Node* prev = nullptr;
        PolisList::Node* curr = list.head;
        while (curr && curr->next) {
            PolisList::Node* next = curr->next;
            if (curr->data.risiko > next->data.risiko) {
                if (prev) prev->next = next;
                else list.head = next;
                curr->next = next->next;
                next->next = curr;
                swapped = true;
                prev = next;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    } while (swapped);
}
