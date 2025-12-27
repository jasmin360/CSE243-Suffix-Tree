#include "MainFrame.h"
#include "functions.h"  // <--- CRITICAL: You must include this to use DNA::SearchPattern
#include <wx/wx.h>
#include <string>

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(900, 700))
{
    // --- THEME COLORS ---
    wxColour darkBg("#1e272e");
    wxColour lightBg("#485460");
    wxColour textWhite(*wxWHITE);

    // Main Panel
    wxPanel* panel = new wxPanel(this);
    panel->SetBackgroundColour(darkBg);
    panel->SetForegroundColour(textWhite);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // --- INPUT SECTION ---

    // Sequence A
    wxStaticText* labelA = new wxStaticText(panel, wxID_ANY, "DNA Sequence A:");
    labelA->SetForegroundColour(textWhite);

    seqA = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    seqA->SetBackgroundColour(lightBg);
    seqA->SetForegroundColour(textWhite);

    // Sequence B
    wxStaticText* labelB = new wxStaticText(panel, wxID_ANY, "DNA Sequence B:");
    labelB->SetForegroundColour(textWhite);

    seqB = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    seqB->SetBackgroundColour(lightBg);
    seqB->SetForegroundColour(textWhite);

    // --- BUTTONS & SEARCH SECTION ---

    // 1. Search Row (Horizontal Sizer)
    wxBoxSizer* searchSizer = new wxBoxSizer(wxHORIZONTAL);

    searchInput = new wxTextCtrl(panel, wxID_ANY, "Enter pattern...");
    searchInput->SetBackgroundColour(lightBg);
    searchInput->SetForegroundColour(textWhite);

    // Radio Buttons (Grouped)
    radioSeqA = new wxRadioButton(panel, wxID_ANY, "Seq A", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    radioSeqB = new wxRadioButton(panel, wxID_ANY, "Seq B");
    radioSeqA->SetForegroundColour(textWhite);
    radioSeqB->SetForegroundColour(textWhite);
    radioSeqA->SetValue(true); // Default to Seq A

    // Create the button
    wxButton* btnSearch = new wxButton(panel, wxID_ANY, "Search Pattern");

    // FIXED: Bind the button IMMEDIATELY here.
    // Do not redeclare it at the bottom of the function.
    btnSearch->Bind(wxEVT_BUTTON, &MainFrame::SearchPattern, this);

    // Add items to the horizontal search row
    searchSizer->Add(searchInput, 1, wxEXPAND | wxRIGHT, 10);
    searchSizer->Add(radioSeqA, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    searchSizer->Add(radioSeqB, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    searchSizer->Add(btnSearch, 0, wxALIGN_CENTER_VERTICAL);

    // 2. Other Buttons
    wxButton* btnCommon = new wxButton(panel, wxID_ANY, "Find Common Region");
    wxButton* btnRepeat = new wxButton(panel, wxID_ANY, "Find Repeats");
    wxButton* btnUnique = new wxButton(panel, wxID_ANY, "Find Unique Regions");

    // --- OUTPUT SECTION ---
    wxStaticText* labelRes = new wxStaticText(panel, wxID_ANY, "Results:");
    labelRes->SetForegroundColour(textWhite);

    output = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
    output->SetBackgroundColour(lightBg);
    output->SetForegroundColour(textWhite);

    // --- ADDING EVERYTHING TO MAIN LAYOUT ---

    vbox->Add(labelA, 0, wxALL, 5);
    vbox->Add(seqA, 1, wxEXPAND | wxALL, 5);

    vbox->Add(labelB, 0, wxALL, 5);
    vbox->Add(seqB, 1, wxEXPAND | wxALL, 5);

    // Add Search Row
    vbox->Add(searchSizer, 0, wxEXPAND | wxALL, 10);

    // Add other buttons
    vbox->Add(btnCommon, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    vbox->Add(btnRepeat, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    vbox->Add(btnUnique, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    vbox->Add(labelRes, 0, wxALL, 5);
    vbox->Add(output, 2, wxEXPAND | wxALL, 5);

    panel->SetSizer(vbox);

    // NOTE: Removed the broken binding code that was here.
}

void MainFrame::SearchPattern(wxCommandEvent& event)
{
    // 1. Get Pattern
    std::string pattern = searchInput->GetValue().ToStdString();
    if (pattern.empty()) {
        output->SetValue("Please enter a pattern to search.");
        return;
    }

    // 2. Get Sequence (A or B based on Radio Button)
    std::string sequence;
    if (radioSeqA->GetValue()) {
        sequence = seqA->GetValue().ToStdString();
    }
    else {
        sequence = seqB->GetValue().ToStdString();
    }

    if (sequence.empty()) {
        output->SetValue("Error: Selected sequence is empty.");
        return;
    }

    // 3. Call your EXISTING backend function
    int count = 0;
    int* indices = DNA::SearchPattern(sequence, pattern, count);

    // 4. Display Results
    if (indices == nullptr || count == 0) {
        output->SetValue("Pattern not found.");
    }
    else {
        wxString resultMsg;
        resultMsg << "Pattern found at indices for "
                  << (radioSeqA->GetValue() ? "SeqA" : "SeqB")  
                  << " :\n";

        for (int i = 0; i < count; i++) {
            resultMsg << indices[i] << ", ";
        }

        // Clean up formatting (remove last comma)
        if (resultMsg.EndsWith(", ")) {
            resultMsg.RemoveLast(2);
        }

        output->SetValue(resultMsg);

        // Clean up memory
        delete[] indices;
    }
}