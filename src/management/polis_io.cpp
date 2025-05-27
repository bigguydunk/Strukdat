#include "../../include/management/polis_io.hpp"
#include <fstream>
#include <sstream>

void loadPolisFromCSV(const std::string& filename, std::vector<Polis>& daftarPolis) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    std::string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        std::stringstream ss(line);
        Polis p;
        std::string umurStr, risikoStr, totalKlaimStr;
        getline(ss, p.nomorPolis, ',');
        getline(ss, p.nama, ',');
        getline(ss, umurStr, ',');
        getline(ss, risikoStr, ',');
        getline(ss, totalKlaimStr, ',');
        p.umur = std::stoi(umurStr);
        p.risiko = std::stoi(risikoStr);
        p.totalKlaim = std::stoi(totalKlaimStr);
        daftarPolis.push_back(p);
    }
}

void savePolisToCSV(const std::string& filename, const std::vector<Polis>& daftarPolis) {
    std::ofstream file(filename);
    file << "nomorPolis,nama,umur,risiko,totalKlaim\n";
    for (const auto& p : daftarPolis) {
        file << p.nomorPolis << ',' << p.nama << ',' << p.umur << ',' << p.risiko << ',' << p.totalKlaim << "\n";
    }
}

void Asuransi::saveKlaimToCSV(const std::string& filename) const {
    std::ofstream file(filename);
    file << "nomorPolis,namaKlaim,jumlahKlaim\n";
    for (const auto& polis : daftarPolis) {
        for (const auto& klaim : polis.klaim) {
            file << polis.nomorPolis << ',' << klaim.first << ',' << klaim.second << "\n";
        }
    }
}

void Asuransi::loadKlaimFromCSV(const std::string& filename) {
    // Clear all klaim in memory
    for (auto& polis : daftarPolis) {
        polis.klaim.clear();
    }
    std::ifstream file(filename);
    if (!file.is_open()) return;
    std::string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string nomorPolis, namaKlaim, jumlahKlaimStr;
        getline(ss, nomorPolis, ',');
        getline(ss, namaKlaim, ',');
        getline(ss, jumlahKlaimStr, ',');
        int jumlahKlaim = std::stoi(jumlahKlaimStr);
        for (auto& polis : daftarPolis) {
            if (polis.nomorPolis == nomorPolis) {
                polis.klaim.push_back(std::make_pair(namaKlaim, jumlahKlaim));
                break;
            }
        }
    }
}
