#include "config.h"
#include "utils.h"
#include "database.h"
#include "features.h"
#include <thread>
#include <chrono>

int main() {
    // Intro Mewah
    loadFromFile();
    clearScrean();
    cout << BIRU << "###############################################\n" << RESET;
    ketikLine("##   SYSTEM STARTING... PLEASE WAIT...       ##", KUNING, 30);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    cout << BIRU << "###############################################\n\n" << RESET;
    
    ketik("Loading Database...", 20);
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    cout << HIJAU << " [OK]" << RESET << endl;
    
    ketik("Tekan Enter untuk masuk Dashboard...");
    cin.ignore();

    string inputMenu;
    int menu;

    do {
        clearScrean();
        cout << BIRU << "================================================\n";
        cout << "==  PROGRAM MENGHITUNG NILAI AKHIR MAHASISWA  ==\n";
        cout << "================================================\n" << RESET;

        cout << "1. Input Data Satuan\n";
        cout << "2. " << MERAH << "Keluar Aplikasi" << RESET << "\n";
        cout << "3. Bantuan (Help)\n";
        cout << "4. Lihat Data Tersimpan\n";
        cout << "5. Cari Data Mahasiswa\n";
        cout << "6. Hapus Data Mahasiswa\n";
        cout << "7. Edit Data Mahasiswa\n";
        cout << "8. " << KUNING << "Input Banyak Data (Batch Mode)" << RESET << "\n";
        
        cout << "\n>> Pilih menu: ";
        getline(cin, inputMenu);

        try { menu = stoi(inputMenu); } catch (...) { menu = 0; }

        switch(menu) {
            case 1:
                inputSatuanData(); 
                cout << "\nTekan Enter untuk lanjut...";
                cin.get();
                break;
            case 2:
                clearScrean();
                cout << "\n" << BIRU << "###############################################\n" << RESET;
                ketikLine("##          MENUTUP SISTEM APLIKASI...       ##", KUNING, 20);
                cout << BIRU << "###############################################\n" << RESET;
                
                // Simulasi proses "Saving" agar terlihat keren
                cout << "\n";
                ketik("Memastikan seluruh data tersimpan...", 30);
                
                // Panggil saveToFile() sekali lagi untuk memastikan (safety net)
                saveToFile(); 
                
                std::this_thread::sleep_for(std::chrono::milliseconds(800)); // Delay 0.8 detik
                cout << HIJAU << " [SUKSES]" << RESET << endl;

                // Pesan penutup dengan efek ketik lambat
                ketikLine("\nTerima kasih telah menggunakan program ini.", UNGU, 40);
                ketikLine("Sampai Jumpa!", UNGU, 60);
                
                // Delay terakhir 1.5 detik agar user sempat baca tulisan "Sampai Jumpa"
                // sebelum jendela terminal benar-benar tertutup otomatis
                std::this_thread::sleep_for(std::chrono::milliseconds(1500)); 
                return 0;
            case 3:
                clearScrean(); // Bersihkan layar dulu biar fokus
                cout << KUNING << "==============================================\n";
                cout << "==             PUSAT BANTUAN                ==\n";
                cout << "==============================================\n" << RESET;
                
                cout << "\n" << BIRU << "[PANDUAN PENGGUNAAN]" << RESET << endl;
                cout << "1. " << HIJAU << "Input Data" << RESET << "   : Masukkan Nama & Nilai (0-100).\n";
                cout << "2. " << HIJAU << "Input Batch" << RESET << "  : Masukkan banyak data sekaligus.\n";
                cout << "   - Ketik 'menu' di kolom Nama untuk Batal/Stop.\n";
                cout << "3. " << HIJAU << "Edit/Hapus" << RESET << "   : Gunakan Nomor Urut (lihat di menu 4).\n";
                
                cout << "\n" << BIRU << "[ATURAN NILAI]" << RESET << endl;
                cout << "* Grade A : 80 - 100\n";
                cout << "* Grade B : 70 - 79\n";
                cout << "* Grade C : 60 - 69\n";
                cout << "* Grade D : 50 - 59\n";
                cout << "* Grade E : < 50\n";

                cout << "\n" << UNGU << "Tekan Enter untuk kembali ke menu utama..." << RESET;
                cin.get(); // Menahan layar agar user bisa baca
                break;
            case 4:
                tampilkanDatabase();
                cin.get();
                break;
            case 5:
                cariData();
                cin.get();
                break;
            case 6:
                clearScrean(); tampilkanDatabase(); hapusData();
                cin.get();
                break;
            case 7:
                clearScrean(); editData();
                cin.get();
                break;
            case 8:
                clearScrean(); inputBanyakData();
                cin.get();
                break;
            default:
                ketikLine("Pilihan tidak valid!", MERAH);
                cin.get();
        }

    } while (true);

    return 0;
}