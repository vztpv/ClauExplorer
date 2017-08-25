
#define _CRT_SECURE_NO_WARNINGS

//
#include <wiz/ClauText.h> // maybe bug exist in parent (wiz::load_data::UserType)
//
#include <Windows.h>
//
#include <wx/wx.h>
//
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/dataview.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////
#define NK_ENTER 13
#define NK_BACKSPACE 8
///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame
{
private:
	wiz::load_data::UserType global;
	wiz::load_data::UserType* now = nullptr;
	int position = -1;
	int dataViewListCtrlNo = -1;
private:
	void RefreshTable(wiz::load_data::UserType* now)
	{
		m_dataViewListCtrl1->DeleteAllItems();
		m_dataViewListCtrl2->DeleteAllItems();
		m_dataViewListCtrl3->DeleteAllItems();
		m_dataViewListCtrl4->DeleteAllItems();

		AddData(now);

		position = -1;
		dataViewListCtrlNo = -1;
	}
	void AddData(wiz::load_data::UserType* global)
	{
		const int size = global->GetUserTypeListSize() + global->GetItemListSize();
		const int utSize = global->GetUserTypeListSize();
		const int size_per_unit = size / 4;
		const int last_size = size - size_per_unit * 3;
		int count = 0;
		int utCount = 0;
		int itCount = 0;

		wxVector<wxVariant> value;

		for (int i = 0; i < size_per_unit; ++i) {
			value.clear();

			if (count < utSize) {
				value.push_back(wxVariant(global->GetUserTypeList(utCount)->GetName().c_str()));
				value.push_back(wxVariant(""));
				utCount++;
			}
			else {
				value.push_back(wxVariant(global->GetItemList(itCount).GetName().c_str()));
				value.push_back(wxVariant(global->GetItemList(itCount).Get(0).c_str()));
				itCount++;
			}

			m_dataViewListCtrl1->AppendItem(value);
			count++;
		}
		for (int i = 0; i < size_per_unit; ++i) {
			value.clear();

			if (count < utSize) {
				value.push_back(wxVariant(global->GetUserTypeList(utCount)->GetName().c_str()));
				value.push_back(wxVariant(""));
				utCount++;
			}
			else {
				value.push_back(wxVariant(global->GetItemList(itCount).GetName().c_str()));
				value.push_back(wxVariant(global->GetItemList(itCount).Get(0).c_str()));
				itCount++;
			}

			m_dataViewListCtrl2->AppendItem(value);
			count++;
		}
		for (int i = 0; i < size_per_unit; ++i) {
			value.clear();

			if (count < utSize) {
				value.push_back(wxVariant(global->GetUserTypeList(utCount)->GetName().c_str()));
				value.push_back(wxVariant(""));
				utCount++;
			}
			else {
				value.push_back(wxVariant(global->GetItemList(itCount).GetName().c_str()));
				value.push_back(wxVariant(global->GetItemList(itCount).Get(0).c_str()));
				itCount++;
			}

			m_dataViewListCtrl3->AppendItem(value);
			count++;
		}
		for (int i = 0; i < last_size; ++i) {
			value.clear();

			if (count < utSize) {
				value.push_back(wxVariant(global->GetUserTypeList(utCount)->GetName().c_str()));
				value.push_back(wxVariant(""));
				utCount++;
			}
			else {
				value.push_back(wxVariant(global->GetItemList(itCount).GetName().c_str()));
				value.push_back(wxVariant(global->GetItemList(itCount).Get(0).c_str()));
				itCount++;
			}

			m_dataViewListCtrl4->AppendItem(value);
			count++;
		}
	}
protected:
	wxMenuBar* menuBar;
	wxMenu* FileMenu;
	wxMenu* DoMenu;
	wxButton* back_button;
	wxTextCtrl* dir_text;
	wxButton* refresh_button;
	wxDataViewListCtrl* m_dataViewListCtrl1;
	wxDataViewListCtrl* m_dataViewListCtrl2;
	wxDataViewListCtrl* m_dataViewListCtrl3;
	wxDataViewListCtrl* m_dataViewListCtrl4;

	// Virtual event handlers, overide them in your derived class
	virtual void FileLoadMenuOnMenuSelection(wxCommandEvent& event) {
		wxFileDialog* openFileDialog = new wxFileDialog(this);

		if (openFileDialog->ShowModal() == wxID_OK) {
			wxString _fileName = openFileDialog->GetPath();
			string fileName(_fileName.c_str());

			global.Remove();
			wiz::load_data::LoadData::LoadDataFromFile(fileName, global);
			now = &global;

			RefreshTable(now);
		}
	}
	virtual void FileSaveMenuOnMenuSelection(wxCommandEvent& event) {  }
	virtual void FileExitMenuOnMenuSelection(wxCommandEvent& event) { Close(true);  }
	virtual void InsertMenuOnMenuSelection(wxCommandEvent& event) {  }
	virtual void ChangeMenuOnMenuSelection(wxCommandEvent& event) {  }
	virtual void RemoveMenuOnMenuSelection(wxCommandEvent& event) {  }
	virtual void back_buttonOnButtonClick(wxCommandEvent& event) {
		if (now && now->GetParent()) { 
			RefreshTable(now->GetParent());
			now = now->GetParent(); 
		} 
	}
	virtual void dir_textOnTextEnter(wxCommandEvent& event) {
		
		wiz::load_data::UserType* ut;
		
		// todo
		 
	}
	virtual void refresh_buttonOnButtonClick(wxCommandEvent& event) {
		if (now) {
			RefreshTable(now);
		}
	}

	virtual void m_dataViewListCtrl1OnChar(wxKeyEvent& event) { 
		dataViewListCtrlNo = 0; position = m_dataViewListCtrl1->GetSelectedRow();
		if (NK_ENTER == event.GetKeyCode() && dataViewListCtrlNo == 0 && position >= 0 && position < now->GetUserTypeListSize()) {
			now = now->GetUserTypeList(position);
			RefreshTable(now);
		}
		else if (NK_BACKSPACE == event.GetKeyCode() && now->GetParent() != nullptr) {
			now = now->GetParent();
			RefreshTable(now);
		}
	}

	virtual void m_dataViewListCtrl2OnChar(wxKeyEvent& event) { 
		dataViewListCtrlNo = 1;  position = m_dataViewListCtrl2->GetSelectedRow();
		if (NK_ENTER == event.GetKeyCode() && dataViewListCtrlNo == 1 && position >= 0 && (position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) < now->GetUserTypeListSize())) {
			now = now->GetUserTypeList(position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4));
			RefreshTable(now);
		}
		else if (NK_BACKSPACE == event.GetKeyCode() && now->GetParent() != nullptr) {
			now = now->GetParent();
			RefreshTable(now);
		}
	}
	virtual void m_dataViewListCtrl3OnChar(wxKeyEvent& event) {
		dataViewListCtrlNo = 2;  position = m_dataViewListCtrl3->GetSelectedRow();
		if (NK_ENTER == event.GetKeyCode() && dataViewListCtrlNo == 2 && position >= 0 && (position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * 2 < now->GetUserTypeListSize())) {
			now = now->GetUserTypeList(position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * 2);
			RefreshTable(now);
		}
		else if (NK_BACKSPACE == event.GetKeyCode() && now->GetParent() != nullptr) {
			now = now->GetParent();
			RefreshTable(now);
		}
	}
	virtual void m_dataViewListCtrl4OnChar(wxKeyEvent& event) {
		dataViewListCtrlNo = 3;  position = m_dataViewListCtrl4->GetSelectedRow();
		if (NK_ENTER == event.GetKeyCode() && dataViewListCtrlNo == 3 && position >= 0 && (position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * 3 < now->GetUserTypeListSize())) {
			now = now->GetUserTypeList(position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * 3);
			RefreshTable(now);
		}
		else if (NK_BACKSPACE == event.GetKeyCode() && now->GetParent() != nullptr) {
			now = now->GetParent();
			RefreshTable(now);
		}
	}
