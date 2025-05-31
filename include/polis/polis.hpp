#pragma once
#include <string>
#include <unordered_map>
#include "../strukdat/polis_list.hpp"
#include "strukdat/strukdat_queue.hpp"
#include "strukdat/strukdat_stack.hpp"

struct AsuransiState {
    PolisList daftarPolis;
    StrukdatQueue<std::pair<std::string, std::pair<std::string, int>>> antrianKlaim;
    AsuransiState() = default;
    AsuransiState(const PolisList& p, const StrukdatQueue<std::pair<std::string, std::pair<std::string, int>>>& q)
        : daftarPolis(p), antrianKlaim(q) {}
};

class Asuransi {
private:
    PolisList daftarPolis;
    StrukdatQueue<std::pair<std::string, std::pair<std::string, int>>> antrianKlaim;
    PolisList klaimDiproses;
    StrukdatStack<AsuransiState> undoStack;
    StrukdatStack<AsuransiState> redoStack;
    std::unordered_map<std::string, Polis*> nomorPolisMap; // hashmap
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
    void rebuildNomorPolisMap();
    PolisList& getDaftarPolis() { return daftarPolis; }
    const PolisList& getDaftarPolis() const { return daftarPolis; }
};