# 🚀 Sistem Manajemen Nilai Mahasiswa (Enterprise Edition)

![C++](https://img.shields.io/badge/Language-C++17-blue.svg)
![Architecture](https://img.shields.io/badge/Architecture-Modular%20%26%20DLL-orange.svg)
![Security](https://img.shields.io/badge/Security-XOR%20Encryption-red.svg)
![Config](https://img.shields.io/badge/Config-INI%20File-green.svg)

Aplikasi **Sistem Manajemen Database** berbasis CLI yang dirancang dengan standar arsitektur perangkat lunak profesional. Proyek ini mendemonstrasikan implementasi **Dynamic Link Library (DLL)** untuk keamanan enkripsi, **External Configuration** (.ini), serta manajemen memori dinamis menggunakan **Modern C++**.

---

## 🔥 Fitur Unggulan (Key Highlights)

### 🔐 1. Advanced Security Architecture
* **Modular Encryption (DLL):** Algoritma enkripsi dipisahkan ke dalam file pustaka dinamis `mycrypto.dll`. Program utama tidak dapat berjalan tanpa lisensi/kunci dari DLL ini.
* **XOR Ciphering:** Seluruh database siswa dan konfigurasi admin disimpan dalam format teks terenkripsi (tidak terbaca manusia/Notepad).
* **File Locking & Integrity:** Database otomatis di-set menjadi *Hidden* & *Read-Only* oleh sistem operasi (Windows). Program juga memvalidasi "Digital Signature" (`OK|`) pada setiap baris data untuk mencegah *crash* akibat manipulasi data paksa.
* **Secure Login:** Autentikasi Admin dengan fitur **PIN Pemulihan** (Recovery Mode) jika lupa password.

### ⚙️ 2. External Configuration System
* **`config.ini` Support:** Admin dapat mengubah **Nama Database**, **Nama Aplikasi**, dan **Kunci Enkripsi** secara langsung melalui file teks `.ini` tanpa perlu melakukan kompilasi ulang (*Re-compile*) pada program utama.
* **Dynamic Loading:** Program membaca konfigurasi secara *real-time* saat dijalankan.

### 💾 3. Relational Data Engineering
* **Primary Key System:** Menggunakan **NIM** sebagai identitas unik untuk mencegah duplikasi data mahasiswa.
* **Dynamic Storage:** Migrasi dari Array Statis ke `std::vector`, memungkinkan penyimpanan data tanpa batas jumlah.
* **Persistent Storage:** Fitur *Auto-Save* memastikan data tersimpan permanen di Hard Drive setiap kali ada perubahan.

### 📊 4. Reporting & Analytics
* **Smart Sorting:** Pengurutan data berdasarkan Ranking Nilai, Abjad Nama, atau Urutan NIM.
* **Class Statistics:** Dashboard visual untuk memantau performa kelas (Rata-rata, Nilai Tertinggi/Terendah, Grafik Kelulusan).
* **Excel Export:** Ekspor laporan ke format `.csv` (Comma Separated Values).

### 🎨 5. User Experience (UX)
* **Batch Input:** Mode input banyak data sekaligus dengan fitur *Draft*, *Review*, dan validasi duplikasi NIM.
* **Interactive UI:** Menggunakan warna (ANSI Color Code) dan efek *typing delay*.

---

## 📂 Struktur Modular

Proyek ini memisahkan logika menjadi beberapa layer untuk kemudahan *maintenance*:

| File / Modul | Peran & Fungsi |
| :--- | :--- |
| **`main.cpp`** | **Orchestrator.** Mengatur alur utama, inisialisasi sistem, dan UI menu. |
| **`mycrypto.dll`** | **Security Engine.** Pustaka dinamis berisi logika Enkripsi/Dekripsi (Source: `mycrypto.cpp`). |
| **`config.ini`** | **Settings.** File pengaturan eksternal (Nama DB & Kunci Enkripsi). |
| **`database.cpp`** | **Backend.** Menangani CRUD Vector, File I/O, dan Logika NIM. |
| **`features.cpp`** | **Frontend Logic.** Menangani fitur Input Batch, Sorting, Export, dan Statistik. |
| **`utils.cpp`** | **Utility.** Menangani Login System, Config Parser, dan Helper UI. |
| **`config.h`** | **Data Structure.** Menyimpan Struct DataMhs dan konstanta global. |

---

## 🛠️ Cara Instalasi & Kompilasi (PENTING!)

Karena menggunakan DLL, proses kompilasi dilakukan dalam **2 Tahap**. Pastikan Anda menggunakan compiler C++ (seperti MinGW/g++).

### Tahap 1: Kompilasi DLL (Mesin Enkripsi)
Jalankan perintah ini untuk membuat file `mycrypto.dll`:
```bash
g++ -shared -o mycrypto.dll mycrypto.cpp -DBUILD_DLL
```

### Tahap 2: Kompilasi Program Utama (Linking)
Jalankan perintah ini untuk menyatukan program dengan DLL:
```bash
g++ main.cpp utils.cpp database.cpp features.cpp -o AppNilai -L. -lmycrypto
```

### Tahap 3: Setup Konfigurasi
Sebelum menjalankan, pastikan file `config.ini` tersedia di folder yang sama dengan isi:
```bash
[App]
AppName=Sistem Nilai Mahasiswa Pro
Version=3.0

[Database]
FileName=database.txt
AdminFile=admin_config.txt

[Security]
EncryptionKey=FIKRIGANTENG
```

## 📖 Panduan Penggunaan Singkat
* **Jalankan Aplikasi**: Buka file `.exe` hasil kompilasi. Pastikan `mycrypto.dll` berada di folder yang sama.
* **Registrasi Admin**: Saat pertama kali dibuka, sistem akan mendeteksi belum ada admin dan meminta pembuatan Akun & PIN.
* **Manajemen Data**: Lakukan Input (Batch/Satuan), Edit, atau Hapus data menggunakan NIM sebagai acuan.
* **Lupa Password**: Saat login, ketik `LUPA` untuk mereset password menggunakan PIN yang dibuat saat registrasi.
* **Ubah Konfigurasi**: Edit file `config.ini` jika ingin mengganti nama database atau kunci enkripsi (Data lama akan menjadi tidak terbaca jika kunci diubah).

## 👨‍💻 Credits
* **Developer**: M. Fikri Raditya A.
* **NIM**: 241321050

---

Note: Branch `main` berisi versi Enterprise (Full Features). Untuk melihat versi dasar tanpa DLL/Enkripsi, 
silakan cek branch `demo-version`.
