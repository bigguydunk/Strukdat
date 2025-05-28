#include "../../include/management/polis.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <fstream>

using namespace std;

// Helper to load polis from CSV (with clearing)
void Asuransi::loadFromCSV(const string& filename) {
    daftarPolis.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[DEBUG] Gagal membuka file: " << filename << endl;
        return;
    }
    cout << "[DEBUG] Berhasil membuka file: " << filename << endl;
    string line;
    getline(file, line); // skip header
    int count = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        Polis p;
        string umurStr, risikoStr, totalKlaimStr;
        getline(ss, p.nomorPolis, ',');
        getline(ss, p.nama, ',');
        getline(ss, umurStr, ',');
        getline(ss, risikoStr, ',');
        getline(ss, totalKlaimStr, ',');
        p.umur = stoi(umurStr);
        p.risiko = stoi(risikoStr);
        p.totalKlaim = stoi(totalKlaimStr);
        daftarPolis.push_back(p);
        count++;
    }
    cout << "[DEBUG] Jumlah polis yang dimuat: " << count << endl;
}

void Asuransi::saveToCSV(const string& filename) const {
    ofstream file(filename);
    file << "nomorPolis,nama,umur,risiko,totalKlaim\n";
    for (const auto& p : daftarPolis) {
        file << p.nomorPolis << ',' << p.nama << ',' << p.umur << ',' << p.risiko << ',' << p.totalKlaim << "\n";
    }
}

void Asuransi::tambahPolis(const string& nama, int umur, int risiko) {
    simpanStateUndo(); 
    string asciiPart;
    for (int i = 0; i < 3 && i < nama.size(); ++i) {
        char upperChar = toupper(nama[i]);
        asciiPart += to_string(upperChar);
    }
    string nomorPolis = asciiPart + to_string(umur) + to_string(risiko);
    Polis polis = {nomorPolis, nama, umur, risiko, 0, {}};
    daftarPolis.push_back(polis);
    cout << "Polis berhasil ditambahkan dengan Nomor Polis: " << nomorPolis << endl;
}

void Asuransi::tambahKlaim(const string& nomorPolis, const string& namaKlaim, int jumlahKlaim) {
    simpanStateUndo();
    bool found = false;
    for (auto& polis : daftarPolis) {
        if (polis.nomorPolis == nomorPolis) {
            polis.totalKlaim += jumlahKlaim;
            polis.klaim.push_back(make_pair(namaKlaim, jumlahKlaim));
            found = true;
            break;
        }
    }
    if (found) {
        antrianKlaim.push(make_pair(nomorPolis, make_pair(namaKlaim, jumlahKlaim)));
        cout << "Klaim sebesar Rp" << jumlahKlaim << " untuk \"" << namaKlaim 
             << "\" berhasil ditambahkan ke antrian untuk Nomor Polis: " << nomorPolis << endl;
    } else {
        cout << "Nomor Polis tidak ditemukan. Klaim gagal ditambahkan." << endl;
    }
}

void Asuransi::tampilkanKlaim() const {
    if (antrianKlaim.empty()) {
        cout << "Tidak ada klaim dalam antrian." << endl;
    } else {
        cout << "Daftar Klaim Kesehatan dalam antrian:" << endl;
        queue<pair<string, pair<string, int>>> tempQueue = antrianKlaim;
        while (!tempQueue.empty()) {
            auto klaim = tempQueue.front();
            cout << "Nomor Polis: " << klaim.first 
                 << " | Nama Klaim: " << klaim.second.first 
                 << " | Klaim: Rp" << klaim.second.second << endl;
            tempQueue.pop();
        }
    }
}

void Asuransi::prosesKlaim() {
    cout << "Memproses klaim kesehatan:" << endl;
    while (!antrianKlaim.empty()) {
        auto klaim = antrianKlaim.front();
        cout << "Memproses klaim untuk Nomor Polis: " << klaim.first 
             << " | Nama Klaim: " << klaim.second.first 
             << " | Klaim: Rp" << klaim.second.second << endl;
        klaimDiproses.push_back(klaim);
        antrianKlaim.pop();
    }
}

void Asuransi::tampilkanPolis() const {
    cout << left << setw(12) << "Nomor Polis" 
         << left << setw(14) << "| Nama" 
         << left << setw(12) << "| Umur" 
         << left << setw(12) << "| Risiko" 
         << left << setw(15) << "| Total Klaim" 
         << left << setw(12) << "| Klaim Detail" << endl;
    cout << "------------------------------------------------------------------------------------------" << endl;
    for (const auto& polis : daftarPolis) {
        cout << left << setw(12) << polis.nomorPolis 
             << left << "| " << setw(12) << polis.nama 
             << left << "| " << setw(10) << polis.umur 
             << left << "| " << setw(10) << polis.risiko 
             << left << "| Rp" << setw(10) << polis.totalKlaim;
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
void Asuransi::simpanStateUndo() {
    undoStack.push(daftarPolis);
    while (!redoStack.empty()) redoStack.pop(); 
}

void Asuransi::undo() {
    if (undoStack.empty()) {
        std::cout << "Tidak ada data Undo.\n";
        return;
    }
    redoStack.push(daftarPolis);
    daftarPolis = undoStack.top();
    undoStack.pop();
    std::cout << "Undo berhasil.\n";
}

void Asuransi::redo() {
    if (redoStack.empty()) {
        std::cout << "Tidak ada data Redo.\n";
        return;
    }
    undoStack.push(daftarPolis);
    daftarPolis = redoStack.top();
    redoStack.pop();
    std::cout << "Redo berhasil.\n";
}
