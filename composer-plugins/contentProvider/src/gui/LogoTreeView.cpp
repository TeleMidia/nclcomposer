#include "../../include/gui/LogoTreeView.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
   
	LogoTreeView::LogoTreeView() {
		cout << "LogoTreeView::LogoTreeView(" << this << ")" << endl;

		refTreeModel = Gtk::TreeStore::create(columnModel);
		set_model(refTreeModel);

		Gtk::Menu::MenuList& menulist = popupMenu.items();
		menulist.push_back(
			    Gtk::Menu_Helpers::MenuElem(_("_Add"), sigc::mem_fun(*this,
			    	    &LogoTreeView::addRow)));

		menulist.push_back(
				Gtk::Menu_Helpers::MenuElem(_("_Remove"), sigc::mem_fun(*this,
					    &LogoTreeView::removeRow)));

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
				sigc::mem_fun(*this, &LogoTreeView::onEditingProgramNumberCell));

		//Appending columns
		append_column_editable(_("Logo filename"), columnModel.logoFilename);
	}
	
	LogoTreeView::~LogoTreeView() {

	}
	
	bool LogoTreeView::on_button_press_event(GdkEventButton* event) {
		TreeView::on_button_press_event(event);
		if((event->type == GDK_BUTTON_PRESS) && (event->button == 3)){
			popupMenu.popup(event->button,event->time);
			return true;
		}
		Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = get_selection();
		Gtk::TreeIter iter = refTreeSelection->get_selected();
		if ((iter) && (event->type == GDK_2BUTTON_PRESS)) {
			onAddButtonClick(iter);
			cout << "double-click." << endl;
			return true;
		}

		return false;
	}

	void LogoTreeView::addRow() {
		std::stringstream str ;
		Gtk::TreeModel::Row row = *(refTreeModel->append());
		row[columnModel.logoFilename] = "c:\\";
		str<< std::hex << std::uppercase << Muxer::getInstance()->getPat()->getFirstProgramNumber();
		row[columnModel.programNumber] = Glib::filename_to_utf8("0x" + str.str());
	}

	void LogoTreeView::removeRow() {
		Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = get_selection();
		Gtk::TreeIter iter = refTreeSelection->get_selected();
		if (iter) //If anything is selected
		{
			Gtk::Window* parentWindow = dynamic_cast<Gtk::Window*>(get_toplevel());
			Gtk::MessageDialog dialog(*parentWindow,_("Do you want to remove this logo?"), false,
					Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
			dialog.set_secondary_text(_("Caution! This logo will be permanently removed."));
			if(dialog.run() == Gtk::RESPONSE_CANCEL)
				return;
			refTreeModel->erase(iter);
		}
	}
	
	bool LogoTreeView::validatePNG() {
		FILE* png;
		size_t rc;
		bool checked = true;
		char pngData[16];
		Gtk::TreeIter iter = refTreeModel->children().begin();
		while(iter){
			Gtk::TreeRow row = *iter;
			
			png = fopen(((string)(row[columnModel.logoFilename])).c_str(), "rb");
			if (png == NULL)
			{
				cout << ((string)(row[columnModel.logoFilename])).c_str() <<
					" file does not exists." << endl;
				return false;
			}
			rc = fread(pngData, 1, 16, png);
			if (((rc != 16) ||
					((pngData[0] & 0xFF) != 0x89) ||
					((pngData[1] & 0xFF) != 0x50) ||
					((pngData[2] & 0xFF) != 0x4E) ||
					((pngData[3] & 0xFF) != 0x47) ||
					((pngData[4] & 0xFF) != 0x0D) ||
					((pngData[5] & 0xFF) != 0x0A) ||
					((pngData[6] & 0xFF) != 0x1A) ||
					((pngData[7] & 0xFF) != 0x0A) ||
					((pngData[8] & 0xFF) != 0x00) ||
					((pngData[9] & 0xFF) != 0x00) ||
					((pngData[10] & 0xFF) != 0x00) ||
					((pngData[11] & 0xFF) != 0x0D) ||
					((pngData[12] & 0xFF) != 0x49) ||
					((pngData[13] & 0xFF) != 0x48) ||
					((pngData[14] & 0xFF) != 0x44) ||
					((pngData[15] & 0xFF) != 0x52))) {
				cout << "Invalid PNG file: (" <<
					((string)(row[columnModel.logoFilename])).c_str() <<
						")" << endl;
				checked = false;
			}
			fclose(png);
			if (!checked) {
				return false;
			}
			iter++;
		}
		return checked;
	}
	
	void LogoTreeView::updateProgramNumbers() {
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
	
	void LogoTreeView::onEditingProgramNumberCell(
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
	
	void LogoTreeView::onAddButtonClick(Gtk::TreeIter iter) {
		Gtk::FileChooserDialog dialog(
			    "Please choose a media file", Gtk::FILE_CHOOSER_ACTION_OPEN);

		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

		//Add filters, so that only certain file types can be selected:
		Gtk::FileFilter filter_png;
		filter_png.set_name("PNG files");
		filter_png.add_pattern("*.png");
		dialog.add_filter(filter_png);

		//Show the dialog and wait for a user response:
		int result = dialog.run();
		string fileName;
		//Handle the response:
		if (result == Gtk::RESPONSE_OK) {
			cout << "Open clicked." << endl;

			fileName = dialog.get_filename();
			cout << "File selected: " <<  fileName << endl;
			Gtk::TreeRow row = *iter;
			row[columnModel.logoFilename] = fileName;
			//if (Muxer::getInstance()->openTSFile(fileName)) {
				//updateGrid();
			//}
		}
	}
	
	bool LogoTreeView::encodeLogo() {
		static int ddid = 0, oldDdid, id = 0;
		bool append = false;
		unsigned int tsId, esPid;
		//CDT
		if (validatePNG()) {
			::si::cdt::CommonDataTable cdt;
			Gtk::TreeIter iter = refTreeModel->children().begin();
			oldDdid = ddid;
			tsId = Muxer::getInstance()->getTSId();

			while(iter){
				Gtk::TreeRow row = *iter;
				id = 1;
				cdt.setDataType(0x01);
				cdt.loadPNG(((string)(row[columnModel.logoFilename])).c_str());
				cdt.setExtensionId(ddid++);
				cdt.setOriginalNetworkId(1);//1
				cdt.setTableId(0xC8);//norma
				cdt.setVersionNumber(0);//norma
				cdt.setSectionSyntaxIndicator(true);//fixo
				cout << "LogoTreeView::encodeLogo(CDT) ";
				cout << "with download_data_id = " << ddid << " '";
				cout << CDT_ES_PATH + intToStrHexa(0x29) + ".ts'" << endl;
				cdt.encodeStream(append, CDT_ES_PATH + intToStrHexa(0x29) + ".ts");
				append = true;
				iter++;
			}
		}
		//SDT
		append = false;
		ddid = oldDdid;
		::si::sdt::ServiceDescriptionTable sdt;
		sdt.setExtensionId(tsId); 
		sdt.setOriginalNetworkId(1);
		sdt.setTableId(0x42);
		sdt.setVersionNumber(0);
		sdt.setSectionSyntaxIndicator(true);
		Gtk::TreeIter iter = refTreeModel->children().begin();
		while(iter){
			Gtk::TreeRow row = *iter;
			sscanf(((string)(row[columnModel.programNumber])).c_str(), "%d", &esPid);
			sdt.insertEvent(esPid, true, false, 0, false);
			sdt.insertLogoTransmissionDescriptor(esPid, 0x01, id++, 0, ddid, "");
			iter++;
		}
		cout << "LogoTreeView::encodeLogo(SDT) '";
		cout << SDT_ES_PATH + intToStrHexa(0x11) + ".ts'" << endl;
		sdt.encodeStream(append, SDT_ES_PATH + intToStrHexa(0x11) + ".ts");
		return true;
	}
   }
  }
 }
}
