#include "database.h"
#include "utils.h"
#include <iostream> 
#include <fstream>  
#include <sstream>  // stringstream
#include <iomanip>

using namespace std;

// Alokasi memori untuk database
DataMhs db[MAKS_DATA];
int jumlahData = 0;

// =================== 1. FUNGSI SIMPAN FILE (Auto-Save) ===================
void saveToFile() {
    ofstream file("database.txt"); // Membuka file untuk menulis
    
    if (file.is_open()) {
        for (int i = 0; i < jumlahData; i++) {
            // Format simpan: Nama|Tugas|UTS|UAS
            file << db[i].nama << "|"
                 << db[i].tugas << "|"
                 << db[i].uts << "|"
                 << db[i].uas << "\n";
        }
        file.close();
    }
}

// =================== 2. FUNGSI BACA FILE (Auto-Load) ===================
void loadFromFile() {
    ifstream file("database.txt"); // Membuka file untuk membaca
    
    if (!file.is_open()) return; // Kalau file tidak ada, lanjut saja (database kosong)

    jumlahData = 0; // Reset memori
    string line;

    // Baca baris per baris
    while (getline(file, line) && jumlahData < MAKS_DATA) {
        stringstream ss(line);
        string segment;
        DataMhs temp;
        
        // 1. Ambil Nama (pisahkan dengan tanda |)
        if (!getline(ss, temp.nama, '|')) continue;

        // 2. Ambil Nilai-nilai (sebagai string dulu)
        string sTugas, sUts, sUas;
        getline(ss, sTugas, '|');
        getline(ss, sUts, '|');
        getline(ss, sUas, '|');

        try {
            // 3. Konversi String ke Float
            temp.tugas = stof(sTugas);
            temp.uts   = stof(sUts);
            temp.uas   = stof(sUas);
            
            // 4. Hitung ulang Grade & Status
            temp.akhir  = hitungNilaiAkhir(temp.tugas, temp.uts, temp.uas);
            temp.grade  = tentukanGrade(temp.akhir);
            temp.status = statusLulus(temp.akhir);

            // 5. Masukkan ke RAM
            db[jumlahData++] = temp;
        } catch (...) {
            // Abaikan jika ada baris data yang rusak/error
        }
    }
    file.close();
}

// =================== 3. SIMPAN DATA KE MEMORI & FILE ===================
void simpanData(const DataMhs& m) {
    if (jumlahData < MAKS_DATA) {
        db[jumlahData++] = m; // Masukkan ke RAM
        saveToFile();         // Langsung update file txt
    }
}

// =================== 4. TAMPILKAN DATABASE ===================
void tampilkanDatabase() {
    if (jumlahData == 0) {
        ketikLine("\n[INFO] Belum ada data mahasiswa tersimpan.", KUNING);
        return;
    }

    cout << BIRU << "\n=================== DATA MAHASISWA TERSIMPAN ===================" << RESET << endl;
    cout << left << setw(5) << "No"
         << setw(20) << "Nama"
         << setw(10) << "Akhir"
         << setw(10) << "Grade"
         << setw(15) << "Status" << endl;
    cout << "----------------------------------------------------------------" << endl;

    for (int i = 0; i < jumlahData; i++) {
        string warnaStatus = (db[i].status == "LULUS") ? HIJAU : MERAH;
        
        cout << left << setw(5) << i+1
             << setw(20) << db[i].nama
             << setw(10) << fixed << setprecision(2) << db[i].akhir
             << setw(10) << db[i].grade
             << warnaStatus << setw(15) << db[i].status << RESET << endl;
    }
    cout << BIRU << "================================================================" << RESET << endl;
}

// =================== 5. CARI DATA ===================
void cariData() {
    if (jumlahData == 0) {
        ketikLine("\nBelum ada data tersimpan.", KUNING);
        return;
    }

    string input;
    int nomor;
    cout << "\nMasukkan nomor data mahasiswa yang dicari: ";
    getline(cin, input);

    if (input == "exit" || input == "EXIT") return;

    try { nomor = stoi(input); }
    catch (...) { 
        ketikLine("!!! Input tidak valid.", MERAH);
        return; 
    }

    if (nomor < 1 || nomor > jumlahData) {
        ketikLine("!!! Nomor tidak ditemukan.", MERAH);
        return;
    }

    int i = nomor - 1;

    cout << endl;
    cout << HIJAU << "======== DATA DITEMUKAN ========" << RESET << endl;
    cout << "Nama   : " << db[i].nama << endl;
    cout << "Tugas  : " << db[i].tugas << endl;
    cout << "UTS    : " << db[i].uts << endl;
    cout << "UAS    : " << db[i].uas << endl;
    cout << "--------------------------------" << endl;
    cout << "Akhir  : " << db[i].akhir << endl;
    cout << "Grade  : " << db[i].grade << endl;
    cout << "Status : " << ((db[i].status == "LULUS") ? HIJAU : MERAH) << db[i].status << RESET << endl;
    cout << HIJAU << "================================" << RESET << endl;
}

// =================== 6. HAPUS DATA ===================
void hapusData() {
    if (jumlahData == 0) {
        ketikLine("\nBelum ada data tersimpan.", KUNING);
        return;
    }

    string input = inputString("\nMasukkan nomor data yang ingin dihapus (ketik 'b' batal): ");
    
    if (input == "b" || input == "B") {
        ketikLine("Aksi dibatalkan.", KUNING);
        return;
    }

    int nomor;
    try { nomor = stoi(input); }
    catch (...) { 
        ketikLine("Input tidak valid.", MERAH);
        return; 
    }

    if (nomor < 1 || nomor > jumlahData) {
        ketikLine("Nomor data tidak ditemukan.", MERAH);
        return;
    }

    int idx = nomor - 1;

    // Konfirmasi
    cout << "Yakin hapus data " << UNGU << db[idx].nama << RESET << "? (y/n): ";
    string yakin; 
    getline(cin, yakin);

    if (yakin == "y" || yakin == "Y") {
        cout << KUNING << "Menghapus data..." << RESET << endl;

        for (int i = idx; i < jumlahData - 1; i++) {
            db[i] = db[i + 1];
        }
        jumlahData--;
        
        saveToFile(); // Update file setelah hapus

        ketikLine("Data berhasil dihapus!", HIJAU);
    } else {
        ketikLine("Batal menghapus.", KUNING);
    }
}