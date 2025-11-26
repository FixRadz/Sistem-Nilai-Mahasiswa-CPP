#include "DetailDialog.h"
#include <wx/statline.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>
#include <fstream>
#include <iomanip>

wxBEGIN_EVENT_TABLE(DetailDialog, wxDialog)
    EVT_BUTTON(wxID_CLOSE, DetailDialog::OnCloseClicked)
    EVT_BUTTON(ID_BTN_PRINT, DetailDialog::OnPrintClicked)
wxEND_EVENT_TABLE()

DetailDialog::DetailDialog(wxWindow* parent, const Mahasiswa& mhs)
    : wxDialog(parent, wxID_ANY, "Detail Mahasiswa", wxDefaultPosition, wxSize(500, 320)) {
    this->currentMhs = mhs;

    wxPanel* panel = new wxPanel(this);
    panel->SetBackgroundColour(*wxWHITE);

    // 1. SETUP FOLDER UTAMA
    wxString exePath = wxStandardPaths::Get().GetExecutablePath();
    wxString appPath = wxFileName(exePath).GetPath(); 
    wxString assetsDir = appPath + "\\assets\\"; 

    // === LOGIKA CERDAS (AUTO-DETECT FOTO) ===
    
    // Opsi A: Coba cari langsung file berdasarkan NIM di folder photos
    // (Asumsi ekstensi .jpg karena kita save sebagai .jpg)
    wxString nimStr = wxString(mhs.getNim());
    wxString tryPathNIM = assetsDir + "photos\\" + nimStr + ".jpg";
    
    // Opsi B: Ambil info dari Database (sebagai cadangan)
    wxString relPathDB = wxString(mhs.getPhotoPath());
    relPathDB.Replace("/", "\\");
    wxString tryPathDB = assetsDir + relPathDB;

    // Opsi C: Default
    wxString pathDefault = assetsDir + "default.png";

    // VARIABEL FINAL
    wxString finalPathToLoad;

    // LOGIKA PRIORITAS:
    // 1. Cek apakah ada file "NIM.jpg" di folder photos?
    if (wxFileExists(tryPathNIM)) {
        finalPathToLoad = tryPathNIM; // KETEMU! Pakai ini.
    }
    // 2. Kalau gak ada, cek path yang tertulis di database
    else if (wxFileExists(tryPathDB) && !relPathDB.IsEmpty()) {
        finalPathToLoad = tryPathDB;
    }
    // 3. Kalau gak ada juga, pakai default
    else {
        finalPathToLoad = pathDefault;
    }

    // === LOAD GAMBAR ===
    wxImage img;
    wxLogNull logNo; 
    bool loaded = false;

    // Load Final Path
    if (img.LoadFile(finalPathToLoad, wxBITMAP_TYPE_ANY)) {
        loaded = true;
    } 
    // Backup terakhir kalau final path korup
    else if (img.LoadFile(pathDefault, wxBITMAP_TYPE_ANY)) {
        loaded = true;
    }

    // Fallback Kotak Abu
    if (!loaded) {
        img.Create(150, 200);
        unsigned char* d = img.GetData();
        if(d) for(int i=0; i<150*200*3; i++) d[i]=220;
    }
    img.Rescale(150, 200, wxIMAGE_QUALITY_HIGH);

    // Layout Utama: Horizontal (Kiri Foto, Kanan Teks)
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // === BAGIAN 1: FOTO (KIRI) ===
    wxStaticBitmap* bmpFoto = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(img));
    
    // Frame foto biar cantik
    wxBoxSizer* photoSizer = new wxBoxSizer(wxVERTICAL);
    photoSizer->Add(bmpFoto, 0, wxALL | wxSHAPED, 5);
    
    // === BAGIAN 2: DATA (KANAN) ===
    wxBoxSizer* infoSizer = new wxBoxSizer(wxVERTICAL);
    
    // Font Styles
    wxFont fontLabel = wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    wxFont fontValue = wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    wxFont fontGrade = wxFont(24, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

    // Nama & NIM
    wxStaticText* lblNama = new wxStaticText(panel, wxID_ANY, mhs.getNama());
    wxFont fontNama = fontLabel; fontNama.SetPointSize(14);
    lblNama->SetFont(fontNama);
    
    wxStaticText* lblNim = new wxStaticText(panel, wxID_ANY, "NIM: " + mhs.getNim());
    lblNim->SetForegroundColour(wxColor(100, 100, 100));

    // Jurusan
    wxStaticText* lblJur = new wxStaticText(panel, wxID_ANY, "Jurusan: " + mhs.getJurusan());
    wxFont fontJur = fontLabel; fontJur.SetPointSize(10); fontJur.SetWeight(wxFONTWEIGHT_NORMAL);
    lblJur->SetFont(fontJur);
    lblJur->SetForegroundColour(wxColor(0, 0, 128));

    // Nilai Rinci
    wxString strNilai = wxString::Format("Tugas: %.0f  |  UTS: %.0f  |  UAS: %.0f", 
                                         mhs.getTugas(), mhs.getUts(), mhs.getUas());
    wxStaticText* lblRinci = new wxStaticText(panel, wxID_ANY, strNilai);

    // Garis Pemisah
    wxStaticLine* line = new wxStaticLine(panel, wxID_ANY);

    // Nilai Akhir & Status
    wxString strAkhir = wxString::Format("Nilai Akhir: %.2f", mhs.getAkhir());
    wxStaticText* lblAkhir = new wxStaticText(panel, wxID_ANY, strAkhir);
    lblAkhir->SetFont(fontLabel);

    // Grade Besar
    wxStaticText* lblGrade = new wxStaticText(panel, wxID_ANY, wxString(1, mhs.getGrade()));
    lblGrade->SetFont(fontGrade);
    
    // Status Lulus/Gagal
    wxStaticText* lblStatus = new wxStaticText(panel, wxID_ANY, mhs.getStatus());
    wxFont fontStatus = fontLabel; fontStatus.SetPointSize(12);
    lblStatus->SetFont(fontStatus);
    
    if (mhs.getStatus() == "LULUS") {
        lblGrade->SetForegroundColour(wxColor(46, 204, 113)); // Hijau
        lblStatus->SetForegroundColour(wxColor(46, 204, 113));
    } else {
        lblGrade->SetForegroundColour(wxColor(231, 76, 60)); // Merah
        lblStatus->SetForegroundColour(wxColor(231, 76, 60));
    }

    // Susun Info
    infoSizer->Add(lblNama, 0, wxBOTTOM, 2);
    infoSizer->Add(lblNim, 0, wxBOTTOM, 5);
    infoSizer->Add(lblJur, 0, wxBOTTOM, 20);
    infoSizer->Add(lblRinci, 0, wxBOTTOM, 10);
    infoSizer->Add(line, 0, wxEXPAND | wxBOTTOM, 10);
    infoSizer->Add(lblAkhir, 0, wxBOTTOM, 5);
    
    // Grade & Status berdampingan
    wxBoxSizer* gradeSizer = new wxBoxSizer(wxHORIZONTAL);
    gradeSizer->Add(lblGrade, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 20);
    gradeSizer->Add(lblStatus, 0, wxALIGN_CENTER_VERTICAL);
    infoSizer->Add(gradeSizer, 0, wxTOP, 10);

    // Tombol Tutup
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btnPrint = new wxButton(panel, ID_BTN_PRINT, "Cetak KHS (HTML)");
    wxButton* btnClose = new wxButton(panel, wxID_CLOSE, "Tutup");

    btnSizer->Add(btnPrint, 0, wxRIGHT, 10);
    btnSizer->Add(btnClose, 0);

    // === UPDATE LAYOUT UTAMA ===
    mainSizer->Add(photoSizer, 0, wxALL, 20);
    mainSizer->Add(infoSizer, 1, wxTOP | wxBOTTOM | wxRIGHT | wxEXPAND, 20);

    wxBoxSizer* rootSizer = new wxBoxSizer(wxVERTICAL);
    rootSizer->Add(mainSizer, 1, wxEXPAND);
    
    // Masukkan btnSizer (bukan btnClose sendirian)
    rootSizer->Add(btnSizer, 0, wxALIGN_RIGHT | wxALL, 15); 

    panel->SetSizer(rootSizer);
    Center();
}

