#include "../include/menu.hpp"
#include <iostream>
using namespace std;

void tampilkanMenu() {
    cout << "Menu:" << endl;
    cout << "1. Tambah Polis" << endl;
    cout << "2. Urutkan Polis Berdasarkan Umur" << endl;
    cout << "3. Urutkan Polis Berdasarkan Risiko" << endl;
    cout << "4. Tampilkan Semua Polis" << endl;
    cout << "5. Tambah Klaim Kesehatan" << endl;
    cout << "6. Tampilkan Klaim Kesehatan" << endl;
    cout << "7. Proses Klaim Kesehatan" << endl;
    cout << "8. Undo" << endl;
    cout << "9. Redo" << endl;
    cout << "10. Keluar" << endl;
    cout << "Pilih opsi: ";
}

bool konfirmasiLanjut() {
    char pilihan;
    cout << "Apakah Anda ingin melanjutkan? (y/n): ";
    cin >> pilihan;
    return pilihan == 'y' || pilihan == 'Y';
}
