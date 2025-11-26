// ================== PART 1: Header & Constructor ==================
#include "MainFrame.h"
#include "DetailDialog.h"
#include "ChangePasswordDialog.h"
#include <wx/msgdlg.h>
#include <wx/textdlg.h>
#include <wx/valnum.h>
#include <wx/graphics.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <sstream>
#include <fstream>
#include <algorithm>

// Event Table
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(ID_BTN_SIMPAN, MainFrame::OnSimpanClicked)
    EVT_BUTTON(ID_BTN_RESET, MainFrame::OnResetClicked)
    EVT_BUTTON(ID_BTN_HAPUS, MainFrame::OnHapusClicked)
    EVT_BUTTON(ID_BTN_EDIT, MainFrame::OnEditClicked)
    EVT_BUTTON(ID_BTN_CARI, MainFrame::OnCariClicked)
    EVT_BUTTON(ID_BTN_REFRESH, MainFrame::OnRefreshClicked)
    EVT_BUTTON(ID_BTN_SORT, MainFrame::OnSortClicked)
    EVT_BUTTON(ID_BTN_UPLOAD, MainFrame::OnUploadClicked)
    EVT_CHOICE(ID_FILTER_CHOICE, MainFrame::OnFilterSelected)
    EVT_LIST_ITEM_ACTIVATED(ID_LIST_DATA, MainFrame::OnTableDoubleClicked)
    
    EVT_MENU(ID_MENU_IMPORT, MainFrame::OnImportClicked)
    EVT_MENU(ID_MENU_EXPORT, MainFrame::OnExportClicked)
    EVT_MENU(ID_MENU_GANTI_PASS, MainFrame::OnGantiPasswordClicked)
    EVT_MENU(ID_MENU_KELUAR, MainFrame::OnExit)
    EVT_MENU(ID_MENU_ABOUT, MainFrame::OnAboutClicked)
    
    EVT_NOTEBOOK_PAGE_CHANGED(ID_NOTEBOOK, MainFrame::OnTabChanged)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 700)) {
    
    SetIcon(wxIcon("MAINICON"));
    SetupMenuBar();
    CreateStatusBar();
    SetStatusText("Sistem Siap (SQL Mode).");

    // Panel Dasar (Parent Utama)
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY);
    
    // Notebook (Tab System) - Parentnya mainPanel
    tabSystem = new wxNotebook(mainPanel, ID_NOTEBOOK);

    // Panel Halaman Tab - Parentnya tabSystem
    wxPanel* panelInput = new wxPanel(tabSystem, wxID_ANY);
    wxPanel* panelData = new wxPanel(tabSystem, wxID_ANY);
    wxPanel* panelStat = new wxPanel(tabSystem, wxID_ANY);
    wxPanel* panelLog = new wxPanel(tabSystem, wxID_ANY);

    // Setup Isi Tab
    SetupTabInput(panelInput);
    SetupTabDatabase(panelData);
    SetupTabStatistik(panelStat);
    SetupTabLog(panelLog);

    // Masukkan ke Tab
    tabSystem->AddPage(panelInput, "Input Data");
    tabSystem->AddPage(panelData, "Database Viewer");
    tabSystem->AddPage(panelStat, "Dashboard");
    tabSystem->AddPage(panelLog, "Log Aktivitas");

    // Layout Utama
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(tabSystem, 1, wxEXPAND | wxALL, 5);
    mainPanel->SetSizer(mainSizer);

    // Init Data
    // (Pastikan Database sudah siap sebelum dipanggil)
    currentSortMode = 0;
    db.initTables(); 
    RefreshTabel();
    RefreshStatistik();
    RefreshLog();
}

void MainFrame::SetupMenuBar() {
    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(ID_MENU_IMPORT, "&Import CSV...");
    fileMenu->Append(ID_MENU_EXPORT, "&Export CSV\tCtrl+E");
    fileMenu->Append(ID_MENU_GANTI_PASS, "&Admin Security");
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_MENU_KELUAR, "&Keluar\tAlt+F4");
    
    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(ID_MENU_ABOUT, "&Tentang...");
    
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(helpMenu, "&Bantuan");
    SetMenuBar(menuBar);
}

// ================== PART 2: UI SETUP (TAB 1 - 4) ==================

