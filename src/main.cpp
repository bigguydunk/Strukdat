#include "../include/management/polis.hpp"
#include "../include/menu.hpp"
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

std::string getExeDir() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string path(buffer);
    size_t pos = path.find_last_of("\\/");
    return (std::string::npos == pos) ? "" : path.substr(0, pos);
}

int main() {
    Asuransi asuransi;
    std::string dataPath = getExeDir() + "\\..\\data\\polis.csv";
    std::string klaimPath = getExeDir() + "\\..\\data\\klaim.csv";
    asuransi.loadFromCSV(dataPath);
    asuransi.loadKlaimFromCSV(klaimPath);
    int pilihan;
    string nomorPolis, nama, namaKlaim;
    int umur, risiko, jumlahKlaim;

    do {
        tampilkanMenu();
        cin >> pilihan;
        cin.ignore();
        switch (pilihan) {
            case 1:
                do {
                    cout << "Masukkan Nama (minimal 3 huruf): ";
                    getline(cin, nama);
                    if (nama.length() < 3) {
                        cout << "Nama harus memiliki minimal 3 huruf. Silahkan coba lagi." << endl;
                    }
                } while (nama.length() < 3);
                cout << "Masukkan Umur: ";
                cin >> umur;
                cin.ignore();
                cout << "Masukkan Risiko (Kecil = 1, Sedang = 2, Tinggi = 3): ";
                cin >> risiko;
                cin.ignore();
                asuransi.tambahPolis(nama, umur, risiko);
                asuransi.saveToCSV(dataPath); // Save after adding polis
                asuransi.saveKlaimToCSV(klaimPath); // Save klaim (empty for new polis)
                break;
            case 2:
                cout << "Fitur urutkan berdasarkan umur belum diimplementasikan." << endl;
                break;
            case 3:
                cout << "Fitur urutkan berdasarkan risiko belum diimplementasikan." << endl;
                break;
            case 4:
                asuransi.tampilkanPolis();
                break;
            case 5:
                cout << "Masukkan Nomor Polis: ";
                getline(cin, nomorPolis);
                cout << "Masukkan Nama Klaim: ";
                getline(cin, namaKlaim);
                cout << "Masukkan Jumlah Klaim (dalam Rupiah, tanpa titik atau notasi): ";
                cin >> jumlahKlaim;
                cin.ignore();
                asuransi.tambahKlaim(nomorPolis, namaKlaim, jumlahKlaim);
                asuransi.saveToCSV(dataPath);
                asuransi.saveKlaimToCSV(klaimPath);
                break;
            case 6:
                asuransi.tampilkanKlaim();
                break;
            case 7:
                asuransi.prosesKlaim();
                break;
            case 8:
                asuransi.undo();
                break;
            case 9:
                asuransi.redo();
                break;
            case 10:
                cout << "Keluar dari program." << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
        if (pilihan != 10 && !konfirmasiLanjut()) {
            break;
        }
    } while (pilihan != 10);
    asuransi.saveToCSV(dataPath);
    asuransi.saveKlaimToCSV(klaimPath);
    return 0;
}