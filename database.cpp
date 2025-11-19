#include "database.h"
#include "utils.h"
#include <iostream> 
#include <fstream>  
#include <sstream>  // stringstream
#include <iomanip>
#include <chrono>
#include <thread>
#include <vector>

using namespace std;

// Alokasi memori untuk database
vector<DataMhs> db;

// =================== 1. FUNGSI SIMPAN FILE (Auto-Save) ===================
void saveToFile() {
    system("attrib -r -h database.txt > nul"); // Decrypt
    ofstream file("database.txt"); // Membuka file untuk menulis
    
    if (file.is_open()) {
        for (int i = 0; i < db.size(); i++) {
            // Format simpan: Nama|Tugas|UTS|UAS
            string rawLine = "OK|" + db[i].nim + "|" + db[i].nama + "|" + 
                            to_string(db[i].tugas) + "|" + 
                            to_string(db[i].uts) + "|" + 
                            to_string(db[i].uas);
            
            string encryptedLine = encryptDecrypt(rawLine);
            file << encryptedLine << "\n";
        }
        file.close();
    }
    system("attrib +r +h database.txt > nul"); // Encrypt
}

// =================== 2. FUNGSI BACA FILE (Auto-Load) ===================
void loadFromFile() {
    ifstream file("database.txt"); // Membuka file untuk membaca
    if (!file.is_open()) return; // Kalau file tidak ada, lanjut saja (database kosong)

    db.clear();
    string line;
    int dataRusak = 0;

    // Baca baris per baris
    while (getline(file, line)) {
        if (line.empty()) continue;

        // 1. Decrypt
        string decryptedLine = encryptDecrypt(line);

        // 2. CEK INTEGRITAS (Apakah depannya "OK|"? )
        if (decryptedLine.size() < 3 || decryptedLine.substr(0, 3) != "OK|") {
            dataRusak++; 
            continue; // DATA PALSU/RUSAK -> LEWAT!
        }

        // 3. Buang stempel "OK|"
        string cleanLine = decryptedLine.substr(3);

        // 4. Parsing Data
        stringstream ss(cleanLine);
        string segment;
        DataMhs temp;
        
        if (!getline(ss, temp.nim, '|')) continue;
        if (!getline(ss, temp.nama, '|')) continue;

        string sTugas, sUts, sUas;
        getline(ss, sTugas, '|');
        getline(ss, sUts, '|');
        getline(ss, sUas, '|');

        try {
            temp.tugas = stof(sTugas);
            temp.uts   = stof(sUts);
            temp.uas   = stof(sUas);
            temp.akhir  = hitungNilaiAkhir(temp.tugas, temp.uts, temp.uas);
            temp.grade  = tentukanGrade(temp.akhir);
            temp.status = statusLulus(temp.akhir);

            db.push_back(temp); 
        } catch (...) { dataRusak++; }
    }
    file.close();

    if (dataRusak > 0) {
        cout << "\n" << MERAH << "[WARNING] " << dataRusak 
             << " data terdeteksi rusak/dimanipulasi & telah diamankan." << RESET << endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

// =================== 3. SIMPAN DATA KE MEMORI & FILE ===================
void simpanData(const DataMhs& m) {
    db.push_back(m);
    saveToFile();
}

// =================== 4. TAMPILKAN DATABASE ===================
void tampilkanDatabase() {
    if (db.empty()) {
        ketikLine("\n[INFO] Belum ada data mahasiswa tersimpan.", KUNING);
        return;
    }

    cout << BIRU << "\n=================== DATA MAHASISWA TERSIMPAN ===================" << RESET << endl;
    cout << left << setw(5) << "No"
         << setw(12) << "NIM"
         << setw(20) << "Nama"
         << setw(10) << "Akhir"
         << setw(10) << "Grade"
         << setw(15) << "Status" << endl;
    cout << "----------------------------------------------------------------" << endl;

    for (int i = 0; i < db.size(); i++) {
        string warnaStatus = (db[i].status == "LULUS") ? HIJAU : MERAH;
        
        cout << left << setw(5) << i+1
             << setw(12) << db[i].nim
             << setw(20) << db[i].nama
             << setw(10) << fixed << setprecision(2) << db[i].akhir
             << setw(10) << db[i].grade
             << warnaStatus << setw(15) << db[i].status << RESET << endl;
    }
    cout << BIRU << "================================================================" << RESET << endl;
}

// =================== 5. CARI DATA ===================
void cariData() {
    if (db.empty()) {
        ketikLine("\nBelum ada data tersimpan.", KUNING);
        return;
    }

    string target = inputString("\nMasukkan NIM mahasiswa yang dicari: ");

    if (target == "exit" || target == "EXIT") return;
    int idx = cariIndexByNIM(target);

    if (idx == -1) {
        ketikLine("!!! Data dengan NIM tersebut tidak ditemukan.", MERAH);
        return;
    }

    cout << endl;
    cout << HIJAU << "======== DATA DITEMUKAN ========" << RESET << endl;
    cout << "Nama   : " << db[idx].nama << endl;
    cout << "Tugas  : " << db[idx].tugas << endl;
    cout << "UTS    : " << db[idx].uts << endl;
    cout << "UAS    : " << db[idx].uas << endl;
    cout << "--------------------------------" << endl;
    cout << "Akhir  : " << db[idx].akhir << endl;
    cout << "Grade  : " << db[idx].grade << endl;
    cout << "Status : " << ((db[idx].status == "LULUS") ? HIJAU : MERAH) << db[idx].status << RESET << endl;
    cout << HIJAU << "================================" << RESET << endl;
}

// =================== 6. HAPUS DATA ===================
void hapusData() {
    if (db.empty()) {
        ketikLine("\nBelum ada data tersimpan.", KUNING);
        return;
    }

    // 2. Input Target (Fitur Batal TETAP ADA)
    string target = inputString("\nMasukkan NIM yang ingin dihapus (ketik 'b' batal): ");
    
    if (target == "b" || target == "B") {
        ketikLine("Aksi dibatalkan.", KUNING);
        return;
    }

    // 3. Logika Baru: Cari Index Vector berdasarkan NIM
    int idx = cariIndexByNIM(target);

    if (idx == -1) {
        ketikLine("!!! NIM tidak ditemukan di database.", MERAH);
        return;
    }

    // 4. Konfirmasi Penghapusan
    // Tampilkan Nama & NIM biar user makin yakin
    cout << "Yakin hapus data " << UNGU << db[idx].nama << " (" << db[idx].nim << ")" << RESET << "? (y/n): ";
    string yakin; 
    getline(cin, yakin);

    if (yakin == "y" || yakin == "Y") {
        cout << KUNING << "Menghapus data..." << RESET << endl;
        
        // Hapus dari Vector
        db.erase(db.begin() + idx);
        
        // Simpan perubahan ke file (Auto-Save)
        saveToFile();
        
        ketikLine("Data berhasil dihapus secara permanen!", HIJAU);
    } else {
        ketikLine("Penghapusan dibatalkan.", KUNING);
    }
}

bool cekNIMDuplikat(string nimBaru) {
    for (const auto& mhs : db) {
        if (mhs.nim == nimBaru) return true;
    }
    return false;
}

int cariIndexByNIM(string nimTarget) {
    for (size_t i = 0; i < db.size(); i++) {
        if (db[i].nim == nimTarget) return (int)i;
    }
    return -1;
}