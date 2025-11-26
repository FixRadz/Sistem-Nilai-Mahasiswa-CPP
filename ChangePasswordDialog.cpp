#include "ChangePasswordDialog.h"
#include "mycrypto.h"
#include <fstream>
#include <string>
#include <wx/msgdlg.h>
#include <wx/statline.h>

wxBEGIN_EVENT_TABLE(ChangePasswordDialog, wxDialog)
    EVT_BUTTON(wxID_SAVE, ChangePasswordDialog::OnSimpanClicked)
    EVT_BUTTON(wxID_CANCEL, ChangePasswordDialog::OnBatalClicked)
wxEND_EVENT_TABLE()

ChangePasswordDialog::ChangePasswordDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Kelola Akun Admin", wxDefaultPosition, wxSize(400, 400)) {
    
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* vSizer = new wxBoxSizer(wxVERTICAL);

    // Judul
    wxStaticText* title = new wxStaticText(panel, wxID_ANY, "GANTI KREDENSIAL");
    wxFont font = title->GetFont(); font.SetWeight(wxFONTWEIGHT_BOLD); font.SetPointSize(12);
    title->SetFont(font);
    
    // Grid Form
    wxFlexGridSizer* grid = new wxFlexGridSizer(2, 5, 10);

    // 1. Verifikasi Lama
    grid->Add(new wxStaticText(panel, wxID_ANY, "Password Lama:"), 0, wxALIGN_CENTER_VERTICAL);
    txtOldPass = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), wxTE_PASSWORD);
    grid->Add(txtOldPass);

    // Spacer (Garis batas)
    grid->Add(new wxStaticLine(panel, wxID_ANY), 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    grid->Add(new wxStaticLine(panel, wxID_ANY), 0, wxEXPAND | wxTOP | wxBOTTOM, 10);

    // 2. Data Baru
    grid->Add(new wxStaticText(panel, wxID_ANY, "Username Baru:"), 0, wxALIGN_CENTER_VERTICAL);
    txtNewUser = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    grid->Add(txtNewUser);

    grid->Add(new wxStaticText(panel, wxID_ANY, "Password Baru:"), 0, wxALIGN_CENTER_VERTICAL);
    txtNewPass = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), wxTE_PASSWORD);
    grid->Add(txtNewPass);

    grid->Add(new wxStaticText(panel, wxID_ANY, "Konfirmasi:"), 0, wxALIGN_CENTER_VERTICAL);
    txtConfirm = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1), wxTE_PASSWORD);
    grid->Add(txtConfirm);

    grid->Add(new wxStaticText(panel, wxID_ANY, "PIN Recovery Baru:"), 0, wxALIGN_CENTER_VERTICAL);
    txtNewPin = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(200, -1));
    grid->Add(txtNewPin);

    // Tombol
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btnSimpan = new wxButton(panel, wxID_SAVE, "Simpan Perubahan");
    wxButton* btnBatal = new wxButton(panel, wxID_CANCEL, "Batal");
    
    btnSizer->Add(btnSimpan, 0, wxRIGHT, 10);
    btnSizer->Add(btnBatal, 0);

    // Layouting
    vSizer->Add(title, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 20);
    vSizer->Add(grid, 0, wxALIGN_CENTER | wxBOTTOM, 20);
    vSizer->Add(btnSizer, 0, wxALIGN_CENTER | wxBOTTOM, 20);

    panel->SetSizer(vSizer);
    Center();

    // Auto-fill username lama (Opsional, baca file config)
    std::ifstream file("admin_config.ini");
    if(file.is_open()) {
        std::string encU; std::getline(file, encU);
        txtNewUser->SetValue(wxString(encryptDecrypt(encU))); // Tampilin user lama
        file.close();
    }
}

// Logika Cek Password Lama
bool ChangePasswordDialog::CheckOldPassword(wxString inputPass) {
    std::ifstream file("admin_config.ini");
    if (!file.is_open()) return false; // Error file

    std::string dummy, encP;
    std::getline(file, dummy); // Skip username
    std::getline(file, encP);  // Ambil password
    file.close();

    // Bandingkan input user dengan password asli (dekripsi)
    std::string realPass = encryptDecrypt(encP);
    return (inputPass == realPass);
}

void ChangePasswordDialog::OnSimpanClicked(wxCommandEvent& event) {
    // 1. Validasi Password Lama
    if (!CheckOldPassword(txtOldPass->GetValue())) {
        wxMessageBox("Password Lama Salah!", "Akses Ditolak", wxOK | wxICON_ERROR);
        return;
    }

    // 2. Validasi Password Baru
    wxString newUser = txtNewUser->GetValue();
    wxString newPass = txtNewPass->GetValue();
    wxString confirm = txtConfirm->GetValue();
    wxString newPin  = txtNewPin->GetValue();

    if (newUser.IsEmpty() || newPass.IsEmpty() || newPin.IsEmpty()) {
        wxMessageBox("Semua kolom wajib diisi!", "Error", wxOK | wxICON_WARNING);
        return;
    }

    if (newPass != confirm) {
        wxMessageBox("Konfirmasi Password tidak cocok!", "Error", wxOK | wxICON_WARNING);
        return;
    }

    // 3. Simpan Data Baru (Enkripsi)
    std::ofstream file("admin_config.ini");
    if (file.is_open()) {
        file << encryptDecrypt(std::string(newUser.mb_str())) << std::endl;
        file << encryptDecrypt(std::string(newPass.mb_str())) << std::endl;
        file << encryptDecrypt(std::string(newPin.mb_str())) << std::endl;
        file.close();
        
        wxMessageBox("Akun Admin Berhasil Diperbarui!\nSilakan login ulang nanti.", "Sukses", wxOK | wxICON_INFORMATION);
        EndModal(wxID_OK);
    } else {
        wxMessageBox("Gagal menulis ke file config!", "Fatal Error", wxOK | wxICON_ERROR);
    }
}

void ChangePasswordDialog::OnBatalClicked(wxCommandEvent& event) {
    EndModal(wxID_CANCEL);
}