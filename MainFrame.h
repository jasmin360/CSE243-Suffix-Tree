#pragma once
#include <wx/wx.h>
#include <wx/slider.h>
#include <string>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

private:
    // File loading handlers
    void OnLoadFileA(wxCommandEvent& event);
    void OnLoadFileB(wxCommandEvent& event);

    // Function handlers
    void SearchPattern(wxCommandEvent& event);
    void UniqueRegions(wxCommandEvent& event);
	void findCommonRegion(wxCommandEvent& event);
	void MaxRepetition(wxCommandEvent& event);

    // UI Components
    wxStaticText* lblStatusA;      // Status label for file A
    wxStaticText* lblStatusB;      // Status label for file B
    wxStaticText* lblSliderVal;    // Slider value label
    wxTextCtrl* searchInput;       // Search input field
    wxTextCtrl* output;            // Output text area
    wxSlider* sliderUnique;        // Slider for unique region length
    wxRadioButton* radioSeqA;
    wxRadioButton* radioSeqB;

    // Data
    std::string loadedSequenceA;   // DNA sequence A
    std::string loadedSequenceB;   // DNA sequence B
};