void MainFrame::SetupTabInput(wxPanel* panel) {
    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
    wxFloatingPointValidator<double> valNum(2);
    
    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "FORMULIR DATA MAHASISWA");
    wxFont font = title->GetFont(); font.SetPointSize(14); font.SetWeight(wxFONTWEIGHT_BOLD);
    title->SetFont(font);

    // === 1. SETUP AREA FOTO (KANAN) ===
    wxBoxSizer* photoSizer = new wxBoxSizer(wxVERTICAL);
    wxString pathDefault = "assets/default.png";
    
    #ifdef _WIN32 
    SetFileAttributesA(pathDefault.c_str(), FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY);
    #endif

    wxImage img;
    { wxLogNull logNo; 
      if (!img.LoadFile(pathDefault, wxBITMAP_TYPE_PNG)) {
          img.Create(150, 200); 
          unsigned char* d = img.GetData();
          if(d) for(int i=0; i<150*200*3; i++) d[i]=200;
      }
    }
    img.Rescale(150, 200, wxIMAGE_QUALITY_HIGH);

    imgProfil = new wxStaticBitmap(panel, wxID_ANY, wxBitmap(img)); 
    wxButton* btnUpload = new wxButton(panel, ID_BTN_UPLOAD, "Upload Foto");

    photoSizer->Add(imgProfil, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5); 
    photoSizer->Add(btnUpload, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5); 

    // === 2. SETUP FORM INPUT (KIRI) ===
    wxFlexGridSizer* grid = new wxFlexGridSizer(2, 10, 10);
    
    grid->Add(new wxStaticText(panel, wxID_ANY, "NIM:"), 0, wxALIGN_CENTER_VERTICAL);
    txtNim = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(250, -1), wxTE_PROCESS_ENTER);
    grid->Add(txtNim);

    grid->Add(new wxStaticText(panel, wxID_ANY, "Nama:"), 0, wxALIGN_CENTER_VERTICAL);
    txtNama = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(250, -1), wxTE_PROCESS_ENTER);
    grid->Add(txtNama);

    grid->Add(new wxStaticText(panel, wxID_ANY, "Jurusan:"), 0, wxALIGN_CENTER_VERTICAL);
    txtJurusan = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(250, -1), wxTE_PROCESS_ENTER);
    grid->Add(txtJurusan);

    grid->Add(new wxStaticText(panel, wxID_ANY, "Tugas (0-100):"), 0, wxALIGN_CENTER_VERTICAL);
    txtTugas = new wxTextCtrl(panel, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, valNum);
    grid->Add(txtTugas);

    grid->Add(new wxStaticText(panel, wxID_ANY, "UTS (0-100):"), 0, wxALIGN_CENTER_VERTICAL);
    txtUts = new wxTextCtrl(panel, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, valNum);
    grid->Add(txtUts);

    grid->Add(new wxStaticText(panel, wxID_ANY, "UAS (0-100):"), 0, wxALIGN_CENTER_VERTICAL);
    txtUas = new wxTextCtrl(panel, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, valNum);
    grid->Add(txtUas);

    // === LOGIKA NAVIGASI ENTER ===
    txtNim->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent&){ txtNama->SetFocus(); });
    txtNama->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent&){ txtJurusan->SetFocus(); });
    txtJurusan->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent&){ txtTugas->SetFocus(); txtTugas->SelectAll(); });
    txtTugas->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent&){ txtUts->SetFocus(); txtUts->SelectAll(); });
    txtUts->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent&){ txtUas->SetFocus(); txtUas->SelectAll(); });
    txtUas->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnSimpanClicked, this);

    // === PERBAIKAN: LOGIKA AUTO-BLOK (Pakai Lambda di dalam CallAfter) ===
    auto autoSelect = [](wxFocusEvent& e) {
        e.Skip(); // Biarkan sistem memproses fokus dulu
        
        wxTextCtrl* txt = dynamic_cast<wxTextCtrl*>(e.GetEventObject());
        if(txt) {
            // Panggil SelectAll di dalam pembungkus
            txt->CallAfter([txt]() { 
                txt->SelectAll(); 
            });
        }
    };

    // Bind event-nya
    txtTugas->Bind(wxEVT_SET_FOCUS, autoSelect);
    txtUts->Bind(wxEVT_SET_FOCUS, autoSelect);
    txtUas->Bind(wxEVT_SET_FOCUS, autoSelect);

    // Tombol Aksi
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    btnSimpan = new wxButton(panel, ID_BTN_SIMPAN, "SIMPAN DATA");
    btnSimpan->SetBackgroundColour(wxColor(40, 167, 69)); 
    btnSimpan->SetForegroundColour(*wxWHITE);
    btnReset = new wxButton(panel, ID_BTN_RESET, "RESET");
    
    btnSizer->Add(btnSimpan, 1, wxRIGHT, 10);
    btnSizer->Add(btnReset, 0);

    // Layout Akhir
    wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);
    hSizer->Add(grid, 1, wxEXPAND | wxRIGHT, 20);
    hSizer->Add(photoSizer, 0, wxALIGN_TOP);

    vSizer->Add(title, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 20);
    vSizer->Add(hSizer, 0, wxALIGN_CENTER | wxBOTTOM, 20);
    vSizer->Add(btnSizer, 0, wxALIGN_CENTER);

    panel->SetSizer(vSizer);
}

