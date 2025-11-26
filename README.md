# 🎓 Sistem Informasi Akademik (SIAKAD) — Enterprise Edition
## v3.0 (GUI, SQL, Dashboard, DLL Security)

![C++](https://img.shields.io/badge/Language-C++17-blue.svg)
![GUI](https://img.shields.io/badge/Framework-wxWidgets%203.2-orange.svg)
![Database](https://img.shields.io/badge/Database-SQLite3-green.svg)
![Security](https://img.shields.io/badge/Security-XOR%20%2B%20DLL-red.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%2010%2F11-lightgrey.svg)
![Status](https://img.shields.io/badge/Build-Stable-brightgreen.svg)

> Aplikasi manajemen nilai akademik mahasiswa berbasis **C++ GUI** dengan **SQL Database**, **Keamanan DLL**, dan **Dashboard Statistik**. Dikembangkan dengan standar **Enterprise Software**.

---

## ✨ Deskripsi Singkat
**Sistem Nilai Mahasiswa v3.0** merupakan evolusi dari aplikasi:
- **v1.0 → Console + File TXT**
- **v2.0 → Console + OOP + File TXT**
- **v3.0 → GUI + SQLite + DLL Security + Dashboard**

Versi GUI ini menghadirkan pengalaman aplikasi profesional untuk kebutuhan akademik dan penelitian.

---

## 🔥 Fitur Unggulan

### 🖥️ 1. Antarmuka Modern & Responsif
- Native Windows GUI (wxWidgets)
- Tab Navigation: **Input / Database / Dashboard / Log Aktivitas**
- Smart UX: Auto Select Text • Enter Navigation • Real-time Validation
- Donut Chart untuk statistik kelulusan

### 🗄️ 2. Backend SQL Performa Tinggi
| Fitur | Status |
|-------|--------|
| SQLite Database | ✔ |
| Pencarian realtime | ✔ |
| Sorting otomatis | ✔ |
| Filter lulus/gagal | ✔ |
| Performa ribuan data | ✔ |

### 📸 3. Asset Manager Pintar
- Upload pas foto mahasiswa
- Otomatis di-*compress* & *resize*
- *Garbage Collection* → Foto terhapus otomatis ketika data dihapus

### 🛡️ 4. Sistem Keamanan Enterprise
| Komponen | Teknologi |
|---------|-----------|
| Login & autentikasi | ✔ |
| PIN Recovery | ✔ |
| DLL Encryption Module | `mycrypto.dll` |
| File Protection | Hidden + Locked |

### 📄 5. Laporan Akademik Profesional
- Export KHS (HTML Print-Ready)
- Import & Export CSV (Excel Compatible)

---

## 📂 Struktur Arsitektur
```
📦 SistemNilaiV3
 ┣ 📁 assets              → Foto & icon aplikasi
 ┣ 📄 MainApp.exe         → UI & business logic
 ┣ 📄 mycrypto.dll        → Modul enkripsi & autentikasi
 ┣ 📄 sqlite3.o           → Engine database SQLite
 ┣ 📄 config.ini          → Secret key & konfigurasi
 ┗ 📄 data.db             → Database mahasiswa
```

---

## 🚀 Cara Instalasi

### Untuk Pengguna Umum
1. Masuk ke tab **Releases**
2. Download **Setup_SistemNilai_v3.0.exe**
3. Install seperti aplikasi Windows biasa
4. Jalankan dari shortcut Desktop

### Untuk Developer (Build from Source)
**Prasyarat**
- MinGW-w64 (C++17)
- wxWidgets 3.2 (compiled)
- SQLite3 library

**Perintah Build**
```bash
g++ MainApp.cpp MainFrame.cpp LoginDialog.cpp DetailDialog.cpp ChangePasswordDialog.cpp Database.cpp Mahasiswa.cpp assets/resource.o sqlite3.o -o MainApp.exe -I D:\Path\To\wxWidgets\include -I D:\Path\To\wxWidgets\lib\gcc_dll\mswu -L D:\Path\To\wxWidgets\lib\gcc_dll -lwxmsw32u -mwindows -D__WXMSW__ -D_UNICODE -DUNICODE -L. -lmycrypto -static-libgcc -static-libstdc++
```
> Pastikan `sqlite3.o` dan `resource.o` sudah dikompilasi sebelumnya.

---

## 🧪 Roadmap Pengembangan

| Versi | Fitur Utama | Status |
|-------|-------------|--------|
| v1.0 | Console + File TXT | 🔴 Legacy |
| v2.0 | Console + OOP | 🟡 Legacy |
| **v3.0** | **GUI + SQL + DLL + Dashboard** | 🟢 Stable |
| v4.0 | Multi-User + Cloud Sync | 🔜 In Progress |

---

## 👨‍💻 Pengembang
| Informasi | Data |
|----------|------|
| Developer | **M. Fikri R.A.** |
| NIM | **241321050** |
| Institusi | **Politeknik Negeri Bandung** |
| Dosen Pengampu | **Algoritma & Pemrograman** |
| Tools | VS Code • MinGW-w64 • wxWidgets • SQLite • Inno Setup |

---

## 📌 Lisensi
Repositori ini dibagikan untuk keperluan **pendidikan, penelitian, dan pengembangan**.  
Penggunaan komersial tanpa izin **tidak diperbolehkan**.

---

## ⭐ Dukungan
Jika aplikasi ini bermanfaat:
- Klik **Star ⭐** untuk mendukung repository ini
- Boleh dibuat **Fork / Pull Request**
- Terbuka untuk kolaborasi pengembangan versi berikutnya

> _“Code to Learn, Build to Improve.”_ 🚀
