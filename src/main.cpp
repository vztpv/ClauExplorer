
#define _CRT_SECURE_NO_WARNINGS

#ifdef _DEBUG
#include <vld.h>
#endif

//
#include <wiz/ClauText.h> 
#include <string>
#include <algorithm>
//
#include <Windows.h>
//
#include <wx/wx.h>

#include <wx/defs.h>
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


class ChangeWindow : public wxFrame
{
private:
	// function??
	int view_mode;
	wiz::load_data::UserType* ut;
	bool isUserType; // ut(true) or it(false)
	int idx; // utidx or itidx. or ilist idx(type == insert)
	int type; // change 1, insert 2
protected:
	wxTextCtrl* var_text;
	wxTextCtrl* val_text;
	wxButton* ok;
	
	// Virtual event handlers, overide them in your derived class
	virtual void okOnButtonClick(wxCommandEvent& event) {
		string var(var_text->GetValue().c_str());
		string val(val_text->GetValue().c_str());

		if (1 == type) {
			if (isUserType) {
				ut->GetUserTypeList(idx)->SetName(var);
			}
			else {
				if (!val.empty()) {
					ut->GetItemList(idx).SetName(var);
					ut->GetItemList(idx).Set(0, val);
				}
			}
		}
		else if (2 == type && 2 == view_mode) {
			if (val.empty()) {
				ut->InsertUserTypeByIlist(idx, wiz::load_data::UserType(var));
			}
			else {
				if (!val.empty()) {
					ut->InsertItemByIlist(idx, var, val);
				}
			}
		}

		Close();
	}

public:
	ChangeWindow(wxWindow* parent,wiz::load_data::UserType* ut, bool isUserType, int idx, int type, int view_mode, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(580, 198), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);
	~ChangeWindow();
};

ChangeWindow::ChangeWindow(wxWindow* parent, wiz::load_data::UserType* ut, bool isUserType, int idx, int type,  int view_mode, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) 
	: ut(ut), isUserType(isUserType), idx(idx), type(type), view_mode(view_mode), wxFrame(parent, id, "change/insert window", pos, size, style)
{

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxVERTICAL);

	var_text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(var_text, 1, wxALL | wxEXPAND, 5);

	val_text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(val_text, 1, wxALL | wxEXPAND, 5);

	ok = new wxButton(this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer4->Add(ok, 0, wxALL | wxEXPAND, 5);

	if (1 == type) {
		if (isUserType) {
			var_text->SetValue(ut->GetUserTypeList(idx)->GetName());
		}
		else {
			var_text->SetValue(ut->GetItemList(idx).GetName());
			val_text->SetValue(ut->GetItemList(idx).Get(0));
		}
	}
	

	this->SetSizer(bSizer4);
	this->Layout();

	// Connect Events
	ok->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ChangeWindow::okOnButtonClick), NULL, this);
}