void MainFrame::SetupTabDatabase(wxPanel* panel) {
    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
    
    wxBoxSizer* toolSizer = new wxBoxSizer(wxHORIZONTAL);
    txtCari = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
    txtCari->SetHint("Keyword...");
    
    wxArrayString filterOpt;
    filterOpt.Add("Semua Data"); filterOpt.Add("LULUS"); filterOpt.Add("TIDAK LULUS");
    
    filterChoice = new wxChoice(panel, ID_FILTER_CHOICE, wxDefaultPosition, wxDefaultSize, filterOpt);
    filterChoice->SetSelection(0);

    btnCari = new wxButton(panel, ID_BTN_CARI, "Cari"); 
    btnSort = new wxButton(panel, ID_BTN_SORT, "Sort"); // Tambahan tombol Sort
    btnRefresh = new wxButton(panel, ID_BTN_REFRESH, "Reload");
    
    toolSizer->Add(txtCari, 0, wxRIGHT, 5);
    toolSizer->Add(filterChoice, 0, wxRIGHT, 5);
    toolSizer->Add(btnCari, 0, wxRIGHT, 5);
    toolSizer->Add(btnSort, 0, wxRIGHT, 5);
    toolSizer->Add(btnRefresh, 0);

    listData = new wxListCtrl(panel, ID_LIST_DATA, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_SUNKEN);
    listData->InsertColumn(0, "NIM", wxLIST_FORMAT_LEFT, 90);
    listData->InsertColumn(1, "Nama", wxLIST_FORMAT_LEFT, 180);
    listData->InsertColumn(2, "Jurusan", wxLIST_FORMAT_LEFT, 120);
    listData->InsertColumn(3, "Akhir", wxLIST_FORMAT_CENTER, 60);
    listData->InsertColumn(4, "Grade", wxLIST_FORMAT_CENTER, 50);
    listData->InsertColumn(5, "Status", wxLIST_FORMAT_CENTER, 90);

    wxBoxSizer* actionSizer = new wxBoxSizer(wxHORIZONTAL);
    btnEdit = new wxButton(panel, ID_BTN_EDIT, "Edit");
    btnHapus = new wxButton(panel, ID_BTN_HAPUS, "Hapus");
    btnHapus->SetForegroundColour(wxColor(220, 53, 69));
    
    actionSizer->Add(btnEdit, 0, wxRIGHT, 10); 
    actionSizer->Add(btnHapus, 0);

    vSizer->Add(toolSizer, 0, wxEXPAND | wxALL, 10);
    vSizer->Add(listData, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
    vSizer->Add(actionSizer, 0, wxALIGN_RIGHT | wxALL, 10);
    
    panel->SetSizer(vSizer);
}

void MainFrame::SetupTabStatistik(wxPanel* panel) {
    panel->SetBackgroundColour(wxColor(240, 242, 245));
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "Dashboard Performa");
    wxFont fTitle = title->GetFont(); fTitle.SetPointSize(16); fTitle.SetWeight(wxFONTWEIGHT_BOLD);
    title->SetFont(fTitle);
    mainSizer->Add(title, 0, wxALL | wxALIGN_LEFT, 20);

    wxBoxSizer* centerSizer = new wxBoxSizer(wxHORIZONTAL);
    
    // Kiri
    wxBoxSizer* leftInfo = new wxBoxSizer(wxVERTICAL);
    lblRataRata = new wxStaticText(panel, wxID_ANY, "0.0");
    wxFont fAvg = lblRataRata->GetFont(); fAvg.SetPointSize(48); fAvg.SetWeight(wxFONTWEIGHT_BOLD);
    lblRataRata->SetFont(fAvg);
    
    wxStaticText* subRata = new wxStaticText(panel, wxID_ANY, "Rata-Rata Kelas");
    
    leftInfo->Add(lblRataRata, 0, wxALIGN_CENTER | wxBOTTOM, 5);
    leftInfo->Add(subRata, 0, wxALIGN_CENTER);
    
    // Kanan (Grafik)
    panelChart = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxSize(400, 300));
    panelChart->SetBackgroundColour(wxColor(255, 255, 255)); 
    panelChart->SetWindowStyle(wxBORDER_SIMPLE); 
    panelChart->Bind(wxEVT_PAINT, &MainFrame::OnPaintStatistik, this);

    centerSizer->Add(leftInfo, 1, wxALIGN_CENTER | wxRIGHT, 40);
    centerSizer->Add(panelChart, 0, wxALIGN_CENTER);

    mainSizer->Add(centerSizer, 1, wxALIGN_CENTER | wxALL, 20);

    // Legenda
    wxBoxSizer* legendSizer = new wxBoxSizer(wxHORIZONTAL);
    lblLulus = new wxStaticText(panel, wxID_ANY, "Lulus: 0");
    lblLulus->SetForegroundColour(wxColor(46, 204, 113)); 
    lblGagal = new wxStaticText(panel, wxID_ANY, "Gagal: 0");
    lblGagal->SetForegroundColour(wxColor(231, 76, 60)); 

    legendSizer->Add(lblLulus, 0, wxRIGHT, 40);
    legendSizer->Add(lblGagal, 0);

    mainSizer->Add(legendSizer, 0, wxALIGN_CENTER | wxBOTTOM, 30);
    panel->SetSizer(mainSizer);
}

