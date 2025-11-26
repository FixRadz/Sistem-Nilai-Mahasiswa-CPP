#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <wx/wx.h>
#include <wx/hyperlink.h>

class LoginDialog : public wxDialog {
public:
    LoginDialog(const wxString& title);
    
    wxString GetUsername() const;

private:
    wxTextCtrl* txtUser;
    wxTextCtrl* txtPass;
    
    void OnLoginClicked(wxCommandEvent& event);
    void OnCancelClicked(wxCommandEvent& event);
    bool isRegistrationMode;
    void OnForgotClicked(wxHyperlinkEvent& event);
    bool ValidateLogin(wxString user, wxString pass);

    wxDECLARE_EVENT_TABLE();
};

enum {
    ID_LINK_FORGOT = 2001
};

#endif