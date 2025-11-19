#include "features.h"
#include "config.h"
#include "utils.h"
#include "database.h"
#include <fstream>

void editData() {
    if (db.empty()) {
        ketikLine("\nBelum ada data untuk diedit.", KUNING);
        return;
    }
    tampilkanDatabase();
    string target = inputString("\nMasukkan NIM yang ingin diedit (ketik 'b' batal): ");
    if (target == "b" || target == "B") {
        ketikLine("Edit dibatalkan.", KUNING);
        return;
    }

    int idx = cariIndexByNIM(target);

    if (idx == -1) {
        ketikLine("!!! NIM tidak ditemukan. Cek kembali penulisan NIM.", MERAH);
        return;
    }

    cout << "\nSedang mengedit data: " << UNGU << db[idx].nama << " (" << db[idx].nim << ")" << RESET << endl;
    cout << KUNING << "[TIPS] Tekan ENTER langsung jika tidak ingin mengubah data." << RESET << endl;

    // --- EDIT NAMA ---
    cout << "Nama Baru (" << db[idx].nama << "): ";
    string namaBaru;
    getline(cin, namaBaru); // Pakai getline biar bisa kosong
    if (!namaBaru.empty()) db[idx].nama = namaBaru;

    // --- EDIT NILAI (String dulu baru convert ke Float) ---
    string tgs, uts, uas;
    
    cout << "Nilai Tugas (" << db[idx].tugas << "): "; 
    getline(cin, tgs);
    if (!tgs.empty()) db[idx].tugas = stof(tgs);

    cout << "Nilai UTS   (" << db[idx].uts << "): "; 
    getline(cin, uts);
    if (!uts.empty()) db[idx].uts = stof(uts);

    cout << "Nilai UAS   (" << db[idx].uas << "): "; 
    getline(cin, uas);
    if (!uas.empty()) db[idx].uas = stof(uas);

    // 6. HITUNG ULANG (Recalculate)
    db[idx].akhir  = hitungNilaiAkhir(db[idx].tugas, db[idx].uts, db[idx].uas);
    db[idx].grade  = tentukanGrade(db[idx].akhir);
    db[idx].status = statusLulus(db[idx].akhir);

    // 7. SIMPAN PERUBAHAN
    saveToFile(); // Auto-Save
    ketikLine("Data berhasil diperbarui dan disimpan!", HIJAU);
}

