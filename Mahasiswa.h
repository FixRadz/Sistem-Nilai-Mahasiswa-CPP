#ifndef MAHASISWA_H
#define MAHASISWA_H

#include <string>
#include <iostream>


class Mahasiswa {
private:
    std::string nim;
    std::string nama;
    float tugas;
    float uts;
    float uas;
    float akhir;
    char grade;
    std::string status;
    std::string photoPath;
    std::string jurusan;

    void updateStatusKelulusan(); 

public:
    Mahasiswa(); 
    Mahasiswa(std::string n, std::string nm);

    void setNama(std::string nm);
    void setNilai(float t, float u, float a);
    void setPhotoPath(std::string path); // Setter
    std::string getPhotoPath() const;
    
    void setDataLengkap(std::string n, std::string nm, std::string jur,
                    float t, float u, float a, float fin, char g, std::string s);

    std::string getNim() const;
    std::string getNama() const;
    float getTugas() const;
    float getUts() const;
    float getUas() const;
    float getAkhir() const;
    char getGrade() const;
    std::string getStatus() const;
    void setJurusan(std::string j);
    std::string getJurusan() const;

    void tampilkanInfo() const;
};

#endif