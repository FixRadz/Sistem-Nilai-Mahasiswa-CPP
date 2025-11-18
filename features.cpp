#include "features.h"
#include "config.h"
#include "utils.h"
#include "database.h"

void editData() {
    if (jumlahData == 0) {
        ketikLine("\nBelum ada data untuk diedit.", KUNING);
        return;
    }
    tampilkanDatabase();
    string input = inputString("\nMasukkan nomor data yang ingin diedit (ketik 'b' batal): ");
    if (input == "b" || input == "B") return;

    int nomor;
    try { nomor = stoi(input); } catch (...) { return; }
    if (nomor < 1 || nomor > jumlahData) return;

    int i = nomor - 1;
    cout << "\nEditing data: " << UNGU << db[i].nama << RESET << endl;

    string namaBaru;
    cout << "Nama baru (kosong = tetap): ";
    getline(cin, namaBaru);
    if (!namaBaru.empty()) db[i].nama = namaBaru;

    cout << "Input nilai baru (kosong = tetap):\n";
    string tgs, uts, uas;
    
    cout << "Tugas (" << db[i].tugas << "): "; getline(cin, tgs);
    if (!tgs.empty()) db[i].tugas = stof(tgs);

    cout << "UTS   (" << db[i].uts << "): "; getline(cin, uts);
    if (!uts.empty()) db[i].uts = stof(uts);

    cout << "UAS   (" << db[i].uas << "): "; getline(cin, uas);
    if (!uas.empty()) db[i].uas = stof(uas);

    db[i].akhir  = hitungNilaiAkhir(db[i].tugas, db[i].uts, db[i].uas);
    db[i].grade  = tentukanGrade(db[i].akhir);
    db[i].status = statusLulus(db[i].akhir);

    saveToFile();
    ketikLine("Data berhasil diperbarui dan disimpan!", HIJAU);
}

void inputBanyakData() {
    cout << KUNING << "\n=== MODE INPUT BANYAK DATA (BATCH) ===" << RESET << endl;
    
    string strJumlah = inputString("Mau input berapa mahasiswa? (ketik 'b' kembali): ");
    if (strJumlah == "b" || strJumlah == "B") return;

    int targetJumlah;
    try { targetJumlah = stoi(strJumlah); } catch (...) { return; }
    if (targetJumlah <= 0) return;

    if (jumlahData + targetJumlah > MAKS_DATA) {
        ketikLine("!!! Kapasitas database tidak cukup.", MERAH);
        return;
    }

    DataMhs draft[20]; 
    int jumlahTerisi = 0;

    ketikLine("\n[TIPS] Ketik 'menu' di kolom Nama untuk opsi berhenti/batal.", BIRU);
    
    for (int i = 0; i < targetJumlah; i++) {
        cout << "\n--- Data ke-" << (i + 1) << " dari " << targetJumlah << " ---\n";
        
        string tempNama = inputString("Masukkan Nama Siswa: ");
        
        // LOGIKA CEK KATA KUNCI "MENU"
        if (tempNama == "menu" || tempNama == "MENU") {
            cout << KUNING << "\n>> Opsi Interupsi:\n" << RESET;
            cout << "1. Stop di sini & Lanjut Review (Simpan " << i << " data saja)\n";
            cout << "2. Batal Semua & Kembali ke Menu Utama\n";
            cout << "3. Lanjut Input (Tadi salah ketik)\n";
            string opsi = inputString("Pilih (1/2/3): ");

            if (opsi == "1") {
                ketikLine("Oke, memproses data yang sudah ada...", HIJAU);
                break; 
            } else if (opsi == "2") {
                ketikLine("Aksi dibatalkan. Kembali ke menu...", MERAH);
                return; 
            } else {
                tempNama = inputString("Masukkan Nama Siswa (Ulangi): ");
            }
        }

        draft[i].nama = tempNama;
        draft[i].tugas = inputAngka("Nilai Tugas: ");
        draft[i].uts   = inputAngka("Nilai UTS  : ");
        draft[i].uas   = inputAngka("Nilai UAS  : ");

        draft[i].akhir  = hitungNilaiAkhir(draft[i].tugas, draft[i].uts, draft[i].uas);
        draft[i].grade  = tentukanGrade(draft[i].akhir);
        draft[i].status = statusLulus(draft[i].akhir);
        
        jumlahTerisi++;
    }

    if (jumlahTerisi == 0) return;

    // --- REVIEW MODE ---
    while (true) {
        clearScrean(); 
        cout << KUNING << "\n=== REVIEW DRAFT (" << jumlahTerisi << " Data) ===" << RESET << endl;
        cout << left << setw(5) << "No" << setw(20) << "Nama" << setw(10) << "Akhir" << endl;
        cout << "------------------------------------\n";
        
        for (int i = 0; i < jumlahTerisi; i++) {
             cout << left << setw(5) << i+1 << setw(20) << draft[i].nama
                  << setw(10) << fixed << setprecision(2) << draft[i].akhir << endl;
        }
        cout << "------------------------------------\n";
        cout << "[S] Simpan Semua  |  [Nomor] Edit Draft  |  [B] Batal\n";
        
        string pilihan = inputString("Pilihan: ");

        if (pilihan == "s" || pilihan == "S") {
            for (int i = 0; i < jumlahTerisi; i++) simpanData(draft[i]);
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
                    cout << "\n>> Edit Draft: " << draft[idx].nama << endl;
                    string nm = inputString("Nama Baru (- utk tetap): ");
                    if (nm != "-") draft[idx].nama = nm;
                    draft[idx].tugas = inputAngka("Tugas: ");
                    draft[idx].uts   = inputAngka("UTS  : ");
                    draft[idx].uas   = inputAngka("UAS  : ");
                    // Recalculate
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