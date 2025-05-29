#include "../../include/management/polis.hpp"
#include "../../include/management/xorcipher.hpp"
#include "../../include/management/encoding.hpp"
#include "../../include/strukdat/simple_queue.hpp"
#include "../../include/strukdat/polis_list_sort.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <unordered_map>

using namespace std;
const string SECRET_KEY = "adamsepuh";

void Asuransi::loadFromCSV(const string& filename) {
    daftarPolis.clear();
    nomorPolisMap.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        Polis p;
        string namaHexTerenkripsi;
        string umurStr, risikoStr, totalKlaimStr;

        getline(ss, p.nomorPolis, '|');
        getline(ss, namaHexTerenkripsi, '|');
        getline(ss, umurStr, '|');
        getline(ss, risikoStr, '|');
        getline(ss, totalKlaimStr, '|');

        if (namaHexTerenkripsi.empty() || umurStr.empty() || risikoStr.empty() || totalKlaimStr.empty() || p.nomorPolis.empty()) {
            continue;
        }
        
        string namaTerenkripsi = from_hex(namaHexTerenkripsi);
        p.nama = xorcipher(namaTerenkripsi, SECRET_KEY);
        
        try {
            p.umur = stoi(umurStr);
            p.risiko = stoi(risikoStr);
            p.totalKlaim = stoi(totalKlaimStr);
            daftarPolis.push_back(p);
            // Traverse to last node to get pointer
            PolisList::Node* last = daftarPolis.begin();
            while (last && last->next) last = last->next;
            if (last) nomorPolisMap[p.nomorPolis] = &last->data;
        } catch (const std::exception& e) {
            continue;
        }
    }
}

void Asuransi::saveToCSV(const string& filename) const {
    ofstream file(filename);
    file << "nomorPolis|nama|umur|risiko|totalKlaim\n";
    for (PolisList::Node* node = daftarPolis.begin(); node; node = node->next) {
        const Polis& p = node->data;
        string namaTerenkripsi = xorcipher(p.nama, SECRET_KEY);
        string namaHexTerenkripsi = to_hex(namaTerenkripsi);
        file << p.nomorPolis << '|' << namaHexTerenkripsi << '|' << p.umur << '|' << p.risiko << '|' << p.totalKlaim << "\n";
    }
}

void Asuransi::urutkanPolisByUmur() {
    simpanStateUndo();
    sortPolisListByUmur(daftarPolis);
    cout << "Polis berhasil diurutkan berdasarkan umur (termuda ke tertua)." << endl;
}

void Asuransi::urutkanPolisByRisiko() {
    simpanStateUndo();
    sortPolisListByRisiko(daftarPolis);
    cout << "Polis berhasil diurutkan berdasarkan risiko (terendah ke tertinggi)." << endl;
}

void Asuransi::tambahPolis(const string& nama, int umur, int risiko) {
    simpanStateUndo();
    string asciiPart;
    for (int i = 0; i < 3 && i < nama.size(); ++i) {
        char upperChar = toupper(nama[i]);
        asciiPart += to_string(static_cast<int>(upperChar));
    }
    string nomorPolis = asciiPart + to_string(umur) + to_string(risiko);
    // Ensure nomorPolis is unique by incrementing if needed
    string uniqueNomorPolis = nomorPolis;
    while (nomorPolisMap.find(uniqueNomorPolis) != nomorPolisMap.end()) {
        long long num = stoll(uniqueNomorPolis);
        ++num;
        uniqueNomorPolis = to_string(num);
    }
    Polis polis;
    polis.nomorPolis = uniqueNomorPolis;
    polis.nama = nama;
    polis.umur = umur;
    polis.risiko = risiko;
    polis.totalKlaim = 0;
    polis.klaimHead = nullptr;
    polis.klaimCount = 0;
    daftarPolis.push_back(polis);
    // Traverse to last node to get pointer
    PolisList::Node* last = daftarPolis.begin();
    while (last && last->next) last = last->next;
    if (last) nomorPolisMap[uniqueNomorPolis] = &last->data;
    cout << "Polis berhasil ditambahkan dengan Nomor Polis: " << uniqueNomorPolis << endl;
}