void MainFrame::SetupTabLog(wxPanel* panel) {
    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "LOG AKTIVITAS");
    wxFont font = title->GetFont(); font.SetWeight(wxFONTWEIGHT_BOLD); title->SetFont(font);

    listLog = new wxListCtrl(panel, ID_LIST_LOG, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxBORDER_SUNKEN);
    listLog->InsertColumn(0, "Waktu", wxLIST_FORMAT_LEFT, 150);
    listLog->InsertColumn(1, "Aktivitas", wxLIST_FORMAT_LEFT, 400);

    vSizer->Add(title, 0, wxALL, 10);
    vSizer->Add(listLog, 1, wxEXPAND | wxALL, 10);
    panel->SetSizer(vSizer);
}

// ================== PART 3: CORE LOGIC ==================

void MainFrame::RefreshTabel() {
    listData->DeleteAllItems();
    
    std::string key = std::string(txtCari->GetValue().mb_str());
    int filterIdx = filterChoice->GetSelection();
    std::string statusFilter = "SEMUA";
    if (filterIdx == 1) statusFilter = "LULUS";
    if (filterIdx == 2) statusFilter = "TIDAK LULUS";

    std::vector<Mahasiswa> all = db.getMahasiswaData(key, statusFilter);

    if (currentSortMode == 1) {
        // Sort by Nama (A-Z)
        std::sort(all.begin(), all.end(), [](const Mahasiswa& a, const Mahasiswa& b) {
            return a.getNama() < b.getNama();
        });
    }
    else if (currentSortMode == 2) {
        // Sort by Nilai (Tertinggi ke Terendah / Descending)
        std::sort(all.begin(), all.end(), [](const Mahasiswa& a, const Mahasiswa& b) {
            return a.getAkhir() > b.getAkhir();
        });
    }
    else if (currentSortMode == 3) {
        // Sort by Nilai (Terendah ke Tertinggi / Ascending)
        std::sort(all.begin(), all.end(), [](const Mahasiswa& a, const Mahasiswa& b) {
            return a.getAkhir() < b.getAkhir();
        });
    }

    for (size_t i = 0; i < all.size(); i++) {
        long idx = listData->InsertItem(i, all[i].getNim());
        listData->SetItem(idx, 1, all[i].getNama());
        listData->SetItem(idx, 2, all[i].getJurusan());
        listData->SetItem(idx, 3, wxString::Format("%.2f", all[i].getAkhir()));
        listData->SetItem(idx, 4, wxString(1, all[i].getGrade()));
        listData->SetItem(idx, 5, all[i].getStatus());
        
        if(all[i].getStatus() == "TIDAK LULUS") 
            listData->SetItemTextColour(idx, wxColor(200,0,0));
    }
}

void MainFrame::RefreshStatistik() {
    // Ambil semua data (tanpa filter)
    const std::vector<Mahasiswa>& all = db.getMahasiswaData(); 
    
    float totalNilai = 0;
    int lulus = 0;
    for (const auto& m : all) {
        totalNilai += m.getAkhir();
        if (m.getStatus() == "LULUS") lulus++;
    }
    int gagal = all.size() - lulus;
    float rata = (all.size() > 0) ? (totalNilai / all.size()) : 0;

    lblRataRata->SetLabel(wxString::Format("%.1f", rata));
    lblLulus->SetLabel(wxString::Format("Lulus: %d", lulus));
    lblGagal->SetLabel(wxString::Format("Gagal: %d", gagal));

    // Update layout label biar gak kepotong
    if(lblRataRata->GetParent()) lblRataRata->GetParent()->Layout();

    if(panelChart) {
        panelChart->Refresh();
        panelChart->Update();
    }
}

