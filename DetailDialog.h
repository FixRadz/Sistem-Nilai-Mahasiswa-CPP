#ifndef DETAILDIALOG_H
#define DETAILDIALOG_H

#include <wx/wx.h>
#include "Mahasiswa.h" // Kita butuh akses ke objek Mahasiswa

class DetailDialog : public wxDialog {
public:
    // Constructor terima objek Mahasiswa
    DetailDialog(wxWindow* parent, const Mahasiswa& mhs);

private:
    Mahasiswa currentMhs;
    void OnCloseClicked(wxCommandEvent& event);
    void OnPrintClicked(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_BTN_CLOSE = 100,
    ID_BTN_PRINT // ID Baru
};

#endif