ChangeWindow::~ChangeWindow()
{
	// Disconnect Events
	ok->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ChangeWindow::okOnButtonClick), NULL, this);
}

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame
{
private:
	bool isMain = false;
	int view_mode = 1; // todo, insert : when view_mode == 2.
	wiz::load_data::UserType global;
	wiz::load_data::UserType* now = nullptr;

	int dataViewListCtrlNo = -1; 
	int position = -1;

private:
	void RefreshTable(wiz::load_data::UserType* now)
	{
		m_dataViewListCtrl1->DeleteAllItems();
		m_dataViewListCtrl2->DeleteAllItems();
		m_dataViewListCtrl3->DeleteAllItems();
		m_dataViewListCtrl4->DeleteAllItems();

		AddData(now);
		
		dataViewListCtrlNo = -1;
		position = -1;

		{
			wxDataViewListCtrl* ctrl[4];
			ctrl[0] = m_dataViewListCtrl1;
			ctrl[1] = m_dataViewListCtrl2;
			ctrl[2] = m_dataViewListCtrl3;
			ctrl[3] = m_dataViewListCtrl4;

			for (int i = 0; i < 4; ++i) {
				if (ctrl[i]->GetItemCount() > 0) {
					dataViewListCtrlNo = i;
					position = 0;

					ctrl[dataViewListCtrlNo]->SelectRow(position);
					ctrl[dataViewListCtrlNo]->SetFocus();
					break;
				}
			}
		}
	}
	void AddData(wiz::load_data::UserType* global)
	{
		if (1 == view_mode) {
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
					if (global->GetUserTypeList(utCount)->GetName().empty()) {
						value.push_back(wxVariant(wxT("NO_NAME")));
					}
					else {
						value.push_back(wxVariant(global->GetUserTypeList(utCount)->GetName().c_str()));
					}
					value.push_back(wxVariant(wxT("")));
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
					if (global->GetUserTypeList(utCount)->GetName().empty()) {
						value.push_back(wxVariant(wxT("NO_NAME")));
					}
					else {
						value.push_back(wxVariant(global->GetUserTypeList(utCount)->GetName().c_str()));
					}
					value.push_back(wxVariant(wxT("")));
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
					if (global->GetUserTypeList(utCount)->GetName().empty()) {
						value.push_back(wxVariant(wxT("NO_NAME")));
					}
					else {
						value.push_back(wxVariant(global->GetUserTypeList(utCount)->GetName().c_str()));
					}
					value.push_back(wxVariant(wxT("")));
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
					if (global->GetUserTypeList(utCount)->GetName().empty()) {
						value.push_back(wxVariant(wxT("NO_NAME")));
					}
					else {
						value.push_back(wxVariant(global->GetUserTypeList(utCount)->GetName().c_str()));
					}
					value.push_back(wxVariant(wxT("")));
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
		else {
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

				if (global->IsUserTypeList(count)) {
					if (global->GetUserTypeList(utCount)->GetName().empty()) {
						value.push_back(wxVariant(wxT("NO_NAME")));
					}
					else {
						value.push_back(wxVariant(global->GetUserTypeList(utCount)->GetName().c_str()));
					}
					value.push_back(wxVariant(wxT("")));
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

				if (global->IsUserTypeList(count)) {
					if (global->GetUserTypeList(utCount)->GetName().empty()) {
						value.push_back(wxVariant(wxT("NO_NAME")));
					}
					else {
						value.push_back(wxVariant(global->GetUserTypeList(utCount)->GetName().c_str()));
					}
					value.push_back(wxVariant(wxT("")));
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

				if (global->IsUserTypeList(count)) {
					if (global->GetUserTypeList(utCount)->GetName().empty()) {
						value.push_back(wxVariant(wxT("NO_NAME")));
					}
					else {
						value.push_back(wxVariant(global->GetUserTypeList(utCount)->GetName().c_str()));
					}
					value.push_back(wxVariant(wxT("")));
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

				if (global->IsUserTypeList(count)) {
					if (global->GetUserTypeList(utCount)->GetName().empty()) {
						value.push_back(wxVariant(wxT("NO_NAME")));
					}
					else {
						value.push_back(wxVariant(global->GetUserTypeList(utCount)->GetName().c_str()));
					}
					value.push_back(wxVariant(wxT("")));
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
	}
protected:
	wxMenuBar* menuBar;
	wxMenu* FileMenu;
	wxMenu* DoMenu;
	wxMenu* ViewMenu;
	wxMenu* WindowMenu;
	wxButton* back_button;
	wxTextCtrl* dir_text;
	wxButton* refresh_button;
	wxDataViewListCtrl* m_dataViewListCtrl1;
	wxDataViewListCtrl* m_dataViewListCtrl2;
	wxDataViewListCtrl* m_dataViewListCtrl3;
	wxDataViewListCtrl* m_dataViewListCtrl4;
	wxStatusBar* m_statusBar1;

	// Virtual event handlers, overide them in your derived class
	virtual void FileLoadMenuOnMenuSelection(wxCommandEvent& event) {
		if (!isMain) { return; }
		wxFileDialog* openFileDialog = new wxFileDialog(this);

		if (openFileDialog->ShowModal() == wxID_OK) {
			wxString _fileName = openFileDialog->GetPath();
			std::string fileName(_fileName.c_str());

			global.Remove();
			wiz::load_data::LoadData::LoadDataFromFile(fileName, global);
			now = &global;

			RefreshTable(now);

			SetTitle(wxT("ClauExplorer : ") + _fileName);
		}
		openFileDialog->Destroy();
	}
	virtual void FileSaveMenuOnMenuSelection(wxCommandEvent& event) { 
		if (!isMain) { return; }
		wxFileDialog* saveFileDialog = new wxFileDialog(this, _("Save"), "", "",
				"", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

		if (saveFileDialog->ShowModal() == wxID_OK)
		{
			string fileName(saveFileDialog->GetPath().c_str());

			wiz::load_data::LoadData::SaveWizDB(global, fileName, "1");
		}
		saveFileDialog->Destroy();
	}
	virtual void FileExitMenuOnMenuSelection(wxCommandEvent& event) { Close(true);  }
	virtual void InsertMenuOnMenuSelection(wxCommandEvent& event) {
		
		if (1 == view_mode) { return; }
		if (-1 == position) { 
			ChangeWindow* changeWindow = new ChangeWindow(this, now, 0, std::max<int>(0, now->GetIListSize()), 2, view_mode);

			changeWindow->Show();
			return;
		}

		int idx = position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * dataViewListCtrlNo;
		bool isUserType = now->IsUserTypeList(idx);

		if (dataViewListCtrlNo == -1) { return; }

		ChangeWindow* changeWindow = new ChangeWindow(this, now, isUserType, idx, 2, view_mode);

		changeWindow->Show();
	}
	virtual void ChangeMenuOnMenuSelection(wxCommandEvent& event) { 
		if (-1 == position) { return; }
		
		if (1 == view_mode) {
			int idx = position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * dataViewListCtrlNo;
			bool isUserType = (idx < now->GetUserTypeListSize());

			ChangeWindow* changeWindow = new ChangeWindow(this, now, isUserType,
				isUserType ? idx : idx - now->GetUserTypeListSize(), 1, view_mode);

			changeWindow->Show();
		}
		else {
			int idx = position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * dataViewListCtrlNo;
			bool isUserType = now->IsUserTypeList(idx);

			ChangeWindow* changeWindow = new ChangeWindow(this, now, isUserType,
				isUserType? now->GetUserTypeIndexFromIlistIndex(idx) : now->GetItemIndexFromIlistIndex(idx), 1, view_mode);

			changeWindow->Show();
		}
	}
	virtual void RemoveMenuOnMenuSelection(wxCommandEvent& event) { 
		if (-1 == position) { return; }
		int idx = position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * dataViewListCtrlNo;
		int type = 1;
		
		if (1 == view_mode) {
			bool isUserType = (idx < now->GetUserTypeListSize());

			if (isUserType) {
				now->RemoveUserTypeList(idx);
			}
			else {
				now->RemoveItemList(idx - now->GetUserTypeListSize());
			}
			RefreshTable(now);
		}
		else {
			bool isUserType = now->IsUserTypeList(idx);

			if (isUserType) {
				now->RemoveUserTypeList(now->GetUserTypeIndexFromIlistIndex(idx));
			}
			else {
				now->RemoveItemList(now->GetItemIndexFromIlistIndex(idx));
			}
			RefreshTable(now);
		}
	}
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
		if (1 == view_mode && NK_ENTER == event.GetKeyCode() && position >= 0 && position < now->GetUserTypeListSize()) {
			now = now->GetUserTypeList(position);
			RefreshTable(now);
		}
		else  if (2 == view_mode && NK_ENTER == event.GetKeyCode() && position >= 0 && position < m_dataViewListCtrl1->GetItemCount()) {
			if (now->IsUserTypeList(position)) {
				const int idx = now->GetUserTypeIndexFromIlistIndex(position);
				now = now->GetUserTypeList(idx);
				RefreshTable(now);
			}
		}
		else if (NK_BACKSPACE == event.GetKeyCode() && now->GetParent() != nullptr) {
			now = now->GetParent();
			RefreshTable(now);
		}
		else {
			wxDataViewListCtrl* ctrl[4];
			ctrl[0] = m_dataViewListCtrl1;
			ctrl[1] = m_dataViewListCtrl2;
			ctrl[2] = m_dataViewListCtrl3;
			ctrl[3] = m_dataViewListCtrl4;

			ctrl[dataViewListCtrlNo]->UnselectRow(position);

			if (WXK_UP == event.GetKeyCode() && dataViewListCtrlNo > -1 && position > 0)//< ctrl[dataViewListCtrlNo]->GetItemCount())
			{
				event.Skip();
				return;
			}
			else if (WXK_DOWN == event.GetKeyCode() && dataViewListCtrlNo > -1 && position >= 0 && position < ctrl[dataViewListCtrlNo]->GetItemCount() - 1)
			{
				event.Skip();
				return;
			}
			else if (WXK_LEFT == event.GetKeyCode() && dataViewListCtrlNo > 0 && position >= 0 && position < ctrl[dataViewListCtrlNo - 1]->GetItemCount())
			{
				dataViewListCtrlNo--;
			}
			else if (WXK_RIGHT == event.GetKeyCode() && dataViewListCtrlNo < 3 && position >= 0 && position < ctrl[dataViewListCtrlNo + 1]->GetItemCount())
			{
				dataViewListCtrlNo++;
			}

			ctrl[dataViewListCtrlNo]->SelectRow(position);
			ctrl[dataViewListCtrlNo]->SetFocus();
		}
	}
	virtual void m_dataViewListCtrl2OnChar(wxKeyEvent& event) { 
		dataViewListCtrlNo = 1; position = m_dataViewListCtrl2->GetSelectedRow();
		if (1 == view_mode && NK_ENTER == event.GetKeyCode() && dataViewListCtrlNo == 1 && position >= 0 && position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) <  now->GetUserTypeListSize()) {
			now = now->GetUserTypeList(position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4));
			RefreshTable(now);
		}
		else  if (2 == view_mode && NK_ENTER == event.GetKeyCode() && position >= 0 && position < m_dataViewListCtrl2->GetItemCount()) {
			const int pos = (position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4));
			if (now->IsUserTypeList(pos)) {
				const int idx = now->GetUserTypeIndexFromIlistIndex(pos);
				now = now->GetUserTypeList(idx);
				RefreshTable(now);
			}
		}
		else if (NK_BACKSPACE == event.GetKeyCode() && now->GetParent() != nullptr) {
			now = now->GetParent();
			RefreshTable(now);
		}
		else {
			wxDataViewListCtrl* ctrl[4];
			ctrl[0] = m_dataViewListCtrl1;
			ctrl[1] = m_dataViewListCtrl2;
			ctrl[2] = m_dataViewListCtrl3;
			ctrl[3] = m_dataViewListCtrl4;

			ctrl[dataViewListCtrlNo]->UnselectRow(position);

			if (WXK_UP == event.GetKeyCode() && dataViewListCtrlNo > -1 && position > 0)//< ctrl[dataViewListCtrlNo]->GetItemCount())
			{
				event.Skip();
				return;
			}
			else if (WXK_DOWN == event.GetKeyCode() && dataViewListCtrlNo > -1 && position >= 0 && position < ctrl[dataViewListCtrlNo]->GetItemCount() - 1)
			{
				event.Skip();
				return;
			}
			else if (WXK_LEFT == event.GetKeyCode() && dataViewListCtrlNo > 0 && position >= 0 && position < ctrl[dataViewListCtrlNo - 1]->GetItemCount())
			{
				dataViewListCtrlNo--;
			}
			else if (WXK_RIGHT == event.GetKeyCode() && dataViewListCtrlNo < 3 && position >= 0 && position < ctrl[dataViewListCtrlNo + 1]->GetItemCount())
			{
				dataViewListCtrlNo++;
			}

			ctrl[dataViewListCtrlNo]->SelectRow(position);
			ctrl[dataViewListCtrlNo]->SetFocus();
		}

	}
	virtual void m_dataViewListCtrl3OnChar(wxKeyEvent& event) {
		dataViewListCtrlNo = 2; position = m_dataViewListCtrl3->GetSelectedRow();
		if (1 == view_mode && NK_ENTER == event.GetKeyCode() && dataViewListCtrlNo == 2 && position >= 0 && position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * 2 < now->GetUserTypeListSize()) {
			now = now->GetUserTypeList(position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * 2);
			RefreshTable(now);
		}
		else  if (2 == view_mode && NK_ENTER == event.GetKeyCode() && position >= 0 && position < m_dataViewListCtrl3->GetItemCount()) {
			const int pos = (position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * 2);
			if (now->IsUserTypeList(pos)) {
				const int idx = now->GetUserTypeIndexFromIlistIndex(pos);
				now = now->GetUserTypeList(idx);
				RefreshTable(now);
			}
		}
		else if (NK_BACKSPACE == event.GetKeyCode() && now->GetParent() != nullptr) {
			now = now->GetParent();
			RefreshTable(now);
		}
		else {
			wxDataViewListCtrl* ctrl[4];
			ctrl[0] = m_dataViewListCtrl1;
			ctrl[1] = m_dataViewListCtrl2;
			ctrl[2] = m_dataViewListCtrl3;
			ctrl[3] = m_dataViewListCtrl4;

			ctrl[dataViewListCtrlNo]->UnselectRow(position);

			if (WXK_UP == event.GetKeyCode() && dataViewListCtrlNo > -1 && position > 0)//< ctrl[dataViewListCtrlNo]->GetItemCount())
			{
				event.Skip();
				return;
			}
			else if (WXK_DOWN == event.GetKeyCode() && dataViewListCtrlNo > -1 && position >= 0 && position < ctrl[dataViewListCtrlNo]->GetItemCount() - 1)
			{
				event.Skip();
				return;
			}
			else if (WXK_LEFT == event.GetKeyCode() && dataViewListCtrlNo > 0 && position >= 0 && position < ctrl[dataViewListCtrlNo - 1]->GetItemCount())
			{
				dataViewListCtrlNo--;
			}
			else if (WXK_RIGHT == event.GetKeyCode() && dataViewListCtrlNo < 3 && position >= 0 && position < ctrl[dataViewListCtrlNo + 1]->GetItemCount())
			{
				dataViewListCtrlNo++;
			}

			ctrl[dataViewListCtrlNo]->SelectRow(position);
			ctrl[dataViewListCtrlNo]->SetFocus();
		}
	}
	virtual void m_dataViewListCtrl4OnChar(wxKeyEvent& event) {
		dataViewListCtrlNo = 3; position = m_dataViewListCtrl4->GetSelectedRow();
		if (1 == view_mode && NK_ENTER == event.GetKeyCode() && dataViewListCtrlNo == 3 && position >= 0 && position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * 3 < now->GetUserTypeListSize()) {
			now = now->GetUserTypeList(position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * 3);
			RefreshTable(now);
		}
		else  if (2 == view_mode && NK_ENTER == event.GetKeyCode() && position >= 0 && position < m_dataViewListCtrl4->GetItemCount()) {
			const int pos = (position + ((now->GetUserTypeListSize() + now->GetItemListSize()) / 4) * 3);
			if (now->IsUserTypeList(pos)) {
				const int idx = now->GetUserTypeIndexFromIlistIndex(pos);
				now = now->GetUserTypeList(idx);
				RefreshTable(now);
			}
		}
		else if (NK_BACKSPACE == event.GetKeyCode() && now->GetParent() != nullptr) {
			now = now->GetParent();
			RefreshTable(now);
		}
		else {
			wxDataViewListCtrl* ctrl[4];
			ctrl[0] = m_dataViewListCtrl1;
			ctrl[1] = m_dataViewListCtrl2;
			ctrl[2] = m_dataViewListCtrl3;
			ctrl[3] = m_dataViewListCtrl4;

			ctrl[dataViewListCtrlNo]->UnselectRow(position);

			if (WXK_UP == event.GetKeyCode() && dataViewListCtrlNo > -1 && position > 0)//< ctrl[dataViewListCtrlNo]->GetItemCount())
			{
				event.Skip();
				return;
			}
			else if (WXK_DOWN == event.GetKeyCode() && dataViewListCtrlNo > -1 && position >= 0 && position < ctrl[dataViewListCtrlNo]->GetItemCount() - 1)
			{
				event.Skip();
				return;
			}
			else if (WXK_LEFT == event.GetKeyCode() && dataViewListCtrlNo > 0 && position >= 0 && position < ctrl[dataViewListCtrlNo - 1]->GetItemCount())
			{
				dataViewListCtrlNo--;
			}
			else if (WXK_RIGHT == event.GetKeyCode() && dataViewListCtrlNo < 3 && position >= 0 && position < ctrl[dataViewListCtrlNo + 1]->GetItemCount())
			{
				dataViewListCtrlNo++;
			}

			ctrl[dataViewListCtrlNo]->SelectRow(position);
			ctrl[dataViewListCtrlNo]->SetFocus();
		}
	}

	virtual void m_dataViewListCtrl1OnDataViewListCtrlSelectionChanged(wxDataViewEvent& event) {
		dataViewListCtrlNo = 0;
		position = m_dataViewListCtrl1->GetSelectedRow();
	}
	virtual void m_dataViewListCtrl2OnDataViewListCtrlSelectionChanged(wxDataViewEvent& event) {
		dataViewListCtrlNo = 1;
		position = m_dataViewListCtrl2->GetSelectedRow();
	}
	virtual void m_dataViewListCtrl3OnDataViewListCtrlSelectionChanged(wxDataViewEvent& event) {
		dataViewListCtrlNo = 2;
		position = m_dataViewListCtrl3->GetSelectedRow();
	}
	virtual void m_dataViewListCtrl4OnDataViewListCtrlSelectionChanged(wxDataViewEvent& event) {
		dataViewListCtrlNo = 3;
		position = m_dataViewListCtrl4->GetSelectedRow();
	}

	virtual void DefaultViewMenuOnMenuSelection(wxCommandEvent& event) {
		view_mode = 1;
		m_statusBar1->SetLabelText(wxT("View Mode A"));
		if (now) {
			RefreshTable(now);
		}
	}
	virtual void IListViewMenuOnMenuSelection(wxCommandEvent& event) {
		view_mode = 2;
		m_statusBar1->SetLabelText(wxT("View Mode B"));
		if (now) {
			RefreshTable(now);
		}
	}

	virtual void OtherWindowMenuOnMenuSelection(wxCommandEvent& event) { 
		if (!isMain) { return; }
		MainFrame* frame = new MainFrame(this);
		frame->view_mode = this->view_mode;
		frame->now = this->now;
		frame->RefreshTable(frame->now);

		frame->SetTitle(GetTitle() + wxT(" : other window"));

		frame->Show(true);
	}

public:

	MainFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ClauExplorer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(789, 512), long style = wxDEFAULT_FRAME_STYLE | wxTAB_TRAVERSAL);

	~MainFrame();

	void FirstFrame() {
		isMain = true;
	}
};

MainFrame::MainFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	now = &global;


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

	ViewMenu = new wxMenu();
	wxMenuItem* DefaultViewMenu;
	DefaultViewMenu = new wxMenuItem(ViewMenu, wxID_ANY, wxString(wxT("ViewA")), wxEmptyString, wxITEM_NORMAL);
	ViewMenu->Append(DefaultViewMenu);

	wxMenuItem* IListViewMenu;
	IListViewMenu = new wxMenuItem(ViewMenu, wxID_ANY, wxString(wxT("ViewB")), wxEmptyString, wxITEM_NORMAL);
	ViewMenu->Append(IListViewMenu);

	menuBar->Append(ViewMenu, wxT("View"));


	WindowMenu = new wxMenu();
	wxMenuItem* OtherWindowMenu;
	OtherWindowMenu = new wxMenuItem(WindowMenu, wxID_ANY, wxString(wxT("OtherWindow")), wxEmptyString, wxITEM_NORMAL);
	WindowMenu->Append(OtherWindowMenu);

	menuBar->Append(WindowMenu, wxT("Window"));


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

	m_statusBar1 = this->CreateStatusBar(1, wxST_SIZEGRIP, wxID_ANY);

	m_statusBar1->SetLabelText(wxT("View Mode A"));

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
	

	this->Connect(DefaultViewMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::DefaultViewMenuOnMenuSelection));
	this->Connect(IListViewMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::IListViewMenuOnMenuSelection));
	
	back_button->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::back_buttonOnButtonClick), NULL, this);
	dir_text->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(MainFrame::dir_textOnTextEnter), NULL, this);
	refresh_button->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrame::refresh_buttonOnButtonClick), NULL, this);
	
	m_dataViewListCtrl1->Connect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl1OnChar), NULL, this);
	m_dataViewListCtrl2->Connect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl2OnChar), NULL, this);
	m_dataViewListCtrl3->Connect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl3OnChar), NULL, this);
	m_dataViewListCtrl4->Connect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl4OnChar), NULL, this);

	m_dataViewListCtrl1->Connect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(MainFrame::m_dataViewListCtrl1OnDataViewListCtrlSelectionChanged), NULL, this);
	m_dataViewListCtrl2->Connect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(MainFrame::m_dataViewListCtrl2OnDataViewListCtrlSelectionChanged), NULL, this);
	m_dataViewListCtrl3->Connect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(MainFrame::m_dataViewListCtrl3OnDataViewListCtrlSelectionChanged), NULL, this);
	m_dataViewListCtrl4->Connect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(MainFrame::m_dataViewListCtrl4OnDataViewListCtrlSelectionChanged), NULL, this);
	
	this->Connect(OtherWindowMenu->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OtherWindowMenuOnMenuSelection));
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

	this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::DefaultViewMenuOnMenuSelection));
	this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::IListViewMenuOnMenuSelection));

	m_dataViewListCtrl1->Disconnect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl1OnChar), NULL, this);
	m_dataViewListCtrl2->Disconnect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl2OnChar), NULL, this);
	m_dataViewListCtrl3->Disconnect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl3OnChar), NULL, this);
	m_dataViewListCtrl4->Disconnect(wxEVT_CHAR, wxKeyEventHandler(MainFrame::m_dataViewListCtrl4OnChar), NULL, this);

	m_dataViewListCtrl1->Disconnect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(MainFrame::m_dataViewListCtrl1OnDataViewListCtrlSelectionChanged), NULL, this);
	m_dataViewListCtrl2->Disconnect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(MainFrame::m_dataViewListCtrl2OnDataViewListCtrlSelectionChanged), NULL, this);
	m_dataViewListCtrl3->Disconnect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(MainFrame::m_dataViewListCtrl3OnDataViewListCtrlSelectionChanged), NULL, this);
	m_dataViewListCtrl4->Disconnect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(MainFrame::m_dataViewListCtrl4OnDataViewListCtrlSelectionChanged), NULL, this);

	this->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OtherWindowMenuOnMenuSelection));
}

class TestApp : public wxApp {
public:
	virtual bool OnInit() {
		MainFrame* frame = new MainFrame(nullptr);
		frame->FirstFrame();
		frame->Show(true);
		return true;
	}
};

IMPLEMENT_APP(TestApp)

