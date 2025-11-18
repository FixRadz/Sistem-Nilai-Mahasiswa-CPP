#include "utils.h"
#include <cstdlib>
#include <thread> // Wajib untuk delay
#include <chrono> // Wajib untuk waktu

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