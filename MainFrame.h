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

    // New Slider Elements
    wxSlider* sliderUnique;      // The slider to pick 'X'
    wxStaticText* lblSliderVal;  // Label to show current slider value (e.g. "Len: 10")

    // Data Storage
    std::string loadedSequence;  // Stores the DNA from the file

    // Functions
    void OnLoadFile(wxCommandEvent& event);
    void SearchPattern(wxCommandEvent& event);

    // Button handlers
    void CommonRegion(wxCommandEvent& event) {}
    void FindRepeats(wxCommandEvent& event) {}
    void UniqueRegions(wxCommandEvent& event); // Logic is now in .cpp
};