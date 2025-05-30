#include "../include/management/polis.hpp"
#include "../include/menu.hpp"
#include <iostream>
#include <string>
#include <conio.h>
using namespace std;

int main() {
    cout << R"(
                        :::::::::::.                  
                   .:::::::     .:::::::.             
             .:::::::::              :::::::::        
         ::::::::                         .:::::::.   
      ::::::                                   .::::: 
      :::                                          :::
      :::                                         .::.
      :::                                         ::: 
      ::::                 .::::                  ::: 
       :::                 .::::                  ::. 
       :::                 .::::                 :::  
        :::                .::::                 ::.  
        .::         :::::::::::::::::::         :::   
         :::        :::::::::::::::::::        :::.   
          :::              .::::               ::.    
           :::              ::::              :::     
            :::             ::::            .:::      
            .:::            ::::            :::      
              :::                         .:::      
               :::                       ::::         
                ::::                    :::           
                  :::.                ::::            
                   ::::             ::::              
                     :::::        ::::.               
                       ::::.    ::::         
                         :::::::::           
                            :::.                      
)";
    cout << "\nSelamat datang di Aplikasi Pengelolaan Asuransi Kesehatan Sederhana!\n\n";
    cout << "Tekan apa saja untuk melanjutkan...";
    _getch();
     cout << "\n\n\n";
    Asuransi asuransi;
    
    std::string dataPath = "data/polis.csv";
    std::string klaimPath = "data/klaim.csv";

    asuransi.loadFromCSV(dataPath);
    asuransi.loadKlaimFromCSV(klaimPath);
    int pilihan;
    string nomorPolis, nama, namaKlaim;
    int umur, risiko, jumlahKlaim;

    do {
        tampilkanMenu();
        cin >> pilihan;
        if(cin.fail()){
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Input tidak valid. Harap masukkan angka." << endl;
            continue;
        }
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
                break;
            case 2:
                asuransi.urutkanPolisByUmur();
                break;
            case 3:
                asuransi.urutkanPolisByRisiko();
                break;
            case 4:
                asuransi.tampilkanPolis();
                break;
            case 5:
                cout << "Masukkan Nomor Polis: ";
                getline(cin, nomorPolis);
                cout << "Masukkan Nama Klaim: ";
                getline(cin, namaKlaim);
                if (namaKlaim.empty()) {
                    cout << "Nama klaim tidak boleh kosong. Menggunakan 'Klaim Umum'." << endl;
                    namaKlaim = "Klaim Umum";
                }
                cout << "Masukkan Jumlah Klaim (dalam Rupiah, tanpa titik atau notasi): ";
                cin >> jumlahKlaim;
                cin.ignore();
                asuransi.tambahKlaim(nomorPolis, namaKlaim, jumlahKlaim);
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
                cout << "Menyimpan data dan keluar dari program." << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
        
        if (pilihan >= 1 && pilihan <= 9 && pilihan != 4 && pilihan != 6) {
             asuransi.saveToCSV(dataPath);
             asuransi.saveKlaimToCSV(klaimPath);
        }

        if (pilihan != 10) {
            if (!konfirmasiLanjut()) break;
        }

    } while (pilihan != 10);
    
    asuransi.saveToCSV(dataPath);
    asuransi.saveKlaimToCSV(klaimPath);
    
    return 0;
}