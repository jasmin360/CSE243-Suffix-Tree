#pragma once
#include <wx/wx.h>
#include <string>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

private:
    // GUI Elements
    wxStaticText* lblStatus;     // Shows "File Loaded: filename.txt"
    wxTextCtrl* output;          // Results box
    wxTextCtrl* searchInput;     // Pattern input

    // Data Storage
    std::string loadedSequence;  // Stores the DNA from the file

    // Functions
    void OnLoadFile(wxCommandEvent& event);
    void SearchPattern(wxCommandEvent& event);

    // Placeholder functions for buttons you haven't implemented yet
    void CommonRegion(wxCommandEvent& event) {}
    void FindRepeats(wxCommandEvent& event) {}
    void UniqueRegions(wxCommandEvent& event) {}
};