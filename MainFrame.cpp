#include "MainFrame.h"
#include "functions.h" 
#include <wx/wx.h>
#include <wx/file.h>      
#include <wx/filename.h>  
#include <wx/statline.h>
#include <wx/slider.h>    // <--- Required for the Slider
#include <string>

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(900, 750))
{
    // --- THEME COLORS ---
    wxColour darkBg("#1e272e");
    wxColour lightBg("#485460");
    wxColour textWhite(*wxWHITE);
    wxColour accentGreen("#05c46b");

    // Main Panel
    wxPanel* panel = new wxPanel(this);
    panel->SetBackgroundColour(darkBg);
    panel->SetForegroundColour(textWhite);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // --- FILE UPLOAD SECTION ---
    wxStaticText* labelTitle = new wxStaticText(panel, wxID_ANY, "DNA Sequence Input:");
    labelTitle->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    labelTitle->SetForegroundColour(textWhite);

    wxBoxSizer* fileSizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton* btnLoadFile = new wxButton(panel, wxID_ANY, "Load DNA File (.txt)");
    btnLoadFile->SetBackgroundColour(accentGreen);
    btnLoadFile->SetForegroundColour(wxColour("#000000"));

    lblStatus = new wxStaticText(panel, wxID_ANY, "No file loaded.");
    lblStatus->SetForegroundColour(textWhite);

    fileSizer->Add(btnLoadFile, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 15);
    fileSizer->Add(lblStatus, 1, wxALIGN_CENTER_VERTICAL);

    btnLoadFile->Bind(wxEVT_BUTTON, &MainFrame::OnLoadFile, this);

    // --- SEARCH INPUT SECTION ---
    wxBoxSizer* searchSizer = new wxBoxSizer(wxHORIZONTAL);

    searchInput = new wxTextCtrl(panel, wxID_ANY, "Enter pattern (e.g. TG)...");
    searchInput->SetBackgroundColour(lightBg);
    searchInput->SetForegroundColour(textWhite);

    wxButton* btnSearch = new wxButton(panel, wxID_ANY, "Search Pattern");
    btnSearch->Bind(wxEVT_BUTTON, &MainFrame::SearchPattern, this);

    searchSizer->Add(searchInput, 1, wxEXPAND | wxRIGHT, 10);
    searchSizer->Add(btnSearch, 0, wxALIGN_CENTER_VERTICAL);

    // --- GENERAL TOOLS (Common / Repeats) ---
    wxBoxSizer* toolsSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btnCommon = new wxButton(panel, wxID_ANY, "Find Common Region");
    wxButton* btnRepeat = new wxButton(panel, wxID_ANY, "Find Repeats");

    toolsSizer->Add(btnCommon, 1, wxRIGHT, 5);
    toolsSizer->Add(btnRepeat, 1, wxRIGHT, 5);

    // --- UNIQUE REGION SECTION (With Slider) ---
    // We put this in its own row so it looks clean
    wxStaticBoxSizer* uniqueGroup = new wxStaticBoxSizer(wxHORIZONTAL, panel, "Find Unique Region");
    uniqueGroup->GetStaticBox()->SetForegroundColour(textWhite);

    // 1. The Label showing value
    lblSliderVal = new wxStaticText(panel, wxID_ANY, "Length: 5");
    lblSliderVal->SetForegroundColour(textWhite);

    // 2. The Slider (Default 5, Range 1-50)
    sliderUnique = new wxSlider(panel, wxID_ANY, 5, 1, 50, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    sliderUnique->SetBackgroundColour(darkBg);
    sliderUnique->SetForegroundColour(textWhite);

    // Bind slider movement to update the label immediately
    sliderUnique->Bind(wxEVT_SLIDER, [this](wxCommandEvent&) {
        lblSliderVal->SetLabel("Length: " + std::to_string(sliderUnique->GetValue()));
        });

    // 3. The Button
    wxButton* btnUnique = new wxButton(panel, wxID_ANY, "Find Unique");
    btnUnique->Bind(wxEVT_BUTTON, &MainFrame::UniqueRegions, this);

    uniqueGroup->Add(lblSliderVal, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    uniqueGroup->Add(sliderUnique, 1, wxEXPAND | wxRIGHT, 10); // Slider takes space
    uniqueGroup->Add(btnUnique, 0, wxALIGN_CENTER_VERTICAL);

    // --- OUTPUT SECTION ---
    wxStaticText* labelRes = new wxStaticText(panel, wxID_ANY, "Results / Status:");
    labelRes->SetForegroundColour(textWhite);

    output = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    output->SetBackgroundColour(lightBg);
    output->SetForegroundColour(textWhite);
    output->SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    // --- LAYOUT ASSEMBLY ---
    vbox->Add(labelTitle, 0, wxALL, 10);
    vbox->Add(fileSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    wxStaticLine* line1 = new wxStaticLine(panel, wxID_ANY);
    vbox->Add(line1, 0, wxEXPAND | wxALL, 10);

    vbox->Add(searchSizer, 0, wxEXPAND | wxALL, 10);
    vbox->Add(toolsSizer, 0, wxEXPAND | wxALL, 10);

    // Add the new Slider Group
    vbox->Add(uniqueGroup, 0, wxEXPAND | wxALL, 10);

    vbox->Add(labelRes, 0, wxLEFT | wxTOP, 10);
    vbox->Add(output, 1, wxEXPAND | wxALL, 10);

    panel->SetSizer(vbox);
}

// --- FILE LOADING FUNCTION ---
void MainFrame::OnLoadFile(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, "Open DNA Text File", "", "",
        "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxString path = openFileDialog.GetPath();
    wxFile file;
    if (file.Open(path)) {
        wxString fileContent;
        file.ReadAll(&fileContent);

        // Clean Data
        std::string raw = fileContent.ToStdString();
        loadedSequence = "";
        for (char c : raw) {
            char upper = toupper(c);
            if (upper == 'A' || upper == 'C' || upper == 'G' || upper == 'T') {
                loadedSequence += upper;
            }
        }

        // Update UI
        wxFileName fileName(path);
        lblStatus->SetLabel("Loaded: " + fileName.GetFullName() + " (" + std::to_string(loadedSequence.length()) + " bases)");
        output->SetValue("File loaded successfully.\nReady to search.");

        // Optional: Update slider max range if the file is small
        if (loadedSequence.length() < 50 && loadedSequence.length() > 0) {
            sliderUnique->SetMax(loadedSequence.length());
        }
        else {
            sliderUnique->SetMax(50); // Reset to default max
        }
    }
    else {
        wxMessageBox("Cannot open file", "Error", wxICON_ERROR);
    }
}

// --- SEARCH FUNCTION ---
void MainFrame::SearchPattern(wxCommandEvent& event)
{
    if (loadedSequence.empty()) {
        output->SetValue("Error: No DNA file loaded. Please load a file first.");
        return;
    }

    std::string rawPattern = searchInput->GetValue().ToStdString();
    std::string pattern = "";
    for (char c : rawPattern) {
        char upper = toupper(c);
        if (upper == 'A' || upper == 'C' || upper == 'G' || upper == 'T') pattern += upper;
    }

    if (pattern.empty()) {
        output->SetValue("Error: Search pattern is empty.");
        return;
    }

    int count = 0;
    int* indices = DNA::SearchPattern(loadedSequence, pattern, count);

    if (indices == nullptr || count == 0) {
        output->SetValue("Pattern '" + pattern + "' NOT found.");
    }
    else {
        wxString resultMsg;
        resultMsg << "Pattern '" << pattern << "' found " << count << " times at indices:\n";

        int limit = (count > 5000) ? 5000 : count;
        for (int i = 0; i < limit; i++) {
            resultMsg << indices[i] << ", ";
        }

        if (count > limit) resultMsg << "... (truncated)";
        else if (resultMsg.EndsWith(", ")) resultMsg.RemoveLast(2);

        output->SetValue(resultMsg);
        delete[] indices;
    }
}

// --- UNIQUE REGIONS FUNCTION (Using Slider) ---
void MainFrame::UniqueRegions(wxCommandEvent& event)
{
    if (loadedSequence.empty()) {
        output->SetValue("Error: No DNA file loaded. Please load a file first.");
        return;
    }

    // 1. Get value directly from Slider
    int x = sliderUnique->GetValue();

    // 2. Call Backend
    std::string* res = DNA::findUniqueRegion(loadedSequence, x);

    if (!res) {
        output->SetValue("Error: Backend returned null.");
        return;
    }

    // 3. Display Result
    if (res[0].empty()) {
        output->SetValue("No unique region of length " + std::to_string(x) + " found.");
    }
    else {
        output->SetValue("Found Unique Region (Length " + std::to_string(x) + "):\n" + res[0]);
    }

    // 4. Cleanup
    delete[] res;
}