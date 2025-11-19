# 🎓 Sistem Manajemen Nilai Mahasiswa (Advanced CLI)

![C++](https://img.shields.io/badge/Language-C++-blue.svg)
![Status](https://img.shields.io/badge/Status-Completed-success.svg)
![Security](https://img.shields.io/badge/Security-Encrypted-red.svg)

Aplikasi berbasis *Command Line Interface* (CLI) yang dirancang untuk mengelola database nilai mahasiswa secara profesional. Program ini tidak hanya sekadar kalkulator nilai, melainkan sebuah **Sistem Manajemen Database** mini yang dilengkapi dengan keamanan data, penyimpanan persisten, dan antarmuka interaktif.

---

## 🔥 Fitur Unggulan (Key Features)

### 🛡️ 1. Keamanan Tingkat Tinggi (Security)
* **Login System:** Dilengkapi autentikasi Admin saat aplikasi dibuka.
* **Enkripsi Data (XOR Cipher):** Database disimpan dalam format teks terenkripsi (tidak bisa dibaca manusia/Notepad).
* **Integrity Check:** Mencegah *crash* akibat manipulasi data manual (file korup).
* **File Locking:** Database otomatis di-set menjadi *Hidden* & *Read-Only* oleh sistem operasi agar tidak terhapus tidak sengaja.
* **Recovery Mode:** Fitur "Lupa Password" menggunakan PIN Pemulihan.

### 💾 2. Manajemen Data Canggih (Data Engineering)
* **Persistent Database:** Data tersimpan permanen di hard drive (tidak hilang saat aplikasi ditutup).
* **Auto-Save System:** Setiap perubahan (Input/Edit/Hapus) langsung disimpan otomatis.
* **Primary Key (NIM):** Menggunakan NIM sebagai kunci unik untuk mencegah duplikasi data.
* **Dynamic Storage:** Menggunakan `std::vector` (Modern C++) sehingga jumlah data tidak terbatas.

### 📊 3. Analisis & Laporan
* **Sorting:** Mengurutkan data berdasarkan Ranking (Nilai), Nama (A-Z), atau NIM.
* **Statistik Kelas:** Menampilkan rata-rata kelas, nilai tertinggi/terendah, dan grafik kelulusan sederhana.
* **Export to Excel:** Fitur ekspor laporan ke format `.csv` yang kompatibel dengan Microsoft Excel.

### 🎨 4. User Experience (UX)
* **Batch Input:** Mode input banyak data sekaligus dengan fitur *Draft* dan *Review*.
* **Interactive UI:** Menggunakan warna (ANSI Color Code) dan efek *typing delay* agar terasa hidup.
* **Robust Input:** Validasi input angka/huruf untuk mencegah program *error*.

---

## 📂 Struktur Modular
Program ini dibangun dengan arsitektur modular untuk kemudahan pengembangan:

| Nama File | Deskripsi Fungsi |
| :--- | :--- |
| **`main.cpp`** | **Orchestrator.** Mengatur alur utama, login, dan menu navigasi. |
| **`database.cpp`** | **Backend Logic.** Mengurus Load/Save file, Enkripsi, dan CRUD Vector. |
| **`features.cpp`** | **Frontend Logic.** Menangani fitur Input, Sorting, Statistik, dan Export. |
| **`utils.cpp`** | **Utility & Security.** Mesin enkripsi, sistem login, dan helper visual. |
| **`config.h`** | **Configuration.** Menyimpan Struct data dan konstanta global. |

---

## 🚀 Cara Instalasi & Menjalankan

Karena program ini terdiri dari beberapa file, Anda harus mengompilasinya secara bersamaan.

### Prasyarat
Pastikan Anda memiliki compiler C++ (seperti **g++** atau MinGW) yang terinstal.

### Langkah Kompilasi (Terminal / CMD)
Jalankan perintah berikut di folder proyek:

```bash
g++ main.cpp utils.cpp database.cpp features.cpp -o AppNilai
```

### Cara Menjalankan

#### Windows:
```bash
./app_nilai.exe
```

#### Linux/Mac:
```bash
./app_nilai
```
## 📖 Panduan Penggunaan Singkat

# Dibuat oleh: M. Fikri Raditya A. || NIM: 241321050
