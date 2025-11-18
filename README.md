# Aplikasi Manajemen Nilai Mahasiswa (C++)

Program CLI berbasis C++ untuk mengelola data nilai mahasiswa dengan fitur database persisten dan antarmuka interaktif.

## 🚀 Fitur Unggulan
* **Modular Code**: Struktur kode terpisah (Header & Implementation) agar mudah dikembangkan.
* **Auto-Save System**: Data otomatis tersimpan ke `database.txt`. Tidak hilang meski program ditutup.
* **Batch Input**: Fitur input banyak data sekaligus dengan opsi "Interupsi/Batal".
* **Interactive UI**: Menggunakan warna dan efek ketikan agar user experience lebih hidup.
* **CRUD Lengkap**: Create, Read, Update, Delete, dan Search data.

## 🛠️ Cara Kompilasi & Menjalankan
Karena program ini terdiri dari beberapa file, gunakan perintah berikut untuk mengompilasi:

### Menggunakan G++ (Terminal/CMD)
```bash
g++ main.cpp utils.cpp database.cpp features.cpp -o app_nilai
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

## 📂 Struktur File
* **config.h**: Menyimpan konstanta dan struct data.
* **database.cpp**: Mengurus penyimpanan file dan array.
* **features.cpp**: Logika fitur kompleks (Input Batch & Edit).
* **utils.cpp**: Fungsi bantuan (Warna, Delay, Validasi Input).
* **main.cpp**: Menu utama program.
