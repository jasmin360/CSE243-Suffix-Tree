#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>
//#include algorithms.h

MainFrame::MainFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, "DNA Suffix Tree Tool (Ukkonnen)", wxDefaultPosition, wxSize(800, 600))
{
	wxPanel* panel = new wxPanel(this);
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

	seqA = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	seqB = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

	output = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

	wxButton* btnSearch = new wxButton(panel, wxID_ANY, "Search Pattern");
	wxButton* btnCommon = new wxButton(panel, wxID_ANY, "Common Region");
	wxButton* btnRepeat = new wxButton(panel, wxID_ANY, "Find Repeats");
	wxButton* btnUnique = new wxButton(panel, wxID_ANY, "Unqiue Regions");


	vbox->Add(new wxStaticText(panel, wxID_ANY, "DNA Sequence A"), 0, wxALL, 5);
	vbox->Add(seqA, 1, wxEXPAND | wxALL, 5);

	vbox->Add(new wxStaticText(panel, wxID_ANY, "DNA Sequence B"), 0, wxALL, 5);
	vbox->Add(seqB, 1, wxEXPAND | wxALL, 5);

	vbox->Add(btnSearch, 0, wxEXPAND | wxALL, 5);
	vbox->Add(btnCommon, 0, wxEXPAND | wxALL, 5);
	vbox->Add(btnRepeat, 0, wxEXPAND | wxALL, 5);
	vbox->Add(btnUnique, 0, wxEXPAND | wxALL, 5);

	vbox->Add(new wxStaticText(panel, wxID_ANY, "Results"), 0, wxALL, 5);
	vbox->Add(output, 1, wxEXPAND | wxALL, 5);

	panel->SetSizer(vbox);
	
	//btnSearch->Bind(wxEVT_BUTTON, &MainFrame::SearchPattern, this);
	//btnSearch->Bind(wxEVT_BUTTON, &MainFrame::CommonRegion, this);
	//btnSearch->Bind(wxEVT_BUTTON, &MainFrame::FindRepeats, this);
	//btnSearch->Bind(wxEVT_BUTTON, &MainFrame::UniqueRegions, this);

}

