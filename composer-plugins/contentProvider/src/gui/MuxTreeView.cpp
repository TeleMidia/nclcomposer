#include "../../include/gui/MuxTreeView.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
  	MuxTreeView::MuxTreeView() {
  		//Create the Tree model:
		refTreeModel = MuxTreeStore::create(grid);
		set_model(refTreeModel);

		//All the items to be reordered with drag-and-drop:
		set_reorderable();

		//Connect signal:
		signal_row_activated().connect(sigc::mem_fun(
			    *this, &MuxTreeView::onTreeViewRowActivated));

		//Add the TreeView's view columns:
		append_column_numeric(_("Pid"), grid.id,"0x%X");
		append_column_editable(_("Mux Me"), grid.transmit);
		append_column(_("Elementary Stream Data"), grid.name);

  		//Create popup (right click)
  		Gtk::Menu::MenuList& menulist = muxPopupMenu.items();

		menulist.push_back(
			    Gtk::Menu_Helpers::MenuElem(_("_Add"), sigc::mem_fun(*this,
			    	    &MuxTreeView::onAddButtonClick)));

		menulist.push_back(
			    Gtk::Menu_Helpers::MenuElem(_("_Remove"), sigc::mem_fun(*this,
			    	    &MuxTreeView::onRemoveButtonClick)));

		muxPopupMenu.accelerate(*this);
  	}

	MuxTreeView::~MuxTreeView() {
		
	}

	void MuxTreeView::updateGrid() {
		unsigned int tsId, programNumber, pcr, versionNumber;
		std::string fileName;

		std::map<unsigned int, Pmt*>* programs;
		std::map<unsigned int, Pmt*>::iterator i;

		std::map<unsigned int, short>* streams;
		std::map<unsigned int, short>::iterator j;

		Muxer::getInstance()->checkConsistency();
		tsId = Muxer::getInstance()->getTSId();
		fileName = Muxer::getInstance()->getTSFileName();
		programs = Muxer::getInstance()->getProgramsInfo();
		if (programs == NULL) {
			return;
		}

		refTreeModel->clear();

		//Fill the TreeView's model
		Gtk::TreeModel::Row row = *(refTreeModel->append());
		row[grid.id] = 0x00;
		row[grid.name] = "Transport Stream Id '" + intToStrHexa(
				tsId) + "' src='" + fileName + "'";

		row[grid.transmit] = true;

		i = programs->begin();
		while (i != programs->end()) {
			Gtk::TreeModel::Row childrow = 
				    *(refTreeModel->append(row.children()));

			childrow[grid.id] = i->first;

			programNumber = i->second->getProgramNumber();

			childrow[grid.name] = "Program Map Table for Program Number " + 
				    	intToStrHexa(programNumber);

			childrow[grid.transmit] = true;

			streams = i->second->getStreamsInformation();
			if (streams == NULL) {
				programs->erase(i);
				return;
			}

			for (j = streams->begin(); j != streams->end(); ++j) {
				Gtk::TreeModel::Row grandchildrow = 
				    *(refTreeModel->append(childrow.children()));

				grandchildrow[grid.id] = j->first;
				grandchildrow[grid.name] = i->second->
					    getStreamTypeName(j->second);

				grandchildrow[grid.transmit] = true;
			}

			pcr = i->second->getPCRPid();
			if (pcr != 0) {
				Gtk::TreeModel::Row grandchildrow = 
				    *(refTreeModel->append(childrow.children()));

				grandchildrow[grid.id] = pcr;
				grandchildrow[grid.name] = "PCR Stream";
				grandchildrow[grid.transmit] = true;
			}

			versionNumber = i->second->getVersionNumber() + 1;
			if (versionNumber > 16) {
				versionNumber = 0;
			}
			i->second->setVersionNumber(versionNumber);

			PMTStreamFactory::createPMTStream(
					PMT_ES_PATH + intToStrHexa(i->first) + ".ts",
					i->first, programNumber, pcr, versionNumber,
					streams, i->second->getCarousels(),
					i->second->getStreamEvents());

			++i;
		}
	}

	void MuxTreeView::onMenuFileSaveAs() {
		Gtk::FileChooserDialog dialog(
			    "Save current TS to disk ", Gtk::FILE_CHOOSER_ACTION_SAVE);

		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);

		cout << "muxpage1 err = '" << strerror(errno) << "'" << endl;
		int result = dialog.run();
		string fileName, currentFileName;
		Gtk::Window* parentWindow;

		if (result == Gtk::RESPONSE_OK) {
			fileName = dialog.get_filename();
			currentFileName = Muxer::getInstance()->getTSFileName();

			if (currentFileName == "") {
				parentWindow = dynamic_cast<Gtk::Window*>(get_toplevel());
				Gtk::MessageDialog dialog(
						*parentWindow,
						"There is no data to save",
						false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);

				dialog.run();
				return;
			}

			if (currentFileName == fileName) {
				parentWindow = dynamic_cast<Gtk::Window*>(get_toplevel());
				Gtk::MessageDialog dialog(
						*parentWindow,
						"Cant override original media file",
						false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);

				dialog.run();
				return;
			}

			if (errno == 2) {
				cout << "muxpage2 err = '" << strerror(errno) << "'" << endl;
				errno = 0;
			}
			Muxer::getInstance()->saveTSFile(fileName);
		}
	}

	void MuxTreeView::onAddButtonClick() {
		Gtk::FileChooserDialog dialog(
			    "Please choose a media file", Gtk::FILE_CHOOSER_ACTION_OPEN);

//		dialog.set_transient_for(*this);

		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

		//Add filters, so that only certain file types can be selected:
		Gtk::FileFilter filter_ts;
		filter_ts.set_name("ts files");
		filter_ts.add_pattern("*.ts");
		dialog.add_filter(filter_ts);

		Gtk::FileFilter filter_mpg;
		filter_mpg.set_name("mpeg files");
		filter_mpg.add_mime_type("video/mpeg");
		dialog.add_filter(filter_mpg);

		Gtk::FileFilter filter_any;
		filter_any.set_name("Any files");
		filter_any.add_pattern("*");
		dialog.add_filter(filter_any);

/*		Gtk::FileFilter filter_text;
		filter_text.set_name("Transport Stream files");
		filter_text.add_mime_type("text/plain");
		dialog.add_filter(filter_text);

		Gtk::FileFilter filter_cpp;
		filter_cpp.set_name("C/C++ files");
		filter_cpp.add_mime_type("text/x-c");
		filter_cpp.add_mime_type("text/x-c++");
		filter_cpp.add_mime_type("text/x-c-header");
		dialog.add_filter(filter_cpp);

		Gtk::FileFilter filter_any;
		filter_any.set_name("Any files");
		filter_any.add_pattern("*");
		dialog.add_filter(filter_any);
*/
		//dialog.set_current_folder("/misc");

		//Show the dialog and wait for a user response:
		int result = dialog.run();
		string fileName;
		//Handle the response:
		if (result == Gtk::RESPONSE_OK) {
			cout << "Open clicked." << endl;

			fileName = dialog.get_filename();
			cout << "File selected: " <<  fileName << endl;
			cout << "Opening with muxer... " << endl;
			if (Muxer::getInstance()->openTSFile(fileName)) {
				updateGrid();
			}
		}
	}

	void MuxTreeView::onRemoveButtonClick() {
		Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = 
				get_selection();
		Gtk::TreeIter iter = refTreeSelection->get_selected();
		if (iter) {
			Gtk::Window* parentWindow = dynamic_cast<Gtk::Window*>(
					get_toplevel());

			Gtk::MessageDialog dialog(
					*parentWindow,
					_("Do you really wish to remove this Stream?"), false,
					Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);

			dialog.set_secondary_text(
					"If you continue all child elements will be removed.");

			if (dialog.run() == Gtk::RESPONSE_CANCEL) {
				return;
			}

			refTreeModel->erase(iter);
			updateGrid();
		}
	}

  	bool MuxTreeView::on_button_press_event(GdkEventButton *event) {
		//Call base class, to allow normal handling,
		//such as allowing the row to be selected by the right-click:
		bool returnValue;
		returnValue = Gtk::TreeView::on_button_press_event(event);

		//Then do our custom stuff:
		if ((event->type == GDK_BUTTON_PRESS) && (event->button == 3)) {
			muxPopupMenu.popup(event->button, event->time);
		}

		return returnValue;
	}

	void MuxTreeView::onTreeViewRowActivated(
		    const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {

		Gtk::TreeModel::iterator iter = refTreeModel->get_iter(path);
		if (iter) {
			Gtk::TreeModel::Row row = *iter;
			std::cout << "Row activated: ID=" << row[grid.id] << ", Name=";
			std::cout << row[grid.name] << std::endl;
		}
	}
}
}
}
}
