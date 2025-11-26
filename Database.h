#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include "Mahasiswa.h"
#include "sqlite3.h" // Library SQLite

// Struktur untuk Log Aktivitas (Fitur Baru)
struct LogEntry {
    std::string timestamp;
    std::string activity;
};

class DatabaseManager {
private:
    sqlite3* db; // Koneksi ke Database SQL
    std::string dbFile;
    std::vector<Mahasiswa> cacheList;

    // Helper untuk eksekusi query tanpa return data (INSERT/UPDATE/DELETE)
    bool executeQuery(const std::string& query);
    

public:
    DatabaseManager();
    ~DatabaseManager();
    void initTables();

    // === CRUD MAHASISWA ===
    // Perhatikan: Kita tidak butuh 'save()' manual lagi. SQL itu auto-save.
    bool tambahMahasiswa(const Mahasiswa& mhs);
    bool hapusMahasiswa(std::string nim);
    bool editMahasiswa(std::string nim, std::string nama, std::string jurusan, float t, float u, float a); // Tambah Jurusan
    
    // === PENCARIAN & DATA ===
    bool isNIMExist(std::string nim);
    Mahasiswa getMahasiswaByNIM(std::string nim);
    
    // Fungsi ambil data fleksibel (Bisa Filter/Search via SQL Query)
    std::vector<Mahasiswa> getMahasiswaData(std::string keyword = "", std::string filterStatus = "SEMUA");

    // === FITUR BARU: LOG SYSTEM ===
    void catatLog(std::string aktivitas); // Simpan ke Tabel Logs
    std::vector<LogEntry> getLogs();      // Ambil data Logs buat GUI

    void load(); 
    void save(); 
    const std::vector<Mahasiswa>& getAllData(); 
    int cariIndexByNIM(std::string nim);
    Mahasiswa getMahasiswa(int index);
    int getJumlahData();
    void setAllData(const std::vector<Mahasiswa>& newData);
};

#endif