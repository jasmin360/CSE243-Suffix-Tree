#include "MainFrame.h"
#include "functions.h"
#include <wx/wx.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/statline.h>
#include <string>
#include <wx/slider.h>

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(900, 750))
{
    // --- THEME COLORS ---
    wxColour darkBg("#1e272e");
    wxColour lightBg("#485460");
    wxColour textWhite(*wxWHITE);
    wxColour accentGreen("#05c46b");
    wxColour accentBlue("#0fbcf9");  // Different color for seqB

    // Main Panel
    wxPanel* panel = new wxPanel(this);
    panel->SetBackgroundColour(darkBg);
    panel->SetForegroundColour(textWhite);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // --- FILE UPLOAD SECTION (SEQ A) ---
    wxStaticText* labelTitleA = new wxStaticText(panel, wxID_ANY, "DNA Sequence A:");
    labelTitleA->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    labelTitleA->SetForegroundColour(textWhite);

    wxBoxSizer* fileASizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btnLoadFileA = new wxButton(panel, wxID_ANY, "Load DNA File A (.txt)");
    btnLoadFileA->SetBackgroundColour(accentGreen);
    btnLoadFileA->SetForegroundColour(wxColour("#000000"));

    lblStatusA = new wxStaticText(panel, wxID_ANY, "No file loaded.");
    lblStatusA->SetForegroundColour(textWhite);

    fileASizer->Add(btnLoadFileA, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 15);
    fileASizer->Add(lblStatusA, 1, wxALIGN_CENTER_VERTICAL);

    btnLoadFileA->Bind(wxEVT_BUTTON, &MainFrame::OnLoadFileA, this);

    // --- FILE UPLOAD SECTION (SEQ B) ---
    wxStaticText* labelTitleB = new wxStaticText(panel, wxID_ANY, "DNA Sequence B:");
    labelTitleB->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    labelTitleB->SetForegroundColour(textWhite);

    wxBoxSizer* fileBSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btnLoadFileB = new wxButton(panel, wxID_ANY, "Load DNA File B (.txt)");
    btnLoadFileB->SetBackgroundColour(accentBlue);
    btnLoadFileB->SetForegroundColour(wxColour("#000000"));

    lblStatusB = new wxStaticText(panel, wxID_ANY, "No file loaded.");
    lblStatusB->SetForegroundColour(textWhite);

    fileBSizer->Add(btnLoadFileB, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 15);
    fileBSizer->Add(lblStatusB, 1, wxALIGN_CENTER_VERTICAL);

    btnLoadFileB->Bind(wxEVT_BUTTON, &MainFrame::OnLoadFileB, this);

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
    wxStaticBoxSizer* uniqueGroup = new wxStaticBoxSizer(wxHORIZONTAL, panel, "Find Unique Region");
    uniqueGroup->GetStaticBox()->SetForegroundColour(textWhite);

    lblSliderVal = new wxStaticText(panel, wxID_ANY, "Length: 5");
    lblSliderVal->SetForegroundColour(textWhite);

    sliderUnique = new wxSlider(panel, wxID_ANY, 5, 1, 50, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    sliderUnique->SetBackgroundColour(darkBg);
    sliderUnique->SetForegroundColour(textWhite);

    sliderUnique->Bind(wxEVT_SLIDER, [this](wxCommandEvent&) {
        lblSliderVal->SetLabel("Length: " + std::to_string(sliderUnique->GetValue()));
        });

    wxButton* btnUnique = new wxButton(panel, wxID_ANY, "Find Unique");
    btnUnique->Bind(wxEVT_BUTTON, &MainFrame::UniqueRegions, this);

    uniqueGroup->Add(lblSliderVal, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    uniqueGroup->Add(sliderUnique, 1, wxEXPAND | wxRIGHT, 10);
    uniqueGroup->Add(btnUnique, 0, wxALIGN_CENTER_VERTICAL);

    // --- OUTPUT SECTION ---
    wxStaticText* labelRes = new wxStaticText(panel, wxID_ANY, "Results / Status:");
    labelRes->SetForegroundColour(textWhite);

    output = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_READONLY);
    output->SetBackgroundColour(lightBg);
    output->SetForegroundColour(textWhite);
    output->SetFont(wxFont(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    // --- LAYOUT ASSEMBLY ---
    vbox->Add(labelTitleA, 0, wxALL, 10);
    vbox->Add(fileASizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    vbox->Add(labelTitleB, 0, wxLEFT | wxRIGHT | wxTOP, 10);
    vbox->Add(fileBSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    wxStaticLine* line1 = new wxStaticLine(panel, wxID_ANY);
    vbox->Add(line1, 0, wxEXPAND | wxALL, 10);

    vbox->Add(searchSizer, 0, wxEXPAND | wxALL, 10);
    vbox->Add(toolsSizer, 0, wxEXPAND | wxALL, 10);
    vbox->Add(uniqueGroup, 0, wxEXPAND | wxALL, 10);

    vbox->Add(labelRes, 0, wxLEFT | wxTOP, 10);
    vbox->Add(output, 1, wxEXPAND | wxALL, 10);

    panel->SetSizer(vbox);
}

// --- FILE LOADING FUNCTION A ---
void MainFrame::OnLoadFileA(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, "Open DNA Text File A", "", "",
        "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxString path = openFileDialog.GetPath();
    wxFile file;
    if (file.Open(path))
    {
        wxString fileContent;
        file.ReadAll(&fileContent);

        // Clean Data
        std::string raw = fileContent.ToStdString();
        loadedSequenceA = "";
        for (char c : raw)
        {
            char upper = toupper(c);
            if (upper == 'A' || upper == 'C' || upper == 'G' || upper == 'T')
            {
                loadedSequenceA += upper;
            }
        }

        // Update UI
        wxFileName fileName(path);
        lblStatusA->SetLabel("Loaded: " + fileName.GetFullName() +
            " (" + std::to_string(loadedSequenceA.length()) + " bases)");
        output->SetValue("File A loaded successfully.\nReady to search.");

        // Update slider max range
        if (loadedSequenceA.length() < 50 && loadedSequenceA.length() > 0)
        {
            sliderUnique->SetMax(loadedSequenceA.length());
        }
        else
        {
            sliderUnique->SetMax(50);
        }
    }
    else
    {
        wxMessageBox("Cannot open file", "Error", wxICON_ERROR);
    }
}

// --- FILE LOADING FUNCTION B ---
void MainFrame::OnLoadFileB(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, "Open DNA Text File B", "", "",
        "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxString path = openFileDialog.GetPath();
    wxFile file;
    if (file.Open(path))
    {
        wxString fileContent;
        file.ReadAll(&fileContent);

        // Clean Data
        std::string raw = fileContent.ToStdString();
        loadedSequenceB = "";
        for (char c : raw)
        {
            char upper = toupper(c);
            if (upper == 'A' || upper == 'C' || upper == 'G' || upper == 'T')
            {
                loadedSequenceB += upper;
            }
        }

        // Update UI
        wxFileName fileName(path);
        lblStatusB->SetLabel("Loaded: " + fileName.GetFullName() +
            " (" + std::to_string(loadedSequenceB.length()) + " bases)");
        output->SetValue("File B loaded successfully.\nReady to search.");
    }
    else
    {
        wxMessageBox("Cannot open file", "Error", wxICON_ERROR);
    }
}

// --- SEARCH FUNCTION (Uses Sequence A) ---
void MainFrame::SearchPattern(wxCommandEvent& event)
{
    if (loadedSequenceA.empty())
    {
        output->SetValue("Error: No DNA file A loaded. Please load file A first.");
        return;
    }

    std::string rawPattern = searchInput->GetValue().ToStdString();
    std::string pattern = "";
    for (char c : rawPattern)
    {
        char upper = toupper(c);
        if (upper == 'A' || upper == 'C' || upper == 'G' || upper == 'T')
            pattern += upper;
    }

    if (pattern.empty())
    {
        output->SetValue("Error: Search pattern is empty.");
        return;
    }

    int count = 0;
    int* indices = DNA::SearchPattern(loadedSequenceA, pattern, count);

    if (indices == nullptr || count == 0)
    {
        output->SetValue("Pattern '" + pattern + "' NOT found.");
    }
    else
    {
        wxString resultMsg;
        resultMsg << "Pattern '" << pattern << "' found " << count << " times at indices:\n";
        int limit = (count > 5000) ? 5000 : count;
        for (int i = 0; i < limit; i++)
        {
            resultMsg << indices[i] << ", ";
        }
        if (count > limit)
            resultMsg << "... (truncated)";
        else if (resultMsg.EndsWith(", "))
            resultMsg.RemoveLast(2);

        output->SetValue(resultMsg);
        delete[] indices;
    }
}

// --- UNIQUE REGIONS FUNCTION (Uses Sequence A) ---
void MainFrame::UniqueRegions(wxCommandEvent& event)
{
    if (loadedSequenceA.empty())
    {
        output->SetValue("Error: No DNA file A loaded. Please load file A first.");
        return;
    }

    int x = sliderUnique->GetValue();
    std::string* res = DNA::findUniqueRegion(loadedSequenceA, x);

    if (!res)
    {
        output->SetValue("Error: Backend returned null.");
        return;
    }

    if (res[0].empty())
    {
        output->SetValue("No unique region of length " + std::to_string(x) + " found.");
    }
    else
    {
        output->SetValue("Found Unique Region (Length " + std::to_string(x) + "):\n" + res[0]);
    }

    delete[] res;
}

void MainFrame::findCommonRegion(wxCommandEvent& event)
{
    if (loadedSequenceA.empty() || loadedSequenceB.empty())
    {
        output->SetValue("Error: Both DNA files must be loaded to find common regions.");
        return;
    }
    std::string commonRegion = suffixTree.findLargestCommonRegion(loadedSequenceA.c_str(), loadedSequenceB.c_str());
    if (commonRegion.empty())
    {
        output->SetValue("No common region found between the two sequences.");
    }
    else
    {
        output->SetValue("Found Common Region:\n" + commonRegion);
    }
}