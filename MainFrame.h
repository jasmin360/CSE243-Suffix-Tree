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
    wxStaticText* lblStatusA;          // Status label for file A
    wxStaticText* lblStatusB;          // Status label for file B

    // Target Sequence Selection
    wxRadioButton* radioSeqA;          // Radio button for Seq A
    wxRadioButton* radioSeqB;          // Radio button for Seq B

    // Search Pattern
    wxTextCtrl* searchInput;           // Search input field

    // Unique Region components
    wxStaticText* lblSliderUnique;     // Slider label for unique region
    wxSlider* sliderUnique;            // Slider for unique region length

    // Max Repetition components
    wxStaticText* lblSliderMaxRep;     // Slider label for max repetition
    wxSlider* sliderMaxRep;            // Slider for max repetition pattern length

    // Output
    wxTextCtrl* output;                // Output text area

    // Data
    std::string loadedSequenceA;       // DNA sequence A
    std::string loadedSequenceB;       // DNA sequence B
};