void inputBanyakData() {
    cout << KUNING << "\n=== MODE INPUT BANYAK DATA (BATCH) ===" << RESET << endl;
    
    string strJumlah = inputString("Mau input berapa mahasiswa? (ketik 'b' kembali): ");
    if (strJumlah == "b" || strJumlah == "B") return;

    int targetJumlah;
    try { targetJumlah = stoi(strJumlah); } catch (...) { return; }
    if (targetJumlah <= 0) return;

    // Gunakan Vector sementara untuk draft
    vector<DataMhs> draft; 
    int jumlahTerisi = 0;

    ketikLine("\n[TIPS] Ketik 'menu' di kolom NIM/Nama untuk berhenti.", BIRU);
    
    for (int i = 0; i < targetJumlah; i++) {
        cout << "\n--- Data ke-" << (i + 1) << " dari " << targetJumlah << " ---\n";
        
        DataMhs temp;

        // --- 1. INPUT NIM (CEK DUPLIKAT DATABASE & DRAFT) ---
        while (true) {
            temp.nim = inputString("Masukkan NIM     : ");
            
            // Cek Interupsi
            if (temp.nim == "menu" || temp.nim == "MENU") goto OpsiInterupsi;

            // Cek Duplikat di Database Utama
            if (cekNIMDuplikat(temp.nim)) {
                ketikLine("!!! NIM sudah ada di database.", MERAH);
                continue;
            }

            // Cek Duplikat di Draft (Batch saat ini)
            bool duplikatDraft = false;
            for(const auto& d : draft) {
                if(d.nim == temp.nim) {
                    ketikLine("!!! NIM ini baru saja Anda masukkan di draft sebelumnya.", MERAH);
                    duplikatDraft = true;
                    break;
                }
            }
            if(!duplikatDraft) break; // Lolos Validasi
        }

        // --- 2. INPUT NAMA ---
        temp.nama = inputString("Masukkan Nama    : ");
        if (temp.nama == "menu" || temp.nama == "MENU") goto OpsiInterupsi;

        // --- 3. INPUT NILAI ---
        temp.tugas = inputAngka("Nilai Tugas      : ");
        temp.uts   = inputAngka("Nilai UTS        : ");
        temp.uas   = inputAngka("Nilai UAS        : ");

        // Hitung
        temp.akhir  = hitungNilaiAkhir(temp.tugas, temp.uts, temp.uas);
        temp.grade  = tentukanGrade(temp.akhir);
        temp.status = statusLulus(temp.akhir);
        
        draft.push_back(temp);
        jumlahTerisi++;
        continue;

        // LABEL UNTUK GOTO (Loncat ke sini kalau ketik 'menu')
        OpsiInterupsi:
            cout << KUNING << "\n>> Opsi Interupsi:\n" << RESET;
            cout << "1. Stop & Review (" << jumlahTerisi << " data)\n";
            cout << "2. Batal Semua\n";
            cout << "3. Lanjut Input\n";
            string opsi = inputString("Pilih: ");

            if (opsi == "1") break; // Keluar loop for, lanjut review
            else if (opsi == "2") return; // Pulang ke menu utama
            else {
                i--; // Ulangi iterasi ini
                cout << "Lanjut input data ke-" << (i+2) << "...\n";
            }
    }

    if (jumlahTerisi == 0) return;

    // --- REVIEW MODE ---
    while (true) {
        clearScrean(); 
        cout << KUNING << "\n=== REVIEW DRAFT (" << jumlahTerisi << " Data) ===" << RESET << endl;
        cout << left << setw(5) << "No" << setw(12) << "NIM" << setw(20) << "Nama" << setw(10) << "Akhir" << endl;
        cout << "------------------------------------------------\n";
        
        for (int i = 0; i < jumlahTerisi; i++) {
             cout << left << setw(5) << i+1 
                  << setw(12) << draft[i].nim 
                  << setw(20) << draft[i].nama
                  << setw(10) << fixed << setprecision(2) << draft[i].akhir << endl;
        }
        cout << "------------------------------------------------\n";
        cout << "[S] Simpan Semua  |  [Nomor] Edit Draft  |  [B] Batal\n";
        
        string pilihan = inputString("Pilihan: ");

        if (pilihan == "s" || pilihan == "S") {
            for (const auto& m : draft) simpanData(m);
            ketikLine("\nSUKSES! Semua data tersimpan.", HIJAU);
            break; 
        }
        else if (pilihan == "b" || pilihan == "B") {
            if (inputString("Yakin batalkan? (y/n): ") == "y") break;
        }
        else {
            try {
                int idx = stoi(pilihan) - 1;
                if (idx >= 0 && idx < jumlahTerisi) {
                    cout << "\n>> Edit Draft No " << (idx+1) << endl;
                    // Disini kita tidak edit NIM biar aman, cuma Nama & Nilai
                    cout << "NIM: " << draft[idx].nim << " (Tidak dapat diubah)\n";
                    
                    string nm = inputString("Nama Baru (- utk tetap): ");
                    if (nm != "-") draft[idx].nama = nm;
                    
                    draft[idx].tugas = inputAngka("Tugas: ");
                    draft[idx].uts   = inputAngka("UTS  : ");
                    draft[idx].uas   = inputAngka("UAS  : ");
                    
                    draft[idx].akhir  = hitungNilaiAkhir(draft[idx].tugas, draft[idx].uts, draft[idx].uas);
                    draft[idx].grade  = tentukanGrade(draft[idx].akhir);
                    draft[idx].status = statusLulus(draft[idx].akhir);
                }
            } catch (...) {}
        }
    }
}

