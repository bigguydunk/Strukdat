#include "../../include/polis/polis.hpp"
#include "../../include/strukdat/xorcipher.hpp"
#include "../../include/strukdat/encoding.hpp"
#include "../../include/strukdat/polis_list_sort.hpp"
#include "../../include/strukdat/strukdat_queue.hpp"
#include "../../include/strukdat/strukdat_stack.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <vector>

using namespace std;
const string SECRET_KEY = "adamsepuh";

void Asuransi::loadFromCSV(const string& filename) {
    daftarPolis.clear();
    nomorPolisMap.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] File tidak dapat dibuka: " << filename << endl;
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
    UmurSort(daftarPolis);
    cout << "Polis berhasil diurutkan berdasarkan umur (termuda ke tertua)." << endl;
}

void Asuransi::urutkanPolisByRisiko() {
    simpanStateUndo();
    RisikoSort(daftarPolis);
    cout << "Polis berhasil diurutkan berdasarkan risiko (terendah ke tertinggi)." << endl;
}

void Asuransi::tambahPolis(const string& nama, int umur, int risiko) {
    simpanStateUndo();
    if (nama.empty() || umur <= 0 || risiko < 0) {
        cout << "Input tidak sesuai format. Nama tidak boleh kosong, umur > 0, risiko >= 0." << endl;
        return;
    }
    string asciiPart;
    for (int i = 0; i < 3 && i < nama.size(); ++i) {
        char upperChar = toupper(nama[i]);
        asciiPart += to_string(static_cast<int>(upperChar));
    }
    string nomorPolis = asciiPart + to_string(umur) + to_string(risiko);
    // agar nomor polis tidak duplikat
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
    PolisList::Node* last = daftarPolis.begin();
    while (last && last->next) last = last->next;
    if (last) nomorPolisMap[uniqueNomorPolis] = &last->data;
    cout << "Polis berhasil ditambahkan dengan Nomor Polis: " << uniqueNomorPolis << endl;
}

void Asuransi::tambahKlaim(const string& nomorPolis, const string& namaKlaim, int jumlahKlaim) {
    simpanStateUndo();
    if (nomorPolis.empty() || namaKlaim.empty() || jumlahKlaim <= 0) {
        cout << "Input tidak sesuai format. Nomor polis/nama klaim tidak boleh kosong, jumlah klaim > 0." << endl;
        return;
    }
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
    if (antrianKlaim.isEmpty()) {
        cout << "Tidak ada klaim dalam antrian.\n";
        return;
    }
    while (true) {
        cout << "\n=== Proses Klaim Kesehatan ===\n";
        cout << "1. Terima satu klaim\n";
        cout << "2. Terima semua klaim\n";
        cout << "3. Hapus satu klaim\n";
        cout << "4. Hapus semua klaim\n";
        cout << "5. Kembali ke menu utama\n";
        cout << "Pilih opsi: ";
        int opsi;
        cin >> opsi;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Input tidak sesuai format. Silakan masukkan angka." << endl;
            continue;
        }
        if (opsi == 2) { // Accept all
            simpanStateUndo();
            cout << "Memproses semua klaim kesehatan:\n";
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
            cout << "Semua klaim telah diproses.\n";
            break;
        } else if (opsi == 4) { // Delete all
            simpanStateUndo();
            while (!antrianKlaim.isEmpty()) {
                antrianKlaim.dequeue();
            }
            cout << "Semua klaim telah dihapus dari antrian.\n";
            break;
        } else if (opsi == 1 || opsi == 3) { 
            vector<pair<string, pair<string, int>>> daftarKlaim;
            auto node = antrianKlaim.getFrontNode();
            int idx = 1;
            cout << "Daftar Klaim dalam Antrian:\n";
            while (node != nullptr) {
                const auto& klaim = node->data;
                cout << idx << ". Nomor Polis: " << klaim.first
                     << " | Nama Klaim: " << klaim.second.first
                     << " | Klaim: Rp" << klaim.second.second << endl;
                daftarKlaim.push_back(klaim);
                node = node->next;
                ++idx;
            }
            if (daftarKlaim.empty()) {
                cout << "Tidak ada klaim dalam antrian.\n";
                break;
            }
            cout << "Pilih nomor klaim yang ingin diproses: ";
            int pilih;
            cin >> pilih;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Input tidak sesuai format. Silakan masukkan angka." << endl;
                continue;
            }
            if (pilih < 1 || pilih > (int)daftarKlaim.size()) {
                cout << "Pilihan tidak valid.\n";
                continue;
            }
            simpanStateUndo();

            StrukdatQueue<std::pair<std::string, std::pair<std::string, int>>> tempQueue;
            node = antrianKlaim.getFrontNode();
            idx = 1;
            while (node != nullptr) {
                if (idx == pilih) {
                    if (opsi == 1) { 
                        auto& klaim = node->data;
                        auto it = nomorPolisMap.find(klaim.first);
                        if (it != nomorPolisMap.end()) {
                            Polis* polis = it->second;
                            polis->totalKlaim += klaim.second.second;
                            polis->addKlaim(klaim.second.first, klaim.second.second);
                        }
                        cout << "Klaim diterima untuk Nomor Polis: " << klaim.first
                             << " | Nama Klaim: " << klaim.second.first
                             << " | Klaim: Rp" << klaim.second.second << endl;
                    } else if (opsi == 3) { 
                        auto& klaim = node->data;
                        cout << "Klaim dihapus untuk Nomor Polis: " << klaim.first
                             << " | Nama Klaim: " << klaim.second.first
                             << " | Klaim: Rp" << klaim.second.second << endl;
                    }
                } else {
                    tempQueue.enqueue(node->data);
                }
                node = node->next;
                ++idx;
            }
            antrianKlaim = tempQueue;
            cout << "Apakah ingin memproses klaim lain? (y/n): ";
            char lanjut;
            cin >> lanjut;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Input tidak sesuai format. Silakan masukkan y/n." << endl;
                continue;
            }
            if (lanjut != 'y' && lanjut != 'Y' && lanjut != 'n' && lanjut != 'N') {
                cout << "Pilihan tidak valid. Silakan masukkan y/n." << endl;
                continue;
            }
            if (lanjut == 'n' || lanjut == 'N') break;
        } else if (opsi == 5) {
            break;
        } else {
            cout << "Pilihan tidak valid.\n";
        }
    }
}

