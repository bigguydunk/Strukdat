#pragma once
#include <string>
#include "../strukdat/polis_list.hpp"
#include "../strukdat/simple_queue.hpp"
#include "../strukdat/simple_stack.hpp"

class Asuransi {
private:
    PolisList daftarPolis;
    SimpleQueue<std::pair<std::string, std::pair<std::string, int>>> antrianKlaim;
    PolisList klaimDiproses;
    SimpleStack<PolisList> undoStack;
    SimpleStack<PolisList> redoStack;
public:
    void tambahPolis(const std::string& nama, int umur, int risiko);
    void tambahKlaim(const std::string& nomorPolis, const std::string& namaKlaim, int jumlahKlaim);
    void tampilkanKlaim() const;
    void prosesKlaim();
    void tampilkanPolis() const;
    void urutkanPolisByUmur();
    void urutkanPolisByRisiko();
    void loadFromCSV(const std::string& filename);
    void saveToCSV(const std::string& filename) const;
    void loadKlaimFromCSV(const std::string& filename);
    void saveKlaimToCSV(const std::string& filename) const;
    void undo();
    void redo();
    void simpanStateUndo();
    PolisList& getDaftarPolis() { return daftarPolis; }
    const PolisList& getDaftarPolis() const { return daftarPolis; }
};