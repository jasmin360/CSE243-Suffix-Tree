#include "MainFrame.h"
#include "functions.h"
#include <wx/wx.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/statline.h>
#include <string>
#include <wx/slider.h>

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(900, 800))
{
    // --- THEME COLORS ---
    wxColour darkBg("#1e272e");
    wxColour lightBg("#485460");
    wxColour textWhite(*wxWHITE);
    wxColour accentGreen("#05c46b");
    wxColour accentBlue("#0fbcf9");

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

    wxStaticLine* line1 = new wxStaticLine(panel, wxID_ANY);

    // --- RADIO BUTTON SECTION (Target Sequence) ---
    wxStaticBoxSizer* radioGroup = new wxStaticBoxSizer(wxHORIZONTAL, panel, "Target Sequence");
    radioGroup->GetStaticBox()->SetForegroundColour(textWhite);

    radioSeqA = new wxRadioButton(panel, wxID_ANY, "Sequence A", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    radioSeqB = new wxRadioButton(panel, wxID_ANY, "Sequence B");
    radioSeqA->SetValue(true);
    radioSeqA->SetForegroundColour(textWhite);
    radioSeqB->SetForegroundColour(textWhite);

    radioGroup->Add(radioSeqA, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 15);
    radioGroup->Add(radioSeqB, 0, wxALIGN_CENTER_VERTICAL);

    // --- SEARCH INPUT SECTION ---
    wxBoxSizer* searchSizer = new wxBoxSizer(wxHORIZONTAL);
    searchInput = new wxTextCtrl(panel, wxID_ANY, "Enter pattern (e.g. TG)...");
    searchInput->SetBackgroundColour(lightBg);
    searchInput->SetForegroundColour(textWhite);

    wxButton* btnSearch = new wxButton(panel, wxID_ANY, "Search Pattern");
    btnSearch->Bind(wxEVT_BUTTON, &MainFrame::SearchPattern, this);

    searchSizer->Add(searchInput, 1, wxEXPAND | wxRIGHT, 10);
    searchSizer->Add(btnSearch, 0, wxALIGN_CENTER_VERTICAL);

    // --- UNIQUE REGION SECTION (With Slider) ---
    wxStaticBoxSizer* uniqueGroup = new wxStaticBoxSizer(wxHORIZONTAL, panel, "Find Unique Region");
    uniqueGroup->GetStaticBox()->SetForegroundColour(textWhite);

    lblSliderUnique = new wxStaticText(panel, wxID_ANY, "Length: 5");
    lblSliderUnique->SetForegroundColour(textWhite);

    sliderUnique = new wxSlider(panel, wxID_ANY, 5, 1, 50000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    sliderUnique->SetBackgroundColour(darkBg);
    sliderUnique->SetForegroundColour(textWhite);

    sliderUnique->Bind(wxEVT_SLIDER, [this](wxCommandEvent&) {
        lblSliderUnique->SetLabel("Length: " + std::to_string(sliderUnique->GetValue()));
        });

    wxButton* btnUnique = new wxButton(panel, wxID_ANY, "Find Unique");
    btnUnique->Bind(wxEVT_BUTTON, &MainFrame::UniqueRegions, this);

    uniqueGroup->Add(lblSliderUnique, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    uniqueGroup->Add(sliderUnique, 1, wxEXPAND | wxRIGHT, 10);
    uniqueGroup->Add(btnUnique, 0, wxALIGN_CENTER_VERTICAL);

    wxStaticLine* line2 = new wxStaticLine(panel, wxID_ANY);

    // --- MAX REPETITION SECTION (With Slider) ---
    wxStaticBoxSizer* maxRepGroup = new wxStaticBoxSizer(wxHORIZONTAL, panel, "Find Max Repetition");
    maxRepGroup->GetStaticBox()->SetForegroundColour(textWhite);

    lblSliderMaxRep = new wxStaticText(panel, wxID_ANY, "Pattern Length: 4");
    lblSliderMaxRep->SetForegroundColour(textWhite);

    sliderMaxRep = new wxSlider(panel, wxID_ANY, 4, 2, 20, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
    sliderMaxRep->SetBackgroundColour(darkBg);
    sliderMaxRep->SetForegroundColour(textWhite);

    sliderMaxRep->Bind(wxEVT_SLIDER, [this](wxCommandEvent&) {
        lblSliderMaxRep->SetLabel("Pattern Length: " + std::to_string(sliderMaxRep->GetValue()));
        });

    wxButton* btnMaxRep = new wxButton(panel, wxID_ANY, "Find Max Repetition");
    btnMaxRep->Bind(wxEVT_BUTTON, &MainFrame::MaxRepetition, this);

    maxRepGroup->Add(lblSliderMaxRep, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    maxRepGroup->Add(sliderMaxRep, 1, wxEXPAND | wxRIGHT, 10);
    maxRepGroup->Add(btnMaxRep, 0, wxALIGN_CENTER_VERTICAL);

    // --- COMMON REGION BUTTON ---
    wxBoxSizer* commonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* btnCommon = new wxButton(panel, wxID_ANY, "Find Common Region (Uses Both Sequences)");
    btnCommon->Bind(wxEVT_BUTTON, &MainFrame::findCommonRegion, this);
    commonSizer->Add(btnCommon, 0, wxALIGN_CENTER_VERTICAL);

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

    vbox->Add(line1, 0, wxEXPAND | wxALL, 10);

    vbox->Add(radioGroup, 0, wxEXPAND | wxALL, 10);
    vbox->Add(searchSizer, 0, wxEXPAND | wxALL, 10);
    vbox->Add(uniqueGroup, 0, wxEXPAND | wxALL, 10);

    vbox->Add(line2, 0, wxEXPAND | wxALL, 10);

    vbox->Add(maxRepGroup, 0, wxEXPAND | wxALL, 10);

    vbox->Add(commonSizer, 0, wxEXPAND | wxALL, 10);

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

        wxFileName fileName(path);
        lblStatusA->SetLabel("Loaded: " + fileName.GetFullName() +
            " (" + std::to_string(loadedSequenceA.length()) + " bases)");
        output->SetValue("File A loaded successfully.\nReady to search.");

        if (loadedSequenceA.length() < 50000 && loadedSequenceA.length() > 0)
        {
            sliderUnique->SetMax(loadedSequenceA.length());
        }
        else
        {
            sliderUnique->SetMax(50000);
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

// --- SEARCH FUNCTION ---
void MainFrame::SearchPattern(wxCommandEvent& event)
{
    bool useSeqA = radioSeqA->GetValue();
    std::string& selectedSequence = useSeqA ? loadedSequenceA : loadedSequenceB;
    std::string seqName = useSeqA ? "A" : "B";

    if (selectedSequence.empty())
    {
        output->SetValue("Error: No DNA file " + seqName + " loaded. Please load file " + seqName + " first.");
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
    int* indices = DNA::SearchPattern(selectedSequence, pattern, count);

    if (indices == nullptr || count == 0)
    {
        output->SetValue("Pattern '" + pattern + "' NOT found in Sequence " + seqName + ".");
    }
    else
    {
        wxString resultMsg;
        resultMsg << "Pattern '" << pattern << "' found " << count << " times in Sequence " << seqName << " at indices:\n";
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

// --- UNIQUE REGIONS FUNCTION ---
// --- UNIQUE REGIONS FUNCTION ---
void MainFrame::UniqueRegions(wxCommandEvent& event)
{
    // 1. Determine which sequence to use (A or B)
    bool useSeqA = radioSeqA->GetValue();
    std::string& selectedSequence = useSeqA ? loadedSequenceA : loadedSequenceB;
    std::string seqName = useSeqA ? "A" : "B";

    // 2. Safety Check
    if (selectedSequence.empty()) {
        output->SetValue("Error: No DNA file " + seqName + " loaded. Please load file " + seqName + " first.");
        return;
    }

    // 3. Get Length 'X' from Slider
    int x = sliderUnique->GetValue();

    // 4. Call Backend
    std::string* res = DNA::findUniqueRegion(selectedSequence, x);

    if (!res) {
        output->SetValue("Error: Backend returned null.");
        return;
    }

    // 5. Format Output (Loop through the 3 results)
    if (res[0].empty()) {
        output->SetValue("No unique region of length " + std::to_string(x) + " found in Sequence " + seqName + ".");
    }
    else {
        wxString msg;
        msg << "Found Unique Regions in " << seqName << " (Length " << x << "):\n\n";

        for (int i = 0; i < 3; i++) {
            if (!res[i].empty()) {
                msg << (i + 1) << ". " << res[i] << "\n";
            }
        }
        output->SetValue(msg);
    }

    // 6. Cleanup
    delete[] res;
}
// --- MAX REPETITION FUNCTION ---
void MainFrame::MaxRepetition(wxCommandEvent& event)
{
    bool useSeqA = radioSeqA->GetValue();
    std::string& selectedSequence = useSeqA ? loadedSequenceA : loadedSequenceB;
    std::string seqName = useSeqA ? "A" : "B";

    if (selectedSequence.empty())
    {
        output->SetValue("Error: No DNA file " + seqName + " loaded. Please load file " + seqName + " first.");
        return;
    }

    int x = sliderMaxRep->GetValue();
    int count = 0;
    std::string res = "";
    std::string maxRepetition = DNA::findMaxRepetition(selectedSequence, x, count, res);

    if (res.empty())
    {
        output->SetValue("No repeating pattern of length " + std::to_string(x) + " found in Sequence " + seqName + ".");
    }
    else
    {
        output->SetValue("Max Repeating Pattern in Sequence " + seqName + ":\n" +
            "Pattern: " + res + "\n" +
            "Repeated: " + std::to_string(count) + " times");
    }
}

// --- COMMON REGION FUNCTION ---
void MainFrame::findCommonRegion(wxCommandEvent& event)
{
    if (loadedSequenceA.empty() || loadedSequenceB.empty())
    {
        output->SetValue("Error: Both DNA files must be loaded to find common regions.");
        return;
    }
    std::string commonRegion = DNA::findLargestCommonRegion(loadedSequenceA, loadedSequenceB);
    if (commonRegion.empty())
    {
        output->SetValue("No common region found between the two sequences.");
    }
    else
    {
        output->SetValue("Largest Common Region Found (Length " +
            std::to_string(commonRegion.length()) + "):\n" + commonRegion);
    }
}