void Asuransi::tampilkanPolis() const {
    const int wPolis = 15, wNama = 20, wUmur = 10, wRisiko = 10, wKlaim = 20; // setara lebar
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
        // baris pertama
        cout << left << setw(wPolis) << polis.nomorPolis
             << left << setw(wNama) << "| " + polis.nama
             << left << setw(wUmur) << "| " + to_string(polis.umur)
             << left << setw(wRisiko) << "| " + to_string(polis.risiko)
             << left << setw(wKlaim) << "| Rp" + to_string(polis.totalKlaim)
             << "| ";
        if (k) {
            cout << "- " << k->namaKlaim << " (Rp" << k->jumlahKlaim << ")" << endl;
            k = k->next;
            while (k) {
                cout << setw(wPolis) << " " << setw(wNama) << "|" << setw(wUmur) << "|" << setw(wRisiko) << "|" << setw(wKlaim) << "|" << "| "
                     << "- " << k->namaKlaim << " (Rp" << k->jumlahKlaim << ")" << endl;
                k = k->next;
            }
        } else {
            cout << "Tidak ada klaim" << endl;
        }
        cout << string(100, '-') << endl;
    }
}

void Asuransi::tampilkanKlaim() const {
    if (antrianKlaim.isEmpty()) {
        std::cout << "Tidak ada klaim dalam antrian." << std::endl;
    } else {
        std::cout << "Daftar Klaim Kesehatan dalam antrian:" << std::endl;
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
    undoStack.push(AsuransiState(daftarPolis, antrianKlaim));
    while (!redoStack.isEmpty()) {
        redoStack.pop();
    }
}

void Asuransi::rebuildNomorPolisMap() {
    nomorPolisMap.clear();
    for (PolisList::Node* node = daftarPolis.begin(); node; node = node->next) {
        nomorPolisMap[node->data.nomorPolis] = &node->data;
    }
}

void Asuransi::undo() {
    if (undoStack.isEmpty()) {
        std::cout << "Tidak ada data untuk di-Undo.\n";
        return;
    }
    redoStack.push(AsuransiState(daftarPolis, antrianKlaim));
    AsuransiState prev = undoStack.top();
    daftarPolis = prev.daftarPolis;
    antrianKlaim = prev.antrianKlaim;
    undoStack.pop();
    rebuildNomorPolisMap();
    std::cout << "Undo berhasil.\n";
}

void Asuransi::redo() {
    if (redoStack.isEmpty()) {
        std::cout << "Tidak ada data untuk di-Redo.\n";
        return;
    }
    undoStack.push(AsuransiState(daftarPolis, antrianKlaim));
    AsuransiState next = redoStack.top();
    daftarPolis = next.daftarPolis;
    antrianKlaim = next.antrianKlaim;
    redoStack.pop();
    rebuildNomorPolisMap();
    std::cout << "Redo berhasil.\n";
}