void inputSatuanData() {
    while (true) { // Loop agar user bisa pilih 'Ulangi' kalau salah input
        clearScrean();
        cout << KUNING << "=== INPUT DATA SATUAN (SINGLE) ===" << RESET << endl;
        cout << "[INFO] Ketik 'menu' pada Nama untuk batal/kembali.\n\n";

        DataMhs temp;

        while (true) {
            temp.nim = inputString("Masukkan NIM     : ");
            
            // Cek Batal
            if (temp.nim == "menu" || temp.nim == "MENU") return;

            // Cek Duplikat
            if (cekNIMDuplikat(temp.nim)) {
                ketikLine("!!! NIM SUDAH TERDAFTAR. Gunakan NIM lain.", MERAH);
            } else {
                break; // NIM Aman, lanjut
            }
        }

        // 1. Input Nama dengan Fitur "Batal"
        temp.nama = inputString("Masukkan Nama Siswa: ");
        
        // Cek Keyword Rahasia
        if (temp.nama == "menu" || temp.nama == "MENU") {
            string confirm = inputString("Batal input dan kembali ke menu? (y/n): ");
            if (confirm == "y" || confirm == "Y") return; // Keluar fungsi, balik ke Main Menu
            else continue; // Ulangi input nama
        }

        // 2. Input Nilai
        temp.tugas = inputAngka("Nilai Tugas: ");
        temp.uts   = inputAngka("Nilai UTS  : ");
        temp.uas   = inputAngka("Nilai UAS  : ");

        // 3. Hitung
        temp.akhir  = hitungNilaiAkhir(temp.tugas, temp.uts, temp.uas);
        temp.grade  = tentukanGrade(temp.akhir);
        temp.status = statusLulus(temp.akhir);

        // 4. Tahap Review (Tinjau)
        cout << "\n" << BIRU << "--- REVIEW DATA (BELUM DISIMPAN) ---" << RESET << endl;
        cout << "NIM    : " << temp.nim << endl;
        cout << "Nama   : " << temp.nama << endl;
        cout << "Akhir  : " << fixed << setprecision(2) << temp.akhir << endl;
        cout << "Grade  : " << temp.grade << endl;
        cout << "Status : " << ((temp.status == "LULUS") ? HIJAU : MERAH) << temp.status << RESET << endl;
        cout << "------------------------------------\n";

        string opsi = inputString("[S]impan  |  [U]langi Input  |  [B]atal : ");

        if (opsi == "s" || opsi == "S") {
            simpanData(temp);
            ketikLine("\nData berhasil disimpan ke database!", HIJAU);
            break; // Selesai, keluar loop
        }
        else if (opsi == "u" || opsi == "U") {
            ketikLine("\nOke, silakan input ulang...", KUNING);
            // Loop akan otomatis mengulang dari atas
        }
        else if (opsi == "b" || opsi == "B") {
            ketikLine("\nInput dibatalkan.", MERAH);
            return; // Keluar tanpa simpan
        }
    }
}

void urutkanData() {
    if (db.size() < 2) {
        ketikLine("\nData terlalu sedikit untuk diurutkan.", KUNING);
        return;
    }

    cout << KUNING << "\n=== MENU PENGURUTAN DATA ===" << RESET << endl;
    cout << "1. Ranking (Nilai Tertinggi -> Terendah)" << endl;
    cout << "2. Nama (A -> Z)" << endl;
    cout << "3. NIM  (0 -> 9)" << endl; // Tambahan Baru
    cout << "4. Batal" << endl;
    
    string pil = inputString("Pilih (1-4): ");
    if (pil == "4") return;

    ketikLine("Sedang mengurutkan data...", KUNING);

    // BUBBLE SORT untuk VECTOR
    for (size_t i = 0; i < db.size() - 1; i++) {
        for (size_t j = 0; j < db.size() - i - 1; j++) {
            bool tukar = false;

            if (pil == "1") {
                // Descending Nilai Akhir
                if (db[j].akhir < db[j + 1].akhir) tukar = true;
            } 
            else if (pil == "2") {
                // Ascending Nama
                if (db[j].nama > db[j + 1].nama) tukar = true;
            }
            else if (pil == "3") {
                // Ascending NIM
                if (db[j].nim > db[j + 1].nim) tukar = true;
            }

            if (tukar) {
                // Swap Data di Vector
                DataMhs temp = db[j];
                db[j] = db[j + 1];
                db[j + 1] = temp;
            }
        }
    }

    saveToFile(); // Simpan urutan baru permanen
    ketikLine("Data berhasil diurutkan!", HIJAU);
    tampilkanDatabase(); 
}

