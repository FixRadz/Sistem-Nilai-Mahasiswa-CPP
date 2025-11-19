#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

// =================== WARNA (ANSI CODE) ===================
const string RESET  = "\033[0m";
const string MERAH  = "\033[31m";
const string HIJAU  = "\033[32m";
const string KUNING = "\033[33m";
const string BIRU   = "\033[36m"; // Cyan
const string UNGU   = "\033[35m";

// =================== KONSTANTA NILAI ===================
const float BOBOT_TUGAS = 0.20;
const float BOBOT_UTS   = 0.30;
const float BOBOT_UAS   = 0.50;

const int BATAS_GRADE_A = 80;
const int BATAS_GRADE_B = 70;
const int BATAS_GRADE_C = 60;
const int BATAS_GRADE_D = 50;
const int BATAS_LULUS   = 60;

// =================== DATA UTAMA ===================
struct DataMhs {
    string nim;
    string nama;
    float tugas;
    float uts;
    float uas;
    float akhir;
    char grade;
    string status;
};

#endif