void Asuransi::tambahKlaim(const string& nomorPolis, const string& namaKlaim, int jumlahKlaim) {
    simpanStateUndo();
    auto it = nomorPolisMap.find(nomorPolis);
    if (it != nomorPolisMap.end()) {
        antrianKlaim.enqueue(make_pair(nomorPolis, make_pair(namaKlaim, jumlahKlaim)));
        cout << "Klaim sebesar Rp" << jumlahKlaim << " untuk \"" << namaKlaim 
             << "\" berhasil ditambahkan ke antrian untuk Nomor Polis: " << nomorPolis << endl;
    } else {
        cout << "Nomor Polis tidak ditemukan. Klaim gagal ditambahkan." << endl;
    }
}

void Asuransi::prosesKlaim() {
    cout << "Memproses klaim kesehatan:" << endl;
    while (!antrianKlaim.isEmpty()) {
        auto klaim = antrianKlaim.front();
        auto it = nomorPolisMap.find(klaim.first);
        if (it != nomorPolisMap.end()) {
            Polis* polis = it->second;
            polis->totalKlaim += klaim.second.second;
            polis->addKlaim(klaim.second.first, klaim.second.second);
        }
        cout << "Memproses klaim untuk Nomor Polis: " << klaim.first 
             << " | Nama Klaim: " << klaim.second.first 
             << " | Klaim: Rp" << klaim.second.second << endl;
        antrianKlaim.dequeue();
    }
    cout << "Semua klaim telah diproses." << endl;
}

void Asuransi::tampilkanPolis() const {
    // Column widths
    const int wPolis = 15, wNama = 20, wUmur = 10, wRisiko = 10, wKlaim = 20;
    cout << left << setw(wPolis) << "Nomor Polis"
         << left << setw(wNama) << "| Nama"
         << left << setw(wUmur) << "| Umur"
         << left << setw(wRisiko) << "| Risiko"
         << left << setw(wKlaim) << "| Total Klaim"
         << left << "| Klaim Detail" << endl;
    cout << string(100, '-') << endl;
    for (PolisList::Node* node = daftarPolis.begin(); node; node = node->next) {
        const Polis& polis = node->data;
        PolisKlaim* k = polis.klaimHead;
        // Print first line (with first claim or 'Tidak ada klaim')
        cout << left << setw(wPolis) << polis.nomorPolis
             << left << setw(wNama) << "| " + polis.nama
             << left << setw(wUmur) << "| " + to_string(polis.umur)
             << left << setw(wRisiko) << "| " + to_string(polis.risiko)
             << left << setw(wKlaim) << "| Rp" + to_string(polis.totalKlaim)
             << "| ";
        if (k) {
            cout << "- " << k->namaKlaim << " (Rp" << k->jumlahKlaim << ")" << endl;
            k = k->next;
            // Print remaining claims, one per line, with columns padded
            while (k) {
                cout << setw(wPolis) << " " << setw(wNama) << "|" << setw(wUmur) << "|" << setw(wRisiko) << "|" << setw(wKlaim) << "|" << "| "
                     << "- " << k->namaKlaim << " (Rp" << k->jumlahKlaim << ")" << endl;
                k = k->next;
            }
        } else {
            cout << "Tidak ada klaim" << endl;
        }
        // Print separator line after each polis
        cout << string(100, '-') << endl;
    }
}

void Asuransi::tampilkanKlaim() const {
    if (antrianKlaim.isEmpty()) {
        std::cout << "Tidak ada klaim dalam antrian." << std::endl;
    } else {
        std::cout << "Daftar Klaim Kesehatan dalam antrian:" << std::endl;
        // Use the new getFrontNode() to iterate without modifying the queue
        auto node = antrianKlaim.getFrontNode();
        while (node != nullptr) {
            const auto& klaim = node->data;
            std::cout << "Nomor Polis: " << klaim.first
                      << " | Nama Klaim: " << klaim.second.first
                      << " | Klaim: Rp" << klaim.second.second << std::endl;
            node = node->next;
        }
    }
}

void Asuransi::simpanStateUndo() {
    undoStack.push(daftarPolis);
    while (!redoStack.isEmpty()) {
        redoStack.pop();
    }
}

void Asuransi::undo() {
    if (undoStack.isEmpty()) {
        std::cout << "Tidak ada data untuk di-Undo.\n";
        return;
    }
    redoStack.push(daftarPolis);
    daftarPolis = undoStack.top();
    undoStack.pop();
    std::cout << "Undo berhasil.\n";
}

void Asuransi::redo() {
    if (redoStack.isEmpty()) {
        std::cout << "Tidak ada data untuk di-Redo.\n";
        return;
    }
    undoStack.push(daftarPolis);
    daftarPolis = redoStack.top();
    redoStack.pop();
    std::cout << "Redo berhasil.\n";
}
