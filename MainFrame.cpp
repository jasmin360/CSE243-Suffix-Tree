#include "MainFrame.h"
#include "functions.h" 
#include <wx/wx.h>
#include <wx/file.h>      // Required for file operations
#include <wx/filename.h>  // Required for extracting filenames
#include <wx/statline.h>
#include <string>

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(900, 700))
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
    btnLoadFile->SetForegroundColour(wxColour("#000000")); // Black text on green button

    lblStatus = new wxStaticText(panel, wxID_ANY, "No file loaded.");
    lblStatus->SetForegroundColour(textWhite);

    fileSizer->Add(btnLoadFile, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 15);
    fileSizer->Add(lblStatus, 1, wxALIGN_CENTER_VERTICAL);

    // Bind Load Button
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

    // --- OTHER TOOLS ---
    wxBoxSizer* toolsSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btnCommon = new wxButton(panel, wxID_ANY, "Find Common Region");
    wxButton* btnRepeat = new wxButton(panel, wxID_ANY, "Find Repeats");
    wxButton* btnUnique = new wxButton(panel, wxID_ANY, "Find Unique Regions");

    toolsSizer->Add(btnCommon, 1, wxRIGHT, 5);
    toolsSizer->Add(btnRepeat, 1, wxRIGHT, 5);
    toolsSizer->Add(btnUnique, 1);

    // --- OUTPUT SECTION ---
    wxStaticText* labelRes = new wxStaticText(panel, wxID_ANY, "Results / Status:");
    labelRes->SetForegroundColour(textWhite);

    output = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    output->SetBackgroundColour(lightBg);
    output->SetForegroundColour(textWhite);
    output->SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)); // Monospace font

    // --- LAYOUT ASSEMBLY ---
    vbox->Add(labelTitle, 0, wxALL, 10);
    vbox->Add(fileSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Visual Separator line
    wxStaticLine* line1 = new wxStaticLine(panel, wxID_ANY);
    vbox->Add(line1, 0, wxEXPAND | wxALL, 10);

    vbox->Add(searchSizer, 0, wxEXPAND | wxALL, 10);
    vbox->Add(toolsSizer, 0, wxEXPAND | wxALL, 10);

    vbox->Add(labelRes, 0, wxLEFT | wxTOP, 10);
    vbox->Add(output, 1, wxEXPAND | wxALL, 10);

    panel->SetSizer(vbox);
}

// --- FILE LOADING FUNCTION ---
void MainFrame::OnLoadFile(wxCommandEvent& event)
{
    // Open File Dialog
    wxFileDialog openFileDialog(this, "Open DNA Text File", "", "",
        "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return; // User cancelled

    // Load file content
    wxString path = openFileDialog.GetPath();
    wxFile file;
    if (file.Open(path)) {
        wxString fileContent;
        file.ReadAll(&fileContent);

        // --- CLEAN DATA IMMEDIATELY ---
        // Convert to std::string for processing
        std::string raw = fileContent.ToStdString();
        loadedSequence = ""; // Clear old data

        for (char c : raw) {
            char upper = toupper(c);
            // Only keep valid DNA bases
            if (upper == 'A' || upper == 'C' || upper == 'G' || upper == 'T') {
                loadedSequence += upper;
            }
        }

        // Update UI
        wxFileName fileName(path);
        lblStatus->SetLabel("Loaded: " + fileName.GetFullName() + " (" + std::to_string(loadedSequence.length()) + " bases)");
        output->SetValue("File loaded successfully.\nReady to search.");
    }
    else {
        wxMessageBox("Cannot open file", "Error", wxICON_ERROR);
    }
}

// --- SEARCH FUNCTION ---
void MainFrame::SearchPattern(wxCommandEvent& event)
{
    // 1. Check if file is loaded
    if (loadedSequence.empty()) {
        output->SetValue("Error: No DNA file loaded. Please load a file first.");
        return;
    }

    // 2. Get and Clean Pattern
    std::string rawPattern = searchInput->GetValue().ToStdString();
    std::string pattern = "";

    for (char c : rawPattern) {
        char upper = toupper(c);
        if (upper == 'A' || upper == 'C' || upper == 'G' || upper == 'T') {
            pattern += upper;
        }
    }

    if (pattern.empty()) {
        output->SetValue("Error: Search pattern is empty.");
        return;
    }

    // 3. Call Backend
    int count = 0;
    int* indices = DNA::SearchPattern(loadedSequence, pattern, count);

    // 4. Display Results
    if (indices == nullptr || count == 0) {
        output->SetValue("Pattern '" + pattern + "' NOT found in the sequence.");
    }
    else {
        wxString resultMsg;
        resultMsg << "Pattern '" << pattern << "' found " << count << " times at indices:\n";

        // If there are too many results, don't crash the UI text box
        int limit = (count > 5000) ? 5000 : count;

        for (int i = 0; i < limit; i++) {
            resultMsg << indices[i] << ", ";
        }

        if (count > limit) {
            resultMsg << "... (output truncated)";
        }
        else if (resultMsg.EndsWith(", ")) {
            resultMsg.RemoveLast(2);
        }

        output->SetValue(resultMsg);
        delete[] indices;
    }
}