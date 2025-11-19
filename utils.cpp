#include "utils.h"
#include "mycrypto.h"
#include <cstdlib>
#include <thread>
#include <chrono>
#include <fstream>

void clearScrean() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// === FUNGSI EFEK KETIKAN (DELAY) ===
void ketik(string teks, int delay) {
    for (char c : teks) {
        cout << c << flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

void ketikLine(string teks, string warna, int delay) {
    cout << warna;
    ketik(teks, delay);
    cout << RESET << endl;
}

void cekExit(const string& input) {
    if (input == "exit" || input == "EXIT") {
        ketikLine("\n...Program dihentikan oleh user. Sampai Jumpa!...", MERAH);
        exit(0);
    }
}

string inputString(string pesan) {
    string teks;
    while (true) {
        cout << pesan; // Pesan input biasa saja biar responsif
        getline(cin, teks);
        cekExit(teks);
        if (!teks.empty()) return teks;
        
        cout << MERAH << "!!! Input tidak boleh kosong!" << RESET << endl;
    }
}

float inputAngka(string pesan) {
    string input;
    float nilai;
    while (true) {
        cout << pesan;
        getline(cin, input);
        cekExit(input);

        try {
            nilai = stof(input);
            if (nilai >= 0 && nilai <= 100) return nilai;
            cout << MERAH << "!!! Input harus rentang 0-100." << RESET << endl;
        }
        catch (...) {
            cout << MERAH << "!!! Input angka tidak valid." << RESET << endl;
        }
    }
}

float hitungNilaiAkhir(float tugas, float uts, float uas) {
    return (tugas * BOBOT_TUGAS) + (uts * BOBOT_UTS) + (uas * BOBOT_UAS);
}

char tentukanGrade(float n) {
    if (n >= BATAS_GRADE_A) return 'A';
    if (n >= BATAS_GRADE_B) return 'B';
    if (n >= BATAS_GRADE_C) return 'C';
    if (n >= BATAS_GRADE_D) return 'D';
    return 'E';
}

string statusLulus(float n) {
    return (n >= BATAS_LULUS) ? "LULUS" : "TIDAK LULUS";
}

// Variable global
string globalUsername = "admin";
string globalPassword = "123";
string globalRecovery = "0000";

void simpanAkun(string user, string pass, string rec) {
    system("attrib -r -h admin_config.txt > nul");
    ofstream file("admin_config.txt");
    if (file.is_open()) {
        file << encryptDecrypt(user) << endl;
        file << encryptDecrypt(pass) << endl;
        file << encryptDecrypt(rec) << endl;
        file.close();
    }
    system("attrib +r +h admin_config.txt > nul");
}

void loginSystem() {
    clearScrean();
    ifstream file("admin_config.txt");
    
    // --- SKENARIO 1: SIGN UP (Admin Baru) ---
    if (!file.is_open()) {
        cout << BIRU << "###############################################\n" << RESET;
        cout << "##        SETUP AWAL ADMINISTRATOR           ##\n";
        cout << BIRU << "###############################################\n\n" << RESET;
        
        ketikLine("[INFO] Belum ada akun. Silakan registrasi.", KUNING);
        
        string newUser = inputString("Buat Username Baru: ");
        string newPass = inputString("Buat Password Baru: ");
        
        cout << "\n" << UNGU << "[PENTING] Buat KODE PEMULIHAN (PIN) 4-6 Digit.\n";
        cout << "Kode ini digunakan jika Anda lupa password." << RESET << endl;
        string newRec  = inputString("PIN Pemulihan: ");
        
        simpanAkun(newUser, newPass, newRec);
        globalUsername = newUser;
        globalPassword = newPass;
        globalRecovery = newRec;
        
        ketikLine("\nRegistrasi Berhasil! Mengunci sistem...", HIJAU);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    } 
    // --- SKENARIO 2: LOGIN (Sudah Ada Akun) ---
    else {
        string encUser, encPass, encRec;
        getline(file, encUser);
        getline(file, encPass);
        getline(file, encRec);
        file.close();

        globalUsername = encryptDecrypt(encUser);
        globalPassword = encryptDecrypt(encPass);
        globalRecovery = encryptDecrypt(encRec);

        cout << BIRU << "###############################################\n" << RESET;
        cout << "##           SECURITY CHECK SYSTEM           ##\n";
        cout << BIRU << "###############################################\n\n" << RESET;

        int percobaan = 3;
        while (percobaan > 0) {
            cout << "Login sebagai " << UNGU << globalUsername << RESET << endl;
            cout << "Ketik " << KUNING << "'LUPA'" << RESET << " jika lupa password.\n";
            cout << "Masukkan Password: ";
            
            string input;
            getline(cin, input);

            // --- FITUR LUPA PASSWORD ---
            if (input == "LUPA" || input == "lupa") {
                cout << "\n" << KUNING << "=== MODE PEMULIHAN AKUN ===" << RESET << endl;
                string inputPIN = inputString("Masukkan PIN Pemulihan Anda: ");
                
                if (inputPIN == globalRecovery) {
                    ketikLine("PIN Benar! Silakan reset password.", HIJAU);
                    string resetPass = inputString("Password Baru: ");
                    simpanAkun(globalUsername, resetPass, globalRecovery);
                    globalPassword = resetPass;
                    ketikLine("Password berhasil direset! Masuk otomatis...", HIJAU);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    return;
                } else {
                    ketikLine("PIN Salah! Kembali ke menu login.", MERAH);
                    cout << "\n";
                    continue;
                }
            }

            // --- CEK PASSWORD ---
            if (input == globalPassword) {
                ketikLine("\nPassword Diterima. Mengakses Database...", HIJAU);
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
                return; 
            } else {
                percobaan--;
                cout << MERAH << ">> Password Salah! Sisa percobaan: " << percobaan << RESET << "\n\n";
            }
        }
        ketikLine("!!! AKSES DITOLAK. SISTEM TERKUNCI !!!", MERAH, 50);
        exit(0);
    }
}

void gantiPasswordAdmin() {
    clearScrean();
    cout << KUNING << "=== GANTI PASSWORD ADMIN ===" << RESET << endl;
    
    // 1. Verifikasi Password Lama
    cout << "Masukkan Password Lama: ";
    string inputOld;
    getline(cin, inputOld);

    if (inputOld != globalPassword) {
        ketikLine("Password lama salah! Akses ditolak.", MERAH);
        return;
    }

    // 2. Input Username Baru
    cout << "\nUsername Baru (Tekan " << BIRU << "Enter" << RESET << " jika tidak ingin mengganti): ";
    string newUser;
    getline(cin, newUser);

    if (newUser.empty()) {
        newUser = globalUsername;
        cout << ">> Username tetap: " << UNGU << newUser << RESET << endl;
    } else {
        cout << ">> Username berubah jadi: " << UNGU << newUser << RESET << endl;
    }
    
    // 3. Input Password Baru
    string newPass = inputString("Password Baru: ");
    string confirmPass = inputString("Konfirmasi Password Baru: ");

    if (newPass != confirmPass) {
        ketikLine("Password konfirmasi tidak sama! Batal.", MERAH);
        return;
    }

    // 4. Opsi Ganti PIN (Opsional)
    string newRec = globalRecovery;
    string gantiPin = inputString("Ganti PIN Pemulihan juga? (y/n): ");
    if (gantiPin == "y" || gantiPin == "Y") {
        newRec = inputString("PIN Baru: ");
    }

    // 5. Simpan
    simpanAkun(newUser, newPass, newRec);
    globalUsername = newUser;
    globalPassword = newPass;
    globalRecovery = newRec;

    ketikLine("\nAkun admin berhasil diperbarui!", HIJAU);
}

string getIniValue(string section, string key) {
    ifstream file("config.ini");
    if (!file.is_open()) return ""; 

    string line, currentSection, result = "";
    while (getline(file, line)) {
        if (line.empty() || line[0] == ';') continue;
        if (line.front() == '[' && line.back() == ']') {
            currentSection = line.substr(1, line.size() - 2);
            continue;
        }
        if (currentSection == section) {
            size_t pos = line.find('=');
            if (pos != string::npos) {
                string k = line.substr(0, pos);
                if (k == key) {
                    result = line.substr(pos + 1);
                    break;
                }
            }
        }
    }
    file.close();
    return result;
}