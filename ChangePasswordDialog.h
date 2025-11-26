#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <wx/wx.h>

class ChangePasswordDialog : public wxDialog {
public:
    ChangePasswordDialog(wxWindow* parent);

private:
    // Komponen Input
    wxTextCtrl* txtOldPass;
    wxTextCtrl* txtNewUser;
    wxTextCtrl* txtNewPass;
    wxTextCtrl* txtConfirm;
    wxTextCtrl* txtNewPin;

    // Fungsi Event
    void OnSimpanClicked(wxCommandEvent& event);
    void OnBatalClicked(wxCommandEvent& event);

    // Helper Cek Password Lama
    bool CheckOldPassword(wxString inputPass);

    wxDECLARE_EVENT_TABLE();
};

#endif