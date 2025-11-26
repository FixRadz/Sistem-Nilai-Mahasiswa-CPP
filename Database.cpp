#include "Database.h"
#include <iostream>
#include <ctime>
#include <cstdio>
#include <windows.h> // Wajib untuk File Attribute & Copy

// Header wxWidgets untuk Path
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/utils.h>

using namespace std;

// === CONSTRUCTOR: SETUP AMAN ===
DatabaseManager::DatabaseManager() {
    // 1. Tentukan Lokasi Database (Absolute Path)
    wxString exePath = wxStandardPaths::Get().GetExecutablePath();
    // GANTI NAMA JADI 'data.db'
    wxString path = wxFileName(exePath).GetPath() + "\\data.db"; 
    
    dbFile = std::string(path.mb_str());

    // 2. UNLOCK FILE DULU (Supaya SQLite bisa baca/tulis)
    // Kita ubah jadi NORMAL agar tidak error "Access Denied"
    SetFileAttributesA(dbFile.c_str(), FILE_ATTRIBUTE_NORMAL);

    // 3. AUTO-BACKUP (Sebelum koneksi dibuka)
    // Copy 'data.db' ke 'data.db.bak'
    string bakFile = dbFile + ".bak";
    CopyFileA(dbFile.c_str(), bakFile.c_str(), FALSE);
    // Sembunyikan file backup biar gak ganggu
    SetFileAttributesA(bakFile.c_str(), FILE_ATTRIBUTE_HIDDEN);

    // 4. BUKA KONEKSI SQLITE
    int rc = sqlite3_open(dbFile.c_str(), &db);
    
    if (rc) {
        // Kalau gagal, coba lagi (kadang file masih locked oleh antivirus)
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    } else {
        initTables(); // Pastikan tabel siap
    }
}

// === DESTRUCTOR: KUNCI SAAT KELUAR ===
DatabaseManager::~DatabaseManager() {
    // 1. Tutup Koneksi SQL
    sqlite3_close(db);

    // 2. KUNCI & SEMBUNYIKAN FILE (HIDDEN)
    // Kita tidak pakai ReadOnly agar saat dibuka lagi besok tidak ribet,
    // tapi kita kasih HIDDEN biar user gak iseng hapus.
    SetFileAttributesA(dbFile.c_str(), FILE_ATTRIBUTE_HIDDEN);
}

// === SETUP TABEL ===
void DatabaseManager::initTables() {
    // Tabel Mahasiswa (Ada Jurusan & Foto)
    string sqlMhs = "CREATE TABLE IF NOT EXISTS mahasiswa ("
                    "nim TEXT PRIMARY KEY, "
                    "nama TEXT NOT NULL, "
                    "jurusan TEXT, "
                    "tugas REAL, uts REAL, uas REAL, "
                    "akhir REAL, grade CHAR(1), status TEXT, "
                    "photo_path TEXT);";
    
    // Tabel Logs
    string sqlLog = "CREATE TABLE IF NOT EXISTS activity_logs ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "timestamp TEXT, "
                    "activity TEXT);";

    executeQuery(sqlMhs);
    executeQuery(sqlLog);
}