void tampilkanStatistik() {
    if (db.empty()) {
        ketikLine("\nBelum ada data untuk dihitung.", KUNING);
        return;
    }

    float totalNilai = 0;
    float nilaiTertinggi = -1;
    float nilaiTerendah = 101;
    string namaTertinggi = "-", nimTertinggi = "-";
    string namaTerendah = "-", nimTerendah = "-";
    int lulus = 0, tidakLulus = 0;

    // Loop Analisis Data
    for (const auto& mhs : db) {
        float val = mhs.akhir;
        totalNilai += val;

        if (val > nilaiTertinggi) {
            nilaiTertinggi = val;
            namaTertinggi = mhs.nama;
            nimTertinggi = mhs.nim;
        }
        if (val < nilaiTerendah) {
            nilaiTerendah = val;
            namaTerendah = mhs.nama;
            nimTerendah = mhs.nim;
        }

        if (mhs.status == "LULUS") lulus++;
        else tidakLulus++;
    }

    float rataRata = totalNilai / db.size();

    // TAMPILAN DASHBOARD
    clearScrean();
    cout << BIRU << "==============================================\n";
    cout << "==            DASHBOARD STATISTIK           ==\n";
    cout << "==============================================\n" << RESET;
    
    cout << "Total Mahasiswa : " << db.size() << " Orang\n\n";

    cout << "[PERFORMA KELAS]\n";
    cout << "Rata-Rata Kelas : " << fixed << setprecision(2) << rataRata << endl;
    cout << "Ranking 1       : " << HIJAU << namaTertinggi << " (" << nimTertinggi << ") - " << nilaiTertinggi << RESET << endl;
    cout << "Nilai Terendah  : " << MERAH << namaTerendah << " (" << nimTerendah << ") - " << nilaiTerendah << RESET << endl;

    cout << "\n[STATUS KELULUSAN]\n";
    cout << "Lulus           : " << HIJAU << lulus << " Orang" << RESET << endl;
    cout << "Tidak Lulus     : " << MERAH << tidakLulus << " Orang" << RESET << endl;

    // Bar Visualisasi Sederhana
    cout << "\n[GRAFIK SEDERHANA]\n";
    cout << "LULUS (" << (lulus*100/db.size()) << "%) : ";
    for(int k=0; k<lulus; k++) cout << HIJAU << "|" << RESET;
    cout << endl;
    
    cout << "GAGAL (" << (tidakLulus*100/db.size()) << "%) : ";
    for(int k=0; k<tidakLulus; k++) cout << MERAH << "|" << RESET;
    cout << endl;

    cout << BIRU << "==============================================\n" << RESET;
}

void exportToCSV() {
    if (db.empty()) {
        ketikLine("\nBelum ada data untuk diekspor.", KUNING);
        return;
    }

    string namaFile = "Laporan_Nilai_Mahasiswa.csv";
    ofstream file(namaFile);

    ketikLine("Sedang membuat file Excel...", KUNING);

    if (file.is_open()) {
        // 1. Tulis Header (Judul Kolom)
        // Tips: Gunakan koma (,) sebagai pemisah kolom Excel
        file << "No,NIM,Nama Mahasiswa,Tugas,UTS,UAS,Nilai Akhir,Grade,Status\n";

        // 2. Loop isi data
        for (int i = 0; i < db.size(); i++) {
            file << (i + 1) << ","
                 << db[i].nim << ","
                 << db[i].nama << ","
                 << db[i].tugas << ","
                 << db[i].uts << ","
                 << db[i].uas << ","
                 << db[i].akhir << ","
                 << db[i].grade << ","
                 << db[i].status << "\n";
        }
        file.close();
        
        cout << HIJAU << "\n[SUKSES] Data berhasil diekspor ke '" << namaFile << "'!" << RESET << endl;
        cout << "Silakan cek folder proyek Anda dan buka file tersebut dengan Excel.\n";
    } else {
        ketikLine("Gagal membuat file. Cek izin folder.", MERAH);
    }
}