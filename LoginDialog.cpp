#include "LoginDialog.h"
#include "mycrypto.h" 
#include <fstream>
#include <string>
#include <wx/msgdlg.h>
#include <wx/statline.h>
#include <cstdio> // Penting untuk remove file

// === DAFTAR EVENT ===
wxBEGIN_EVENT_TABLE(LoginDialog, wxDialog)
    EVT_BUTTON(wxID_OK, LoginDialog::OnLoginClicked)
    EVT_BUTTON(wxID_CANCEL, LoginDialog::OnCancelClicked)
    EVT_HYPERLINK(ID_LINK_FORGOT, LoginDialog::OnForgotClicked) 
wxEND_EVENT_TABLE()

// === CONSTRUCTOR ===
LoginDialog::LoginDialog(const wxString& title)
    : wxDialog(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 350)) {
    
    // Cek File Config
    std::ifstream checkFile("admin_config.ini");
    isRegistrationMode = !checkFile.good(); 
    checkFile.close();

    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);
    
    // Judul
    wxString judulStr = isRegistrationMode ? "SETUP ADMIN BARU" : "LOGIN SYSTEM";
    wxStaticText* lblTitle = new wxStaticText(panel, wxID_ANY, judulStr);
    wxFont font = lblTitle->GetFont(); font.SetWeight(wxFONTWEIGHT_BOLD); font.SetPointSize(14);
    lblTitle->SetFont(font);

    // Grid Form
    wxFlexGridSizer* grid = new wxFlexGridSizer(2, 5, 10);

    // User (Tambah wxTE_PROCESS_ENTER)
    grid->Add(new wxStaticText(panel, wxID_ANY, "Username:"), 0, wxALIGN_CENTER_VERTICAL);
    txtUser = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), wxTE_PROCESS_ENTER);
    grid->Add(txtUser);

    // Pass (Tambah wxTE_PROCESS_ENTER + wxTE_PASSWORD)
    grid->Add(new wxStaticText(panel, wxID_ANY, "Password:"), 0, wxALIGN_CENTER_VERTICAL);
    txtPass = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), wxTE_PASSWORD | wxTE_PROCESS_ENTER);
    grid->Add(txtPass);

    // === NAVIGASI ENTER YANG KAMU MINTA ===
    
    // 1. Di Username tekan Enter -> Pindah fokus ke Password
    txtUser->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent&){ 
        txtPass->SetFocus(); 
    });

    // 2. Di Password tekan Enter -> Langsung LOGIN (Panggil fungsi tombol)
    txtPass->Bind(wxEVT_TEXT_ENTER, &LoginDialog::OnLoginClicked, this);

    // ======================================

    // Link Lupa Password
    wxHyperlinkCtrl* lnkForgot = nullptr;
    if (!isRegistrationMode) {
        lnkForgot = new wxHyperlinkCtrl(panel, ID_LINK_FORGOT, "Lupa Password?", "");
    }

    // Info Teks
    wxStaticText* lblInfo = new wxStaticText(panel, wxID_ANY, "");
    if (isRegistrationMode) {
        lblInfo->SetLabel("Selamat Datang!\nSilakan buat akun Admin baru.");
        lblInfo->SetForegroundColour(wxColor(0, 100, 0));
    } else {
        lblInfo->SetLabel("Silakan login untuk mengakses database.");
    }

    // Tombol (Set tombol Login sebagai Default biar ada outline tebal)
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btnAction = new wxButton(panel, wxID_OK, isRegistrationMode ? "Buat Akun" : "Login");
    wxButton* btnCancel = new wxButton(panel, wxID_CANCEL, "Batal");
    
    // Jadikan tombol ini default (bisa ditekan enter global)
    btnAction->SetDefault(); 

    btnSizer->Add(btnAction, 0, wxRIGHT, 10);
    btnSizer->Add(btnCancel, 0);

    // Susun Layout
    vSizer->Add(lblTitle, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 20);
    vSizer->Add(grid, 0, wxALIGN_CENTER | wxBOTTOM, 5);
    
    if (lnkForgot) {
        vSizer->Add(lnkForgot, 0, wxALIGN_RIGHT | wxRIGHT, 50); 
    }
    
    vSizer->Add(lblInfo, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 15);
    vSizer->Add(btnSizer, 0, wxALIGN_CENTER | wxBOTTOM, 20);

    panel->SetSizer(vSizer);
    Center();
}