// === HELPER QUERY ===
bool DatabaseManager::executeQuery(const string& query) {
    char* errMsg = 0;
    int rc = sqlite3_exec(db, query.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

// === LOGGING ===
void DatabaseManager::catatLog(string aktivitas) {
    time_t now = time(0);
    string dt = ctime(&now); dt.pop_back();

    string sql = "INSERT INTO activity_logs (timestamp, activity) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    
    if(sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, dt.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, aktivitas.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
}

// === TAMBAH DATA ===
bool DatabaseManager::tambahMahasiswa(const Mahasiswa& m) {
    string sql = "INSERT INTO mahasiswa (nim, nama, jurusan, tugas, uts, uas, akhir, grade, status, photo_path) "
                 "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, m.getNim().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, m.getNama().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, m.getJurusan().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, m.getTugas());
    sqlite3_bind_double(stmt, 5, m.getUts());
    sqlite3_bind_double(stmt, 6, m.getUas());
    sqlite3_bind_double(stmt, 7, m.getAkhir());
    string sGrade(1, m.getGrade());
    sqlite3_bind_text(stmt, 8, sGrade.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 9, m.getStatus().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 10, m.getPhotoPath().c_str(), -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc == SQLITE_DONE) {
        catatLog("Menambahkan Mahasiswa NIM: " + m.getNim());
        return true;
    }
    return false;
}

// === FUNGSI HAPUS (SQL VERSION + CLEANUP) ===
bool DatabaseManager::hapusMahasiswa(std::string nim) {
    // 1. Cari Data Dulu (Buat ambil info foto & nama file)
    Mahasiswa m = getMahasiswaByNIM(nim);
    
    // Kalau NIM tidak ditemukan di SQL, batal
    if (m.getNim() == "") return false; 

    // ====================================================
    // STEP 1: PERSIAPAN PATH
    // ====================================================
    wxString exePath = wxStandardPaths::Get().GetExecutablePath();
    wxString appPath = wxFileName(exePath).GetPath(); 

    // ====================================================
    // STEP 2: HAPUS FOTO PROFIL
    // ====================================================
    std::string photoPath = m.getPhotoPath();
    if (photoPath != "default.png" && !photoPath.empty()) {
        wxString fullPhotoPath = appPath + "\\assets\\" + wxString(photoPath);
        fullPhotoPath.Replace("/", "\\");
        
        if (wxFileExists(fullPhotoPath)) {
            wxRemoveFile(fullPhotoPath);
        }
    }

    // ====================================================
    // STEP 3: HAPUS FILE LAPORAN HTML
    // ====================================================
    wxString reportFile = "KHS_" + wxString(nim) + ".html";
    wxString fullReportPath = appPath + "\\reports\\" + reportFile;

    if (wxFileExists(fullReportPath)) {
        wxRemoveFile(fullReportPath);
    }

    // ====================================================
    // STEP 4: HAPUS DARI DATABASE SQL (INTI)
    // ====================================================
    std::string sql = "DELETE FROM mahasiswa WHERE nim = ?;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, nim.c_str(), -1, SQLITE_STATIC);
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        if (rc == SQLITE_DONE) {
            catatLog("Menghapus Mahasiswa NIM: " + nim);
            
            // Update Cache (Penting buat GUI biar langsung hilang di tabel)
            cacheList = getMahasiswaData(); 
            
            return true;
        }
    }
    return false;
}

// === EDIT DATA ===
bool DatabaseManager::editMahasiswa(string nim, string nama, string jurusan, float t, float u, float a) {
    Mahasiswa temp; 
    temp.setNilai(t, u, a); 

    string sql = "UPDATE mahasiswa SET nama=?, jurusan=?, tugas=?, uts=?, uas=?, akhir=?, grade=?, status=? WHERE nim=?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, nama.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, jurusan.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, t);
        sqlite3_bind_double(stmt, 4, u);
        sqlite3_bind_double(stmt, 5, a);
        sqlite3_bind_double(stmt, 6, temp.getAkhir());
        string sGrade(1, temp.getGrade());
        sqlite3_bind_text(stmt, 7, sGrade.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 8, temp.getStatus().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 9, nim.c_str(), -1, SQLITE_STATIC);

        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        
        if (rc == SQLITE_DONE) {
            catatLog("Edit Data NIM: " + nim);
            return true;
        }
    }
    return false;
}

// === AMBIL DATA (SELECT) ===
vector<Mahasiswa> DatabaseManager::getMahasiswaData(string keyword, string filterStatus) {
    vector<Mahasiswa> list;
    string sql = "SELECT * FROM mahasiswa WHERE 1=1"; 

    if (!keyword.empty()) {
        sql += " AND (nim LIKE '%" + keyword + "%' OR nama LIKE '%" + keyword + "%')";
    }
    if (filterStatus != "SEMUA" && !filterStatus.empty()) {
        if (filterStatus == "LULUS") sql += " AND status = 'LULUS'";
        else if (filterStatus == "TIDAK LULUS") sql += " AND status = 'TIDAK LULUS'";
    }
    
    sql += " ORDER BY nim ASC;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string n = (const char*)sqlite3_column_text(stmt, 0);
            string nm = (const char*)sqlite3_column_text(stmt, 1);
            string jur = (const char*)sqlite3_column_text(stmt, 2);
            float t = (float)sqlite3_column_double(stmt, 3);
            float u = (float)sqlite3_column_double(stmt, 4);
            float a = (float)sqlite3_column_double(stmt, 5);
            float fin = (float)sqlite3_column_double(stmt, 6);
            string gStr = (const char*)sqlite3_column_text(stmt, 7);
            string st = (const char*)sqlite3_column_text(stmt, 8);
            string photo = (const char*)sqlite3_column_text(stmt, 9);

            Mahasiswa m(n, nm);
            m.setDataLengkap(n, nm, jur, t, u, a, fin, gStr[0], st);
            m.setPhotoPath(photo);
            list.push_back(m);
        }
    }
    sqlite3_finalize(stmt);
    return list;
}

// === CEK NIM ===
bool DatabaseManager::isNIMExist(string nim) {
    string sql = "SELECT count(*) FROM mahasiswa WHERE nim = ?;";
    sqlite3_stmt* stmt;
    bool exists = false;
    
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, nim.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            if (sqlite3_column_int(stmt, 0) > 0) exists = true;
        }
    }
    sqlite3_finalize(stmt);
    return exists;
}

// === GET SINGLE ===
Mahasiswa DatabaseManager::getMahasiswaByNIM(string nim) {
    vector<Mahasiswa> res = getMahasiswaData(nim);
    if (!res.empty()) return res[0];
    return Mahasiswa();
}

// === GET LOGS ===
vector<LogEntry> DatabaseManager::getLogs() {
    vector<LogEntry> logs;
    string sql = "SELECT timestamp, activity FROM activity_logs ORDER BY id DESC LIMIT 100;";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            LogEntry l;
            l.timestamp = (const char*)sqlite3_column_text(stmt, 0);
            l.activity = (const char*)sqlite3_column_text(stmt, 1);
            logs.push_back(l);
        }
    }
    sqlite3_finalize(stmt);
    return logs;
}

// === FUNGSI JEMBATAN COMPATIBILITY (WAJIB ADA) ===
void DatabaseManager::load() { cacheList = getMahasiswaData(); }
void DatabaseManager::save() { /* SQL Auto Save */ }
const std::vector<Mahasiswa>& DatabaseManager::getAllData() { cacheList = getMahasiswaData(); return cacheList; }
int DatabaseManager::cariIndexByNIM(std::string nim) {
    cacheList = getMahasiswaData(); 
    for(size_t i = 0; i < cacheList.size(); i++) if(cacheList[i].getNim() == nim) return (int)i;
    return -1;
}
Mahasiswa DatabaseManager::getMahasiswa(int index) {
    if(index >= 0 && index < cacheList.size()) return cacheList[index];
    return Mahasiswa();
}
int DatabaseManager::getJumlahData() { cacheList = getMahasiswaData(); return cacheList.size(); }
void DatabaseManager::setAllData(const std::vector<Mahasiswa>& newData) { cacheList = newData; }