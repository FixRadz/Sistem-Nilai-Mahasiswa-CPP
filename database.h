#ifndef DATABASE_H
#define DATABASE_H

#include "config.h"

extern vector<DataMhs> db;

void simpanData(const DataMhs& m);
void tampilkanDatabase();
void cariData();
void hapusData();

// === FILE HANDLING ===
void saveToFile(); // Simpan ke database.txt
void loadFromFile(); // Baca dari database.txt

// === CEK NIM ===
bool cekNIMDuplikat(string nimBaru);
int cariIndexByNIM(string nimTarget);

#endif