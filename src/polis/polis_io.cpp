#include "../../include/polis/polis.hpp"
#include "../../include/strukdat/xorcipher.hpp"
#include "../../include/strukdat/encoding.hpp"
#include "strukdat/strukdat_queue.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

const std::string secretkey = "adamsepuh";

void Asuransi::saveKlaimToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "[ERROR] Gagal membuka file untuk menyimpan klaim: " << filename << std::endl;
        return;
    }
    file << "nomorPolis|namaKlaim|jumlahKlaim\n";
    for (PolisList::Node* node = daftarPolis.begin(); node; node = node->next) {
        const Polis& polis = node->data;
        for (PolisKlaim* klaim = polis.klaimHead; klaim; klaim = klaim->next) {
            std::string namaKlaimTerenkripsi = xorcipher(klaim->namaKlaim, secretkey);
            std::string namaKlaimHex = to_hex(namaKlaimTerenkripsi);
            file << polis.nomorPolis << '|' << namaKlaimHex << '|' << klaim->jumlahKlaim << "\n";
        }
    }
}

void Asuransi::loadKlaimFromCSV(const std::string& filename) {
    for (PolisList::Node* node = daftarPolis.begin(); node; node = node->next) {
        Polis& polis = node->data;
        polis.clearKlaim();
    }
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    getline(file, line);
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string nomorPolis, namaKlaimHex, jumlahKlaimStr;
        
        getline(ss, nomorPolis, '|');
        getline(ss, namaKlaimHex, '|');
        getline(ss, jumlahKlaimStr, '|');

        if (nomorPolis.empty() || jumlahKlaimStr.empty()) {
            continue; 
        }
        
        std::string namaKlaimTerenkripsi = from_hex(namaKlaimHex);
        std::string namaKlaimAsli = xorcipher(namaKlaimTerenkripsi, secretkey);
        
        try {
            int jumlahKlaim = std::stoi(jumlahKlaimStr);
            for (PolisList::Node* node = daftarPolis.begin(); node; node = node->next) {
                Polis& polis = node->data;
                if (polis.nomorPolis == nomorPolis) {
                    polis.addKlaim(namaKlaimAsli, jumlahKlaim);
                    break;
                }
            }
        } catch (const std::invalid_argument& ia) {
        }
    }
}
