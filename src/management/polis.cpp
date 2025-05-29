#include "../../include/management/polis.hpp"
#include "../../include/management/xorcipher.hpp"
#include "../../include/management/encoding.hpp"
#include "../../include/strukdat/simple_queue.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <fstream>

using namespace std;
const string SECRET_KEY = "adamsepuh";

void Asuransi::loadFromCSV(const string& filename) {
    daftarPolis.clear();
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
        } catch (const std::exception& e) {
            continue;
        }
    }
}

void Asuransi::saveToCSV(const string& filename) const {
    ofstream file(filename);
    file << "nomorPolis|nama|umur|risiko|totalKlaim\n";
    for (const auto& p : daftarPolis) {
        string namaTerenkripsi = xorcipher(p.nama, SECRET_KEY);
        string namaHexTerenkripsi = to_hex(namaTerenkripsi);
        file << p.nomorPolis << '|' << namaHexTerenkripsi << '|' << p.umur << '|' << p.risiko << '|' << p.totalKlaim << "\n";
    }
}

void Asuransi::urutkanPolisByUmur() {
    simpanStateUndo();
    sort(daftarPolis.begin(), daftarPolis.end(), [](const Polis& a, const Polis& b) {
        return a.umur < b.umur;
    });
    cout << "Polis berhasil diurutkan berdasarkan umur (termuda ke tertua)." << endl;
}

void Asuransi::urutkanPolisByRisiko() {
    simpanStateUndo();
    sort(daftarPolis.begin(), daftarPolis.end(), [](const Polis& a, const Polis& b) {
        return a.risiko < b.risiko;
    });
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
    Polis polis = {nomorPolis, nama, umur, risiko, 0, {}};
    daftarPolis.push_back(polis);
    cout << "Polis berhasil ditambahkan dengan Nomor Polis: " << nomorPolis << endl;
}

void Asuransi::tambahKlaim(const string& nomorPolis, const string& namaKlaim, int jumlahKlaim) {
    simpanStateUndo();
    bool found = false;
    for (const auto& polis : daftarPolis) {
        if (polis.nomorPolis == nomorPolis) {
            found = true;
            break;
        }
    }
    if (found) {
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
        for (auto& polis : daftarPolis) {
            if (polis.nomorPolis == klaim.first) {
                polis.totalKlaim += klaim.second.second;
                polis.klaim.push_back(make_pair(klaim.second.first, klaim.second.second));
                break;
            }
        }
        cout << "Memproses klaim untuk Nomor Polis: " << klaim.first 
             << " | Nama Klaim: " << klaim.second.first 
             << " | Klaim: Rp" << klaim.second.second << endl;
        klaimDiproses.push_back(klaim);
        antrianKlaim.dequeue();
    }
    cout << "Semua klaim telah diproses." << endl;
}

void Asuransi::tampilkanPolis() const {
    cout << left << setw(15) << "Nomor Polis" 
         << left << setw(20) << "| Nama" 
         << left << setw(10) << "| Umur" 
         << left << setw(10) << "| Risiko" 
         << left << setw(20) << "| Total Klaim" 
         << left << "| Klaim Detail" << endl;
    cout << "----------------------------------------------------------------------------------------------------" << endl;
    for (const auto& polis : daftarPolis) {
        cout << left << setw(15) << polis.nomorPolis 
             << left << "| " << setw(18) << polis.nama 
             << left << "| " << setw(8) << polis.umur 
             << left << "| " << setw(8) << polis.risiko 
             << left << "| Rp" << setw(16) << polis.totalKlaim;
        if (!polis.klaim.empty()) {
            cout << " | ";
            for (size_t i = 0; i < polis.klaim.size(); ++i) {
                cout << polis.klaim[i].first << " (Rp" << polis.klaim[i].second << ")";
                if (i < polis.klaim.size() - 1) {
                    cout << ", ";
                }
            }
        } else {
            cout << " | Tidak ada klaim";
        }
        cout << endl;
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
