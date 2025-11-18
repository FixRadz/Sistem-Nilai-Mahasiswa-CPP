#ifndef DATABASE_H
#define DATABASE_H

#include "config.h"

extern DataMhs db[MAKS_DATA];
extern int jumlahData;

void simpanData(const DataMhs& m);
void tampilkanDatabase();
void cariData();
void hapusData();

// === FITUR BARU: FILE HANDLING ===
void saveToFile(); // Simpan ke database.txt
void loadFromFile(); // Baca dari database.txt

#endif