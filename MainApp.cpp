#include <wx/wx.h>
#include "MainFrame.h"
#include "LoginDialog.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    wxInitAllImageHandlers();
    LoginDialog login("Login Admin Sistem Nilai");
    
    if (login.ShowModal() == wxID_OK) {
        MainFrame *frame = new MainFrame("Sistem Nilai Mahasiswa v3.0 (Administrator)");
        frame->Show(true);
        return true;
   } else {
        // 3. Kalau Batal/Silang, Matikan Aplikasi
        return false; 
    }
}