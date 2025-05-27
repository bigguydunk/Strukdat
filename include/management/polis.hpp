#pragma once
#include <string>
#include <vector>
#include <queue>

struct Polis {
    std::string nomorPolis;
    std::string nama;
    int umur;
    int risiko;
    int totalKlaim;
    std::vector<std::pair<std::string, int>> klaim;
};

class Asuransi {
private:
    std::vector<Polis> daftarPolis;
    std::queue<std::pair<std::string, std::pair<std::string, int>>> antrianKlaim;
    std::vector<std::pair<std::string, std::pair<std::string, int>>> klaimDiproses;
public:
    void tambahPolis(const std::string& nama, int umur, int risiko);
    void tambahKlaim(const std::string& nomorPolis, const std::string& namaKlaim, int jumlahKlaim);
    void tampilkanKlaim() const;
    void prosesKlaim();
    void tampilkanPolis() const;
    void loadFromCSV(const std::string& filename);
    void saveToCSV(const std::string& filename) const;
    void loadKlaimFromCSV(const std::string& filename);
    void saveKlaimToCSV(const std::string& filename) const;
    std::vector<Polis>& getDaftarPolis() { return daftarPolis; }
    const std::vector<Polis>& getDaftarPolis() const { return daftarPolis; }
};
