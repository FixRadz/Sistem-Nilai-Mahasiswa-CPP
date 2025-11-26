#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include "Database.h"
#include "DetailDialog.h"

// HAPUS 'using namespace std;' DARI SINI JUGA!

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    DatabaseManager db; 

    wxNotebook* tabSystem;

    wxTextCtrl* txtNim;
    wxTextCtrl* txtNama;
    wxTextCtrl* txtJurusan;
    wxTextCtrl* txtTugas;
    wxTextCtrl* txtUts;
    wxTextCtrl* txtUas;
    wxButton* btnSimpan;
    wxButton* btnReset;
    wxPanel* panelChart;
    wxPanel* panelLog;
    wxStaticBitmap* imgProfil;
    wxString currentPhotoFile;

    wxListCtrl* listData;
    wxTextCtrl* txtCari;
    wxButton* btnCari;
    wxButton* btnRefresh;
    wxButton* btnHapus; 
    wxButton* btnEdit;
    wxButton* btnSort;  

    wxStaticText* lblRataRata;
    wxStaticText* lblLulus;
    wxStaticText* lblGagal;
    wxGauge* gaugeLulus; 
    wxChoice* filterChoice;
    wxListCtrl* listLog;

    int currentSortMode;

    void SetupTabInput(wxPanel* panel);
    void SetupTabDatabase(wxPanel* panel);
    void SetupTabStatistik(wxPanel* panel);
    void RefreshTabel();
    void RefreshStatistik();
    void SetupMenuBar();
    void SetupTabLog(wxPanel* panel);
    void RefreshLog();

    void OnSimpanClicked(wxCommandEvent& event);
    void OnResetClicked(wxCommandEvent& event);
    void OnHapusClicked(wxCommandEvent& event);
    void OnEditClicked(wxCommandEvent& event); 
    void OnCariClicked(wxCommandEvent& event);
    void OnRefreshClicked(wxCommandEvent& event);
    void OnExportClicked(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAboutClicked(wxCommandEvent& event);
    void OnGantiPasswordClicked(wxCommandEvent& event);
    void OnTabChanged(wxBookCtrlEvent& event);
    void OnSortClicked(wxCommandEvent& event);
    void OnImportClicked(wxCommandEvent& event);
    void OnPaintStatistik(wxPaintEvent& event);
    void OnUploadClicked(wxCommandEvent& event);
    void OnTableSelected(wxListEvent& event);
    void OnTableDoubleClicked(wxListEvent& event);
    void OnFilterSelected(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_BTN_SIMPAN = 1001,
    ID_MENU_GANTI_PASS = 2001,
    ID_BTN_RESET,
    ID_BTN_CARI,
    ID_BTN_HAPUS,
    ID_BTN_EDIT,
    ID_BTN_REFRESH,
    ID_LIST_DATA,
    ID_LIST_LOG,
    ID_NOTEBOOK,
    ID_MENU_EXPORT,
    ID_MENU_KELUAR,
    ID_MENU_ABOUT,
    ID_BTN_UPLOAD,
    ID_BTN_SORT = 3001,
    ID_MENU_IMPORT,
    ID_FILTER_CHOICE = 4001
};

#endif