void MainFrame::RefreshLog() {
    if(!listLog) return;
    listLog->DeleteAllItems();
    std::vector<LogEntry> logs = db.getLogs(); 
    for(size_t i=0; i<logs.size(); i++) {
        long idx = listLog->InsertItem(i, logs[i].timestamp);
        listLog->SetItem(idx, 1, logs[i].activity);
    }
}

void MainFrame::OnSimpanClicked(wxCommandEvent& event) {
    std::string nim = std::string(txtNim->GetValue().mb_str());
    std::string nama = std::string(txtNama->GetValue().mb_str());
    std::string jur = std::string(txtJurusan->GetValue().mb_str());
    
    if (jur.empty()) jur = "-";

    double t, u, a;
    txtTugas->GetValue().ToDouble(&t); 
    txtUts->GetValue().ToDouble(&u); 
    txtUas->GetValue().ToDouble(&a);

    if (nim.empty() || nama.empty()) {
        wxMessageBox("NIM dan Nama wajib diisi!", "Error", wxOK | wxICON_ERROR); return;
    }
    
    if (db.isNIMExist(nim)) {
        wxMessageBox("NIM sudah terdaftar!", "Error", wxOK | wxICON_ERROR); return;
    }

    // Handle Foto
    std::string finalPhoto = "default.png";
    if (!currentPhotoFile.IsEmpty()) {
        wxString exePath = wxStandardPaths::Get().GetExecutablePath();
        wxString appPath = wxFileName(exePath).GetPath();
        wxString photosDir = appPath + "\\assets\\photos";

        if (!wxDirExists(photosDir)) wxFileName::Mkdir(photosDir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);

        wxImage img;
        { wxLogNull logNo; 
          if (img.LoadFile(currentPhotoFile)) {
              img.Rescale(150, 200, wxIMAGE_QUALITY_HIGH);
              wxString destPath = photosDir + "\\" + wxString(nim) + ".jpg";
              img.SetOption("quality", 90);
              if (img.SaveFile(destPath, wxBITMAP_TYPE_JPEG)) 
                  finalPhoto = "photos/" + nim + ".jpg";
          }
        }
    }

    Mahasiswa m(nim, nama);
    m.setJurusan(jur); 
    m.setNilai(t, u, a);
    m.setPhotoPath(finalPhoto);

    if (db.tambahMahasiswa(m)) {
        wxMessageBox("Data Berhasil Disimpan!", "Sukses", wxOK | wxICON_INFORMATION);
        
        // Reset Form & Refresh
        OnResetClicked(event);
        RefreshTabel();
        RefreshStatistik();
        RefreshLog();
    } else {
        wxMessageBox("Gagal menulis ke Database SQL.", "Database Error", wxOK | wxICON_ERROR);
    }
}

// ================== PART 4: OTHER EVENTS & LOGIC ==================

void MainFrame::OnResetClicked(wxCommandEvent& event) {
    // Reset Form Teks
    txtNim->Clear(); 
    txtNama->Clear(); 
    txtJurusan->Clear();
    txtTugas->SetValue("0"); 
    txtUts->SetValue("0"); 
    txtUas->SetValue("0");
    
    // Reset Variabel Foto
    currentPhotoFile = ""; 
    
    // Reset Tampilan Foto ke Default
    wxString pathDefault = "assets/default.png";
    wxImage img;
    { 
        wxLogNull logNo; // Bungkam error jika file tidak ada
        if(!img.LoadFile(pathDefault, wxBITMAP_TYPE_PNG)) {
            img.Create(150, 200); // Bikin kotak kosong kalau default hilang
            unsigned char* d = img.GetData();
            if(d) for(int i=0; i<150*200*3; i++) d[i]=200; 
        }
    }
    img.Rescale(150, 200, wxIMAGE_QUALITY_HIGH);
    imgProfil->SetBitmap(wxBitmap(img));
}

