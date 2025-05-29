#include "../../include/management/polis_io.hpp"
#include "../../include/management/xorcipher.hpp"
#include "../../include/management/encoding.hpp"
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
    for (const auto& polis : daftarPolis) {
        for (const auto& klaim : polis.klaim) {
            std::string namaKlaimTerenkripsi = xorcipher(klaim.first, secretkey);
            std::string namaKlaimHex = to_hex(namaKlaimTerenkripsi);
            file << polis.nomorPolis << '|' << namaKlaimHex << '|' << klaim.second << "\n";
        }
    }
}

void Asuransi::loadKlaimFromCSV(const std::string& filename) {
    for (auto& polis : daftarPolis) {
        polis.klaim.clear();
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
            for (auto& polis : daftarPolis) {
                if (polis.nomorPolis == nomorPolis) {
                    polis.klaim.push_back(std::make_pair(namaKlaimAsli, jumlahKlaim));
                    break;
                }
            }
        } catch (const std::invalid_argument& ia) {
        }
    }
}
