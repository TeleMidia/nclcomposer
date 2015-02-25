#include "../../include/gui/EPGTreeView.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
   	
	EPGTreeView::EPGTreeView() {
		cout << "EPGTreeView::EPGTreeView(" << this << ")" << endl;

		refTreeModel = Gtk::TreeStore::create(columnModel);
		set_model(refTreeModel);

		Gtk::Menu::MenuList& menulist = popupMenu.items();
		menulist.push_back(
			    Gtk::Menu_Helpers::MenuElem(_("_Add"), sigc::mem_fun(*this,
			    	    &EPGTreeView::addRow)));

		menulist.push_back(
				Gtk::Menu_Helpers::MenuElem(_("_Remove"), sigc::mem_fun(*this,
					    &EPGTreeView::removeRow)));

		menulist.push_back(
				Gtk::Menu_Helpers::MenuElem(_("_Encode"), sigc::mem_fun(*this,
						&EPGTreeView::encodeEIT)));

		//Creating combo to ProgramNumber
		refComboBoxModel = Gtk::ListStore::create(comboBoxModel);

		//Appending programnumber combo
		Gtk::TreeView::Column* column = manage(
				new Gtk::TreeView::Column(_("Program Number")));

		Gtk::CellRendererCombo* cellRenderer = manage(
				new Gtk::CellRendererCombo());

		column->pack_start(*cellRenderer);
		(*this).append_column(*column);

		//Setting ProgramNumber Combo
		column->add_attribute(*cellRenderer,"text",columnModel.programNumber);
		cellRenderer->set_property("model", refComboBoxModel);
		cellRenderer->set_property("editable", true);
		cellRenderer->set_property("text_column", 0);
		cellRenderer->set_property("visible", true);
		cellRenderer->signal_edited().connect(
				sigc::mem_fun(*this, &EPGTreeView::onEditingProgramNumberCell));

		//Appending columns
		append_column_editable(_("Start"), columnModel.start);
		append_column_editable(_("Duration"), columnModel.duration);

		//Creating combo to language
		refComboBoxModelLang = Gtk::ListStore::create(comboBoxModelLang);

		//Appending language combo
		Gtk::TreeView::Column* columnLang = manage(
				new Gtk::TreeView::Column(_("Language code")));

		Gtk::CellRendererCombo* cellRendererLang = manage(
				new Gtk::CellRendererCombo());

		columnLang->pack_start(*cellRendererLang);
		(*this).append_column(*columnLang);

		//Setting Language Combo
		columnLang->add_attribute(
				*cellRendererLang,"text",columnModel.language);

		cellRendererLang->set_property("model", refComboBoxModelLang);
		cellRendererLang->set_property("editable", true);
		cellRendererLang->set_property("text_column", 0);
		cellRendererLang->set_property("visible", true);
		cellRendererLang->signal_edited().connect(
				sigc::mem_fun(*this, &EPGTreeView::onEditingLanguageCell));

		append_column_editable(_("Name"), columnModel.name);
		append_column_editable(_("Short description"), columnModel.description);

		updateProgramNumbers();

		Gtk::TreeModel::Row row = *(refComboBoxModelLang->append());
		FILE *fp;
		if ((fp = fopen("ISO639-2B.txt","r"))!=NULL) {
			while (!feof(fp)) {
				char lang[255];
				row = *(refComboBoxModelLang->append());
				fgets(lang, 255, fp);
				lang[3] = 0;
				row[comboBoxModelLang.code] = lang;
			}
			fclose(fp);
			cout << "Language list loaded." << endl;

		} else {
			cout << "Language list not loaded." << endl;
		}
	}
	
	EPGTreeView::~EPGTreeView() {

	}
	
	bool EPGTreeView::on_button_press_event(GdkEventButton* event) {
		TreeView::on_button_press_event(event);
		if((event->type == GDK_BUTTON_PRESS) && (event->button == 3)){
			popupMenu.popup(event->button,event->time);
			return true;
		}
		return false;
	}

	void EPGTreeView::addRow() {
		std::stringstream str ;
		Gtk::TreeModel::Row row = *(refTreeModel->append());
		row[columnModel.start] = "2008/01/01 00:00:00";
		row[columnModel.duration] = "01:00:00";
		row[columnModel.language] = "por";
		row[columnModel.name] = "Untitled";
		row[columnModel.description] = "No description available.";
		str<< std::hex << std::uppercase << Muxer::getInstance()->getPat()->getFirstProgramNumber();
		row[columnModel.programNumber] = Glib::filename_to_utf8("0x" + str.str());
	}

	void EPGTreeView::removeRow() {
		Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = get_selection();
		Gtk::TreeIter iter = refTreeSelection->get_selected();
		if (iter) //If anything is selected
		{
			Gtk::Window* parentWindow = dynamic_cast<Gtk::Window*>(get_toplevel());
			Gtk::MessageDialog dialog(*parentWindow,_("Do you want to remove this event?"), false,
					Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
			dialog.set_secondary_text(_("Caution! This event will be permanently removed."));
			if(dialog.run() == Gtk::RESPONSE_CANCEL)
				return;
			refTreeModel->erase(iter);
		}
	}
	
	void EPGTreeView::encodeEIT() {
		cout << "EPGTreeView::encodeEIT(" << this << ")" << endl;
		int year, month, day, hour, minute, second, id, tsId;
		struct tm startTime, duration;
		//unsigned int esPid;
		::si::eit::EventInfoTable eit;
		Gtk::TreeIter iter = refTreeModel->children().begin();
		
		id = 1;
		tsId = Muxer::getInstance()->getTSId();
		eit.setTransportStreamId(tsId);//pat
		//TODO: could be more than one program number.
		//considering the first standard program number only.
		eit.setExtensionId(Muxer::getInstance()->getPat()->getFirstProgramNumber()); 
		eit.setOriginalNetworkId(1);//1
		eit.setTableId(0x4E);//norma
		eit.setVersionNumber(0);//norma
		eit.setSectionSyntaxIndicator(true);//fixo
		eit.setLastTableId(0x4E);//norma

		while(iter){
			Gtk::TreeRow row = *iter;
			sscanf(((string)(row[columnModel.start])).c_str(), "%d/%d/%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
			startTime.tm_year = year;
			startTime.tm_mon = month;
			startTime.tm_mday = day;
			startTime.tm_hour = hour;
			startTime.tm_min = minute;
			startTime.tm_sec = second;
			sscanf(((string)(row[columnModel.duration])).c_str(), "%d:%d:%d", &hour, &minute, &second);
			duration.tm_hour = hour;
			duration.tm_min = minute;
			duration.tm_sec = second;
			eit.insertEvent(startTime, duration);
			eit.insertShortEventDescriptor(id, (string)row[columnModel.language], (string) row[columnModel.name], (string)row[columnModel.description]);
			iter++;
			id++;
		}
		//esPid = Muxer::getInstance()->getPat()->getNextPid();
		cout << "EPGTreeView::encodeEIT() '";
		cout << EIT_ES_PATH + intToStrHexa(0x12) + ".ts'" << endl;
		eit.encodeStream(false, EIT_ES_PATH + intToStrHexa(0x12) + ".ts");
		//clear all treeview
		//insert pid in muxer page
	}

	void EPGTreeView::updateProgramNumbers() {
		std::map<unsigned int, Pmt*>::iterator iter;
		std::map<unsigned int, Pmt*>* programs;

		programs = Muxer::getInstance()->getProgramsInfo();
		if (programs == NULL) {
			return;
		}
		refComboBoxModel->clear();
		iter = programs->begin();
		while (iter != programs->end()) {
			int programNumber = iter->second->getProgramNumber();
			std::stringstream str ;
			str<< std::hex << std::uppercase << programNumber;
			Gtk::TreeModel::Row row = *(refComboBoxModel->append());
			row[comboBoxModel.programNumberChoice] = Glib::filename_to_utf8(
					"0x" + str.str());

			++iter;
		}
	}
	
	void EPGTreeView::onEditingProgramNumberCell(
			const Glib::ustring& path_string, const Glib::ustring& new_text) {

		Gtk::TreePath path(path_string);
		Gtk::TreeModel::iterator iter = refTreeModel->get_iter(path);
		std::stringstream str1(new_text.raw());
		int programNumber;
		if(new_text.find("0x") != Glib::ustring::npos){
			if(!(str1 >> std::hex >> programNumber)){
				programNumber = 0; 
			}
		}else{
			if(!(str1 >> programNumber)){
				programNumber = 0; 
			}
		}
		if(iter){
			Gtk::TreeRow row = *iter;
			std::stringstream str2 ;
			str2<< std::hex << std::uppercase << programNumber;
			
			row[columnModel.programNumber] = Glib::filename_to_utf8("0x" + str2.str());
		}
	}

	void EPGTreeView::onEditingLanguageCell(
			const Glib::ustring& path_string, const Glib::ustring& new_text) {

		Gtk::TreePath path(path_string);
		Gtk::TreeModel::iterator iter = refTreeModel->get_iter(path);

		if (iter) {
			Gtk::TreeRow row = *iter;
			row[columnModel.language] = new_text;
		}
	}
   }
  }
 }
}
