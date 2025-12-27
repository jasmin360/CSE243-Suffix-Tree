#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);

private:
	wxTextCtrl* seqA;
	wxTextCtrl* seqB;
	wxTextCtrl* output;
	wxTextCtrl* searchInput;


	wxRadioButton* radioSeqA;
	wxRadioButton* radioSeqB;

	void SearchPattern(wxCommandEvent& event);
	void CommonRegion(wxCommandEvent& event);
	void FindRepeats(wxCommandEvent& event);
	void UniqueRegions(wxCommandEvent& event);
};