public:

	MainFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ClauExplorer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(789, 512), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

	~MainFrame();

};

MainFrame::MainFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	menuBar = new wxMenuBar(0);
	FileMenu = new wxMenu();
	wxMenuItem* FileLoadMenu;
	FileLoadMenu = new wxMenuItem(FileMenu, wxID_ANY, wxString(wxT("Load")), wxEmptyString, wxITEM_NORMAL);
	FileMenu->Append(FileLoadMenu);

	wxMenuItem* FileSaveMenu;
	FileSaveMenu = new wxMenuItem(FileMenu, wxID_ANY, wxString(wxT("Save")), wxEmptyString, wxITEM_NORMAL);
	FileMenu->Append(FileSaveMenu);

	FileMenu->AppendSeparator();

	wxMenuItem* FileExitMenu;
	FileExitMenu = new wxMenuItem(FileMenu, wxID_ANY, wxString(wxT("Exit")), wxEmptyString, wxITEM_NORMAL);
	FileMenu->Append(FileExitMenu);

	FileMenu->AppendSeparator();

	menuBar->Append(FileMenu, wxT("File"));

	DoMenu = new wxMenu();
	wxMenuItem* InsertMenu;
	InsertMenu = new wxMenuItem(DoMenu, wxID_ANY, wxString(wxT("Insert")), wxEmptyString, wxITEM_NORMAL);
	DoMenu->Append(InsertMenu);

	wxMenuItem* ChangeMenu;
	ChangeMenu = new wxMenuItem(DoMenu, wxID_ANY, wxString(wxT("Change")), wxEmptyString, wxITEM_NORMAL);
	DoMenu->Append(ChangeMenu);

	wxMenuItem* RemoveMenu;
	RemoveMenu = new wxMenuItem(DoMenu, wxID_ANY, wxString(wxT("Remove")), wxEmptyString, wxITEM_NORMAL);
	DoMenu->Append(RemoveMenu);

	menuBar->Append(DoMenu, wxT("Do"));

	this->SetMenuBar(menuBar);

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	back_button = new wxButton(this, wxID_ANY, wxT("бу"), wxDefaultPosition, wxDefaultSize, 0);
	back_button->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString));

	bSizer2->Add(back_button, 0, wxALL, 5);

	dir_text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	bSizer2->Add(dir_text, 1, wxALL, 5);

	refresh_button = new wxButton(this, wxID_ANY, wxT("Refresh"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(refresh_button, 0, wxALL, 5);


	bSizer->Add(bSizer2, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxHORIZONTAL);

	m_dataViewListCtrl1 = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(m_dataViewListCtrl1, 1, wxALL | wxEXPAND, 5);

	m_dataViewListCtrl2 = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(m_dataViewListCtrl2, 1, wxALL | wxEXPAND, 5);

	m_dataViewListCtrl3 = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(m_dataViewListCtrl3, 1, wxALL | wxEXPAND, 5);

	m_dataViewListCtrl4 = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(m_dataViewListCtrl4, 1, wxALL | wxEXPAND, 5);

	bSizer->Add(bSizer3, 1, wxEXPAND, 5);

	m_dataViewListCtrl1->AppendTextColumn(wxT("name"));
	m_dataViewListCtrl1->AppendTextColumn(wxT("value"));

	m_dataViewListCtrl2->AppendTextColumn(wxT("name"));
	m_dataViewListCtrl2->AppendTextColumn(wxT("value"));
	
	m_dataViewListCtrl3->AppendTextColumn(wxT("name"));
	m_dataViewListCtrl3->AppendTextColumn(wxT("value"));

	m_dataViewListCtrl4->AppendTextColumn(wxT("name"));
	m_dataViewListCtrl4->AppendTextColumn(wxT("value"));

	this->SetSizer(bSizer);
	this->Layout();

	this->Centre(wxBOTH);

	// Connect Events
	this->Connect(FileLoadMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::FileLoadMenuOnMenuSelection));
	this->Connect(FileSaveMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::FileSaveMenuOnMenuSelection));
	this->Connect(FileExitMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::FileExitMenuOnMenuSelection));
	this->Connect(InsertMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::InsertMenuOnMenuSelection));
	this->Connect(ChangeMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::ChangeMenuOnMenuSelection));
	this->Connect(RemoveMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::RemoveMenuOnMenuSelection));
	
	back_button->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::back_buttonOnButtonClick), NULL, this);
	dir_text->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(MainFrame::dir_textOnTextEnter), NULL, this);
	refresh_button->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::refresh_buttonOnButtonClick), NULL, this);
	
	m_dataViewListCtrl1->Connect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl1OnChar), NULL, this);
	m_dataViewListCtrl2->Connect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl2OnChar), NULL, this);
	m_dataViewListCtrl3->Connect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl3OnChar), NULL, this);
	m_dataViewListCtrl4->Connect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl4OnChar), NULL, this);

}

MainFrame::~MainFrame()
{
	// Disconnect Events
	this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::FileLoadMenuOnMenuSelection));
	this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::FileSaveMenuOnMenuSelection));
	this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::FileExitMenuOnMenuSelection));
	this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::InsertMenuOnMenuSelection));
	this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::ChangeMenuOnMenuSelection));
	this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::RemoveMenuOnMenuSelection));
	
	back_button->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::back_buttonOnButtonClick), NULL, this);
	dir_text->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(MainFrame::dir_textOnTextEnter), NULL, this);
	refresh_button->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::refresh_buttonOnButtonClick), NULL, this);

	m_dataViewListCtrl1->Disconnect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl1OnChar), NULL, this);
	m_dataViewListCtrl2->Disconnect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl2OnChar), NULL, this);
	m_dataViewListCtrl3->Disconnect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl3OnChar), NULL, this);
	m_dataViewListCtrl4->Disconnect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl4OnChar), NULL, this);
}

class TestApp : public wxApp {
public:
	virtual bool OnInit() {
		MainFrame* frame = new MainFrame(nullptr);
		frame->Show(true);
		return true;
	}
};

IMPLEMENT_APP(TestApp)

