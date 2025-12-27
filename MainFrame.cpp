#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>

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
    // wxRB_GROUP makes the first one start the group (mutually exclusive)
    radioSeqA = new wxRadioButton(panel, wxID_ANY, "Seq A", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    radioSeqB = new wxRadioButton(panel, wxID_ANY, "Seq B");
    radioSeqA->SetForegroundColour(textWhite);
    radioSeqB->SetForegroundColour(textWhite);
    radioSeqA->SetValue(true); // Default to Seq A

    wxButton* btnSearch = new wxButton(panel, wxID_ANY, "Search Pattern");

    // Add items to the horizontal search row
    searchSizer->Add(searchInput, 1, wxEXPAND | wxRIGHT, 10); // Input box takes available space
    searchSizer->Add(radioSeqA, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    searchSizer->Add(radioSeqB, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    searchSizer->Add(btnSearch, 0, wxALIGN_CENTER_VERTICAL);

    // 2. Other Buttons
    // We create them here, but we will add them with different flags later
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

    // Add other buttons (Notice: Removed wxEXPAND, added wxALIGN_CENTER_HORIZONTAL)
    vbox->Add(btnCommon, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    vbox->Add(btnRepeat, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    vbox->Add(btnUnique, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

    vbox->Add(labelRes, 0, wxALL, 5);
    vbox->Add(output, 2, wxEXPAND | wxALL, 5);

    panel->SetSizer(vbox);

    // --- BINDINGS ---
    // btnSearch->Bind(wxEVT_BUTTON, &MainFrame::SearchPattern, this);
    // btnCommon->Bind(wxEVT_BUTTON, &MainFrame::CommonRegion, this);
    // btnRepeat->Bind(wxEVT_BUTTON, &MainFrame::FindRepeats, this);
    // btnUnique->Bind(wxEVT_BUTTON, &MainFrame::UniqueRegions, this);
}