void MainFrame::OnHapusClicked(wxCommandEvent& event) {
    // 1. Hitung jumlah yang dipilih
    int selectedCount = listData->GetSelectedItemCount();
    
    if (selectedCount == 0) {
        wxMessageBox("Pilih minimal satu data di tabel!", "Info", wxOK | wxICON_INFORMATION);
        return;
    }

    // 2. Kumpulkan semua NIM yang dipilih
    std::vector<std::string> nimsToDelete;
    long item = -1;
    
    // Loop untuk mencari semua item yang statusnya SELECTED
    while ((item = listData->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != -1) {
        wxString nim = listData->GetItemText(item, 0); // Ambil kolom 0 (NIM)
        nimsToDelete.push_back(std::string(nim.mb_str()));
    }

    // 3. Konfirmasi Penghapusan Massal
    wxString msg;
    if (selectedCount == 1) {
        msg = "Yakin hapus mahasiswa dengan NIM " + nimsToDelete[0] + "?";
    } else {
        msg = wxString::Format("Yakin ingin menghapus %d data mahasiswa terpilih?\nTindakan ini tidak dapat dibatalkan.", selectedCount);
    }

    if (wxMessageBox(msg, "Konfirmasi Hapus Massal", wxYES_NO | wxICON_WARNING) == wxYES) {
        
        // Lepaskan gambar dari preview dulu (Unlock file)
        OnResetClicked(event);
        
        int successCount = 0;

        // 4. Eksekusi Hapus Berulang
        for (const auto& nim : nimsToDelete) {
            if (db.hapusMahasiswa(nim)) {
                successCount++;
            }
        }

        // 5. Refresh UI
        RefreshTabel();
        RefreshStatistik();
        RefreshLog();
        
        wxMessageBox(wxString::Format("Berhasil menghapus %d data.", successCount), "Sukses", wxOK | wxICON_INFORMATION);
    }
}

void MainFrame::OnEditClicked(wxCommandEvent& event) {
    long item = listData->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (item == -1) { wxMessageBox("Pilih data dulu!", "Info"); return; }
    
    std::string sNim = std::string(listData->GetItemText(item, 0).mb_str());
    Mahasiswa m = db.getMahasiswaByNIM(sNim);
    
    if (m.getNim() != "") {
        // Input Nama Baru
        wxString newNama = wxGetTextFromUser("Nama Baru:", "Edit Data", m.getNama());
        if (newNama.IsEmpty()) return; // Batal
        
        // Input Jurusan Baru
        wxString newJur = wxGetTextFromUser("Jurusan Baru:", "Edit Data", m.getJurusan());
        if (newJur.IsEmpty()) return;
        
        // Input Nilai
        wxString sT = wxGetTextFromUser("Nilai Tugas:", "Edit Nilai", wxString::Format("%.0f", m.getTugas()));
        if(sT.empty()) return; double t; sT.ToDouble(&t);
        
        wxString sU = wxGetTextFromUser("Nilai UTS:", "Edit Nilai", wxString::Format("%.0f", m.getUts()));
        if(sU.empty()) return; double u; sU.ToDouble(&u);
        
        wxString sA = wxGetTextFromUser("Nilai UAS:", "Edit Nilai", wxString::Format("%.0f", m.getUas()));
        if(sA.empty()) return; double a; sA.ToDouble(&a);

        // Simpan Perubahan
        db.editMahasiswa(sNim, std::string(newNama.mb_str()), std::string(newJur.mb_str()), (float)t, (float)u, (float)a);
        
        RefreshTabel(); 
        RefreshStatistik(); 
        RefreshLog();
        wxMessageBox("Data Berhasil Diperbarui!", "Sukses");
    }
}

// --- Event Simpel ---
void MainFrame::OnCariClicked(wxCommandEvent& event) { RefreshTabel(); }
void MainFrame::OnFilterSelected(wxCommandEvent& event) { RefreshTabel(); }
void MainFrame::OnRefreshClicked(wxCommandEvent& event) { 
    db.load(); // Reload dari DB (sebenarnya initTables/re-query di SQL)
    RefreshTabel(); 
    RefreshStatistik(); 
    RefreshLog(); 
}
void MainFrame::OnTabChanged(wxBookCtrlEvent& event) { 
    RefreshTabel(); 
    RefreshStatistik(); 
    RefreshLog(); 
    event.Skip(); 
}
void MainFrame::OnExit(wxCommandEvent& event) { Close(true); }

// --- Fitur Tambahan ---
void MainFrame::OnSortClicked(wxCommandEvent& event) {
    wxArrayString choices;
    choices.Add("NIM (Default)");                // Index 0
    choices.Add("Nama (A - Z)");                 // Index 1
    choices.Add("Nilai Akhir (Tertinggi - Terendah)"); // Index 2
    choices.Add("Nilai Akhir (Terendah - Tertinggi)"); // Index 3

    wxSingleChoiceDialog dialog(this, "Pilih metode pengurutan:", "Urutkan Data", choices);
    
    // Set pilihan yang sedang aktif biar user tau
    dialog.SetSelection(currentSortMode); 

    if (dialog.ShowModal() == wxID_OK) {
        currentSortMode = dialog.GetSelection(); // Simpan pilihan user
        
        // Refresh tabel untuk menerapkan sorting baru
        RefreshTabel(); 
        
        wxMessageBox("Tabel berhasil diurutkan!", "Info", wxOK | wxICON_INFORMATION);
    }
}

void MainFrame::OnImportClicked(wxCommandEvent& event) {
    wxFileDialog openDialog(this, "Import CSV", "", "", "CSV (*.csv)|*.csv", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openDialog.ShowModal() == wxID_CANCEL) return;
    
    std::ifstream file(openDialog.GetPath().ToStdString());
    if (!file.is_open()) return;

    std::string line; 
    std::getline(file, line); // Skip Header CSV
    
    int sukses = 0, gagal = 0;
    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string n, nm, jur, dum;
        
        // Parsing CSV: NIM,Nama,Jurusan,dst...
        // Sesuaikan dengan format Export
        std::getline(ss, n, ','); 
        std::getline(ss, nm, ',');
        std::getline(ss, jur, ',');
        
        // Skip duplikat
        if(db.isNIMExist(n)) { gagal++; continue; }
        
        try {
            // Default nilai 0 jika CSV tidak lengkap, foto default
            Mahasiswa m(n, nm);
            m.setJurusan(jur);
            m.setPhotoPath("default.png"); 
            db.tambahMahasiswa(m);
            sukses++;
        } catch(...) { gagal++; }
    }
    file.close();
    
    RefreshTabel(); 
    RefreshStatistik(); 
    RefreshLog();
    
    wxString msg = wxString::Format("Import Selesai.\nSukses: %d\nGagal/Duplikat: %d", sukses, gagal);
    wxMessageBox(msg, "Laporan Import", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnExportClicked(wxCommandEvent& event) {
    wxFileDialog openDialog(this, "Export Data", "", "Laporan_SQL.csv", 
                            "CSV (*.csv)|*.csv", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    
    if (openDialog.ShowModal() == wxID_CANCEL) return;
    
    std::ifstream file(openDialog.GetPath().ToStdString());
    if (!file.is_open()) return;

    std::string line; 
    std::getline(file, line); // Skip Header

    int sukses = 0, gagal = 0;

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        std::string n, nm, jur, sT, sU, sA, dum;
        
        // PARSING SESUAI URUTAN EXPORT BARU
        // Format: NIM, Nama, Jurusan, Tugas, UTS, UAS, ...
        if (!std::getline(ss, n, ',')) continue;   // NIM
        if (!std::getline(ss, nm, ',')) continue;  // Nama
        if (!std::getline(ss, jur, ',')) continue; // Jurusan (INI YANG DULU HILANG)
        
        std::getline(ss, sT, ','); // Tugas
        std::getline(ss, sU, ','); // UTS
        std::getline(ss, sA, ','); // UAS
        // Sisanya (Akhir/Grade/Status) kita abaikan, biar sistem hitung ulang validasinya

        // Cek Duplikat
        if (db.isNIMExist(n)) { gagal++; continue; } 
        
        try {
            Mahasiswa m(n, nm);
            m.setJurusan(jur); // Set Jurusan
            // Konversi nilai, default 0 jika kosong
            float t = sT.empty() ? 0 : stof(sT);
            float u = sU.empty() ? 0 : stof(sU);
            float a = sA.empty() ? 0 : stof(sA);
            
            m.setNilai(t, u, a);
            
            // Karena import CSV biasanya gak bawa file foto fisik, set default
            m.setPhotoPath("default.png"); 
            
            db.tambahMahasiswa(m);
            sukses++;
        } catch(...) { gagal++; }
    }
    file.close();
    
    RefreshTabel(); 
    RefreshStatistik(); 
    RefreshLog();
    
    wxString msg = wxString::Format("Import Selesai.\nSukses: %d\nGagal/Duplikat: %d", sukses, gagal);
    wxMessageBox(msg, "Laporan Import", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnUploadClicked(wxCommandEvent& event) {
    wxFileDialog openDialog(this, "Pilih Foto", "", "", 
                            "Images (*.jpg;*.png;*.jpeg)|*.jpg;*.png;*.jpeg", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    
    if (openDialog.ShowModal() == wxID_CANCEL) return;
    
    wxString path = openDialog.GetPath();
    wxImage img;
    
    if (img.LoadFile(path)) {
        img.Rescale(150, 200, wxIMAGE_QUALITY_HIGH);
        imgProfil->SetBitmap(wxBitmap(img));
        currentPhotoFile = path; // Simpan path sementara
    } else {
        wxMessageBox("Gagal memuat gambar.", "Error", wxOK | wxICON_ERROR);
    }
}

void MainFrame::OnTableDoubleClicked(wxListEvent& event) {
    long item = event.GetIndex();
    std::string nim = std::string(listData->GetItemText(item, 0).mb_str());
    
    Mahasiswa m = db.getMahasiswaByNIM(nim);
    
    // Tampilkan Detail Dialog
    DetailDialog dlg(this, m);
    dlg.ShowModal();
}

void MainFrame::OnGantiPasswordClicked(wxCommandEvent& event) {
    ChangePasswordDialog dlg(this); 
    dlg.ShowModal();
}

void MainFrame::OnAboutClicked(wxCommandEvent& event) {
    wxMessageBox("Sistem Nilai Mahasiswa v3.0 (SQL Enterprise)\n\n"
                 "Fitur: SQLite Database, Enkripsi, Audit Log, GUI Modern.\n"
                 "Developer: [NAMA KAMU]", "Tentang Aplikasi", wxOK | wxICON_INFORMATION);
}

// === LOGIKA MENGGAMBAR CHART (Sama seperti v3.0) ===
void MainFrame::OnPaintStatistik(wxPaintEvent& event) {
    wxPaintDC dc(panelChart);
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

    if (gc) {
        const std::vector<Mahasiswa>& all = db.getMahasiswaData();
        int total = all.size();
        int lulus = 0;
        for(const auto& m : all) { if(m.getStatus() == "LULUS") lulus++; }
        int gagal = total - lulus;

        double w, h; gc->GetSize(&w, &h);
        double cx = w / 2; double cy = h / 2;
        double radius = (std::min(w, h) / 2) - 20;
        double holeRadius = radius * 0.6;

        // Reset Background
        gc->SetBrush(wxBrush(wxColor(255, 255, 255)));
        gc->SetPen(*wxTRANSPARENT_PEN);
        gc->DrawRectangle(0, 0, w, h);

        // Jika Kosong
        if (total == 0) {
            gc->SetBrush(wxBrush(wxColor(236, 240, 241)));
            gc->DrawEllipse(cx - radius, cy - radius, radius * 2, radius * 2);
            
            // Bolong Tengah
            gc->SetBrush(wxBrush(wxColor(255, 255, 255)));
            gc->DrawEllipse(cx - holeRadius, cy - holeRadius, holeRadius * 2, holeRadius * 2);
            
            // Teks
            wxFont font = wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
            gc->SetFont(font, wxColor(189, 195, 199));
            double tw, th; gc->GetTextExtent("NO DATA", &tw, &th);
            gc->DrawText("NO DATA", cx - tw / 2, cy - th / 2);
            
            delete gc; return;
        }

        // Gambar Slice
        double startAngle = -M_PI / 2; 
        double sweepLulus = (2 * M_PI * lulus / total);
        double sweepGagal = (2 * M_PI * gagal / total);

        // Slice Lulus (Hijau)
        if (lulus > 0) {
            gc->SetBrush(wxBrush(wxColor(46, 204, 113))); 
            wxGraphicsPath path = gc->CreatePath();
            path.MoveToPoint(cx, cy);
            path.AddArc(cx, cy, radius, startAngle, startAngle + sweepLulus, true);
            path.CloseSubpath();
            gc->FillPath(path);
        }

        // Slice Gagal (Merah)
        if (gagal > 0) {
            gc->SetBrush(wxBrush(wxColor(231, 76, 60)));
            wxGraphicsPath path = gc->CreatePath();
            path.MoveToPoint(cx, cy);
            path.AddArc(cx, cy, radius, startAngle + sweepLulus, startAngle + sweepLulus + sweepGagal, true);
            path.CloseSubpath();
            gc->FillPath(path);
        }

        // Tutup Tengah (Donut)
        gc->SetBrush(wxBrush(wxColor(255, 255, 255)));
        gc->DrawEllipse(cx - holeRadius, cy - holeRadius, holeRadius * 2, holeRadius * 2);

        // Info Tengah
        wxFont fontBig = wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
        gc->SetFont(fontBig, wxColor(44, 62, 80)); 
        wxString strTotal = wxString::Format("%d", total);
        double tw, th; gc->GetTextExtent(strTotal, &tw, &th);
        gc->DrawText(strTotal, cx - tw / 2, cy - th * 0.8);

        wxFont fontSmall = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
        gc->SetFont(fontSmall, wxColor(127, 140, 141));
        gc->GetTextExtent("Mahasiswa", &tw, &th);
        gc->DrawText("Mahasiswa", cx - tw / 2, cy + th * 0.2);

        delete gc; 
    }
}