// Helper Enkripsi
std::string EncryptWx(wxString str) {
    return encryptDecrypt(std::string(str.mb_str()));
}

// === FUNGSI LOGIN / REGISTER ===
void LoginDialog::OnLoginClicked(wxCommandEvent& event) {
    wxString user = txtUser->GetValue();
    wxString pass = txtPass->GetValue();

    if (user.IsEmpty() || pass.IsEmpty()) {
        wxMessageBox("Username/Password wajib diisi!", "Warning", wxOK | wxICON_WARNING);
        return;
    }

    if (isRegistrationMode) {
        wxTextEntryDialog pinDlg(this, "Buat PIN Pemulihan (4-6 Digit):", "Security Setup");
        if (pinDlg.ShowModal() != wxID_OK) return;
        wxString pin = pinDlg.GetValue();

        if (pin.IsEmpty()) {
            wxMessageBox("PIN tidak boleh kosong!", "Error", wxOK | wxICON_ERROR); return;
        }

        std::ofstream file("admin_config.ini");
        file << EncryptWx(user) << std::endl;
        file << EncryptWx(pass) << std::endl;
        file << EncryptWx(pin) << std::endl;
        file.close();

        wxMessageBox("Akun Berhasil Dibuat!", "Sukses", wxOK | wxICON_INFORMATION);
        EndModal(wxID_OK);
    }
    else {
        std::ifstream file("admin_config.ini");
        std::string encU, encP, dummy;
        std::getline(file, encU);
        std::getline(file, encP);
        file.close();

        std::string realUser = encryptDecrypt(encU);
        std::string realPass = encryptDecrypt(encP);

        if (std::string(user.mb_str()) == realUser && std::string(pass.mb_str()) == realPass) {
            EndModal(wxID_OK);
        } else {
            wxMessageBox("Username atau Password Salah!", "Gagal", wxOK | wxICON_ERROR);
            txtPass->Clear();
            txtPass->SetFocus();
        }
    }
}

// === FUNGSI LUPA PASSWORD (RESET) ===
void LoginDialog::OnForgotClicked(wxHyperlinkEvent& event) {
    std::ifstream file("admin_config.ini");
    std::string encU, encP, encR;
    std::getline(file, encU);
    std::getline(file, encP);
    std::getline(file, encR);
    file.close();
    
    std::string realRec = encryptDecrypt(encR);
    std::string realPass = encryptDecrypt(encP);

    wxTextEntryDialog pinCheck(this, 
        "Masukkan PIN Pemulihan Anda.\n(Atau ketik 'RESET' jika lupa segalanya)", 
        "Lupa Password");
    
    if (pinCheck.ShowModal() == wxID_OK) {
        std::string input = std::string(pinCheck.GetValue().mb_str());

        if (input == realRec) {
            wxMessageBox("PIN Benar!\nPassword Anda adalah: " + realPass, 
                         "Info Akun", wxOK | wxICON_INFORMATION);
        } 
        else if (input == "RESET" || input == "reset") {
            int confirm = wxMessageBox(
                "PERINGATAN: Ini akan MENGHAPUS akun Admin.\n"
                "Data Mahasiswa AMAN.\n\nYakin reset?", 
                "Factory Reset", wxYES_NO | wxICON_WARNING);

            if (confirm == wxYES) {
                std::remove("admin_config.ini"); // Hapus file config
                wxMessageBox("Akun direset. Aplikasi akan restart.", "Info");
                EndModal(wxID_CANCEL);
                exit(0); // Matikan aplikasi
            }
        }
        else {
            wxMessageBox("PIN Salah!", "Error", wxOK | wxICON_ERROR);
        }
    }
}

// === FUNGSI BATAL (INI YANG TADI HILANG) ===
void LoginDialog::OnCancelClicked(wxCommandEvent& event) {
    EndModal(wxID_CANCEL);
}