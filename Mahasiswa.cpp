#include "Mahasiswa.h"
#include <iomanip>

const float BOBOT_TUGAS = 0.20;
const float BOBOT_UTS   = 0.30;
const float BOBOT_UAS   = 0.50;

using namespace std;

// Constructor Kosong
Mahasiswa::Mahasiswa() {
    tugas = 0; uts = 0; uas = 0; akhir = 0;
    grade = 'E';
    status = "TIDAK LULUS";
    photoPath = "default.png";
}
void Mahasiswa::setPhotoPath(string path) { this->photoPath = path; }
string Mahasiswa::getPhotoPath() const { return photoPath; }

// Constructor dengan Data Awal
Mahasiswa::Mahasiswa(string n, string nm) {
    nim = n;
    nama = nm;
    tugas = 0; uts = 0; uas = 0; akhir = 0;
    grade = 'E';
    status = "TIDAK LULUS";
}

// === LOGIKA UTAMA ===
void Mahasiswa::setNilai(float t, float u, float a) {
    // Proteksi Data (Sanitization)
    if (t < 0) t = 0; if (t > 100) t = 100;
    if (u < 0) u = 0; if (u > 100) u = 100;
    if (a < 0) a = 0; if (a > 100) a = 100;

    this->tugas = t;
    this->uts = u;
    this->uas = a;
    
    updateStatusKelulusan();
}

void Mahasiswa::updateStatusKelulusan() {
    // 1. Hitung Akhir
    this->akhir = (tugas * BOBOT_TUGAS) + (uts * BOBOT_UTS) + (uas * BOBOT_UAS);
    
    // 2. Tentukan Grade
    if (akhir >= 80) grade = 'A';
    else if (akhir >= 70) grade = 'B';
    else if (akhir >= 60) grade = 'C';
    else if (akhir >= 50) grade = 'D';
    else grade = 'E';

    // 3. Tentukan Status
    status = (akhir >= 60) ? "LULUS" : "TIDAK LULUS";
}

// Setter Manual (Untuk Load Database)
void Mahasiswa::setDataLengkap(string n, string nm, string jur,
                               float t, float u, float a, float fin, char g, string s) {
    nim = n; nama = nm; jurusan = jur;
    tugas = t; uts = u; uas = a;
    akhir = fin; grade = g; status = s;
}

void Mahasiswa::setNama(string nm) {
    nama = nm;
}

// === GETTER (Akses Data) ===
string Mahasiswa::getNim() const { return nim; }
string Mahasiswa::getNama() const { return nama; }
float Mahasiswa::getTugas() const { return tugas; }
float Mahasiswa::getUts() const { return uts; }
float Mahasiswa::getUas() const { return uas; }
float Mahasiswa::getAkhir() const { return akhir; }
char Mahasiswa::getGrade() const { return grade; }
string Mahasiswa::getStatus() const { return status; }

// === TAMPILAN ===
void Mahasiswa::tampilkanInfo() const {
    cout << left << setw(12) << nim 
         << setw(20) << nama 
         << setw(10) << fixed << setprecision(2) << akhir 
         << setw(10) << grade 
         << setw(15) << status << endl;
}

void Mahasiswa::setJurusan(std::string j) { jurusan = j; }
std::string Mahasiswa::getJurusan() const { return jurusan; }