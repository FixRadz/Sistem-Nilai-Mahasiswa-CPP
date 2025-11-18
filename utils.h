#ifndef UTILS_H
#define UTILS_H

#include "config.h"

// Fungsi Visual & Utilitas
void clearScrean();
void cekExit(const string& input);
void ketik(string teks, int delay = 20); // Efek ngetik
void ketikLine(string teks, string warna = RESET, int delay = 20); // Ngetik + Warna + Enter

// Fungsi Input
string inputString(string pesan);
float inputAngka(string pesan);

// Logika Matematika
float hitungNilaiAkhir(float tugas, float uts, float uas);
char tentukanGrade(float n);
string statusLulus(float n);

#endif