void DetailDialog::OnCloseClicked(wxCommandEvent& event) {
    EndModal(wxID_OK);
}

void DetailDialog::OnPrintClicked(wxCommandEvent& event) {
    wxString exePath = wxStandardPaths::Get().GetExecutablePath();
    wxString appPath = wxFileName(exePath).GetPath();
    
    // Tentukan nama folder tujuan: "reports"
    wxString reportDir = appPath + "\\reports";

    // Buat folder jika belum ada
    if (!wxDirExists(reportDir)) {
        wxFileName::Mkdir(reportDir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
    }

    // === 2. TENTUKAN NAMA FILE LENGKAP ===
    // Format: .../reports/KHS_101.html
    wxString fullPath = reportDir + "\\KHS_" + currentMhs.getNim() + ".html";
    
    // === 3. TULIS KODE HTML ===
    std::ofstream file(fullPath.ToStdString());
    
    if (!file.is_open()) {
        wxMessageBox("Gagal membuat file laporan!\nCek izin folder.", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    file << "<!DOCTYPE html><html><head><title>KHS - " << currentMhs.getNama() << "</title>";
    file << "<style>";
    file << "body { font-family: 'Times New Roman', serif; padding: 40px; max-width: 800px; margin: auto; }";
    file << ".header { text-align: center; border-bottom: 2px solid black; padding-bottom: 10px; margin-bottom: 30px; }";
    file << ".logo { font-size: 24px; font-weight: bold; }";
    file << ".sub { font-size: 14px; color: #555; }";
    file << ".info { margin-bottom: 20px; }";
    file << "table { width: 100%; border-collapse: collapse; margin-top: 20px; }";
    file << "th, td { border: 1px solid black; padding: 10px; text-align: center; }";
    file << "th { background-color: #f2f2f2; }";
    file << ".ttd { margin-top: 50px; text-align: right; }";
    file << "</style></head><body>";

    // KOP SURAT
    file << "<div class='header'>";
    file << "<div class='logo'>POLITEKNIK NEGERI BANDUNG</div>";
    file << "<div class='sub'>Jl. Gegerkalong Hilir, Ciwaruga, Kec. Parongpong, Kabupaten Bandung Barat</div>";
    file << "<h2>KARTU HASIL STUDI (KHS)</h2>";
    file << "</div>";

    // BIODATA
    file << "<div class='info'>";
    file << "<strong>NIM:</strong> " << currentMhs.getNim() << "<br>";
    file << "<strong>Nama:</strong> " << currentMhs.getNama() << "<br>";
    file << "<strong>Jurusan:</strong> " << currentMhs.getJurusan() << "<br>";
    file << "</div>";

    // TABEL NILAI
    file << "<table>";
    file << "<tr><th>Komponen</th><th>Nilai</th><th>Bobot</th></tr>";
    file << "<tr><td>Tugas</td><td>" << currentMhs.getTugas() << "</td><td>20%</td></tr>";
    file << "<tr><td>UTS</td><td>" << currentMhs.getUts() << "</td><td>30%</td></tr>";
    file << "<tr><td>UAS</td><td>" << currentMhs.getUas() << "</td><td>50%</td></tr>";
    file << "<tr><th colspan='2'>NILAI AKHIR</th><th>" << std::fixed << std::setprecision(2) << currentMhs.getAkhir() << "</th></tr>";
    file << "<tr><th colspan='2'>GRADE</th><th>" << currentMhs.getGrade() << "</th></tr>";
    file << "</table>";

    // STATUS
    std::string warnaStatus = (currentMhs.getStatus() == "LULUS") ? "green" : "red";
    file << "<h3 style='text-align:center; margin-top:30px; color:" << warnaStatus << ";'>STATUS: " << currentMhs.getStatus() << "</h3>";

    // TANDA TANGAN
    file << "<div class='ttd'>";
    file << "<p>Bandung, " << __DATE__ << "</p>"; // Tanggal otomatis
    file << "<br><br><br>";
    file << "<p><strong><u>( Dosen Wali )</u></strong></p>";
    file << "</div>";

    file << "</body></html>";
    file.close();

    // 3. Buka File di Browser Default
    wxLaunchDefaultBrowser(fullPath);
}