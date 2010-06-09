#include "../../include/gui/DSMCCOCTreeView.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
   	const Glib::ustring DSMCCOCTreeView::FILE_TYPE = "File";
	const Glib::ustring DSMCCOCTreeView::DIRECTORY_TYPE = "Directory";
	const Glib::ustring
			DSMCCOCTreeView::SERVICE_GATEWAY_TYPE = "Service Gateway";

	const Glib::ustring DSMCCOCTreeView::STREAM_EVENT_TYPE = "Stream Event";

   	DSMCCOCTreeView::DSMCCOCTreeView(DSMCCOCPreviewFrame* previewFrame) {
   		DSMCCOCTreeView();
   		this->previewFrame = previewFrame;
   	}

	DSMCCOCTreeView::DSMCCOCTreeView() {
		previewFrame = NULL;
		refTreeModel = Gtk::TreeStore::create(columnModel);
		set_model(refTreeModel);

		//Add the TreeView's view columns:fileName
		append_column(_("Name"), columnModel.name);
		append_column(_("SD"), columnModel.serviceDomainText);
		append_column(_("IOR"), columnModel.ior); //Interoperable Object Reference

		append_column(_("Type"), columnModel.type);
		append_column(_("Original File"), columnModel.originalFile);
		append_column(_("Size"), columnModel.size);

		Gtk::Menu::MenuList& menulist = popupMenu.items();
		Gtk::Menu* addMenu = manage(new Gtk::Menu());

		menulist.push_back(
				Gtk::Menu_Helpers::MenuElem(_("_Add..."), *addMenu));

		menulist.push_back(
				Gtk::Menu_Helpers::MenuElem("_Remove", sigc::mem_fun(
						*this,
						&DSMCCOCTreeView::removeSelectedNodeFromTree)));

		Gtk::Menu::MenuList& addlist = addMenu->items();
		addlist.push_back(Gtk::Menu_Helpers::MenuElem(
				"_Add directory", sigc::mem_fun(
						*this, &DSMCCOCTreeView::onAddNewDirectoryClick)));

		addlist.push_back(Gtk::Menu_Helpers::MenuElem(
				"_Add Stream Event Object", sigc::mem_fun(
						*this, &DSMCCOCTreeView::onAddStreamEventClick)));

		popupMenu.accelerate(*this);
	}

	DSMCCOCTreeView::~DSMCCOCTreeView() {

	}

	void DSMCCOCTreeView::setModel(model::ElementaryStreamsSet* model) {
		this->model = model;
	}

	model::ElementaryStreamsSet* DSMCCOCTreeView::getModel(){
		return model;
	}

	void DSMCCOCTreeView::refresh() {
		refTreeModel->clear();
		vector<model::ElementaryStream*> streams = model->getStreams();
		for (unsigned int i=0; i < streams.size(); i++) {
			if (streams[i] == NULL ||
					streams[i]->getType() !=
						model::ElementaryStream::DATA_ELEMENTARY_STREAM) {

				continue;
			}
			model::DSMCCCarouselStream* carousel;
			carousel = dynamic_cast<model::DSMCCCarouselStream*>(streams[i]);
			if (carousel == NULL) {
				continue;
			}
		}
	}

	service_domain_type DSMCCOCTreeView::generateServiceDomainID() {
		service_domain_type sdID = 0;
		Gtk::TreeIter iter = refTreeModel->children().begin();
		bool found = true;
		while (found) {
			sdID++;
			found=false;
			while (iter) {
				Gtk::TreeRow row = *iter;
				if (sdID == row[columnModel.serviceDomain]) {
					found = true;
				}
				iter++;
			}
		}
		return sdID;
	}

	void DSMCCOCTreeView::setPreviewFrame(DSMCCOCPreviewFrame* previewFrame) {
		this->previewFrame = previewFrame;
	}

	bool DSMCCOCTreeView::on_button_press_event(GdkEventButton* event) {
		TreeView::on_button_press_event(event);
		if((event->type == GDK_BUTTON_PRESS) && (event->button == 3)){
			popupMenu.popup(event->button,event->time);
			return true;
		}
		return false;
	}

	void DSMCCOCTreeView::onAddStreamEventClick() {
		Glib::RefPtr<Gtk::TreeSelection> refTreeSelection;
		refTreeSelection = get_selection();
		Gtk::TreeIter iter = refTreeSelection->get_selected();
		if (iter) {
			Gtk::TreeRow row = *iter;
			while(row.parent()) { //Finding the root
				iter = row.parent();
				row = *iter;
			}

			StreamEventObjectDialog dialog(row[columnModel.serviceDomain]);
			if (dialog.run() == Gtk::RESPONSE_OK) {
				StreamEventObject_Type steo = dialog.getStreamEventObject();
				addStreamEventObject(
						steo.serviceDomain, steo.eventID, steo.eventName);
			}
		}
	}

	void DSMCCOCTreeView::onAddNewDirectoryClick(){
		Gtk::FileChooserDialog dialog(
			    "Please choose a root directory to add.",
			    Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
		dialog.set_current_folder(Glib::get_home_dir());

		//Show the dialog and wait for a user response:
		int result = dialog.run();
		unsigned int esPid, cId;
		std::string fileName, filePath, tsFile;
		std::map<std::string, ObjectInfo*>* objectsIor;

		if (result == Gtk::RESPONSE_OK) {
			fileName = dialog.get_filename();

			filePath = "";
			esPid = Muxer::getInstance()->getPat()->getNextPid();
			cId = Muxer::getInstance()->getPat()->getNextCarouselId();
			tsFile = OC_ES_PATH + intToStrHexa(esPid) + ".ts";

#ifdef WIN32
			if (fileName.find_last_of("\\") != std::string::npos) {
				filePath = fileName.substr(0, fileName.find_last_of("\\"));
			}
#else
			if (fileName.find_last_of("/") != std::string::npos) {
				filePath = fileName.substr(0, fileName.find_last_of("/"));
			}
#endif

			OCStreamFactory::createOCStream(filePath, tsFile, esPid, cId, 21);
			objectsIor = OCStreamFactory::getObjectsIor();
			addNewDirectory(fileName, objectsIor);
		}
	}

	void DSMCCOCTreeView::removeSelectedNodeFromTree() {
		//Beginning of deletion process
		Glib::RefPtr<Gtk::TreeSelection> refTreeSelection;
		Gtk::TreeIter iter;
		Gtk::Window* parentWindow;

		refTreeSelection = get_selection();
		iter = refTreeSelection->get_selected();
		if (iter) {
			//Ask for confirmation
			//TODO set to parent window
			parentWindow = dynamic_cast<Gtk::Window*>(get_toplevel());
			Gtk::MessageDialog dialog(*parentWindow,
					"Do you really want to remove the Service Domain?",
					false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);

			dialog.set_secondary_text(
					"The entire service domain will be deleted.");

			if (dialog.run() == Gtk::RESPONSE_CANCEL) {
				return;
			}

			Gtk::TreeRow row = *iter;
			while (row.parent()) { //Finding the root
				iter = row.parent();
				row = *iter;
			}

			if (previewFrame!=NULL) {
				previewFrame->removeCarousel(row[columnModel.serviceDomain]);
			}

			refTreeModel->erase(iter);
		}
	}

	void DSMCCOCTreeView::clearCarousels() {
		refTreeModel->clear();
	}

	short DSMCCOCTreeView::addFileAsChild(
			string fileName, Gtk::TreeModel::Row* parentRow) {

		Gtk::TreeModel::Row newRow;
		service_domain_type serviceDomain = 0;
		std::stringstream str;

		//add ior
		ObjectInfo* oi;
		oi = NULL;
		if (currentObjectsIor != NULL) {
			if (currentObjectsIor->count(fileName) != 0) {
				oi = (*currentObjectsIor)[fileName];
			}

		} else {
			cout << "DSMCCOCTreeView::addFileAsChild Warning! ";
			cout << "currentObjectsIor id NULL" << endl;
		}

		if (parentRow) {
			newRow = *(refTreeModel->append(parentRow->children()));
			newRow[columnModel.name] = Glib::filename_to_utf8(
					fileName.substr(
							fileName.find_last_of(G_DIR_SEPARATOR_S)+1));

		//root
		} else {
			newRow = *(refTreeModel->append());
			serviceDomain = oi->sd;
			newRow[columnModel.serviceDomain] = serviceDomain;
			str << std::hex << std::uppercase << serviceDomain;
			newRow[columnModel.serviceDomainText] = Glib::filename_to_utf8(
					"0x"+str.str());

			if (previewFrame != NULL && oi != NULL) {
				previewFrame->addNewCarousel(
						oi->pid, serviceDomain, oi->componentTag);
			}

			newRow[columnModel.name] = G_DIR_SEPARATOR_S;
			newRow[columnModel.type] = SERVICE_GATEWAY_TYPE;
		}

		newRow[columnModel.originalFile] = Glib::filename_to_utf8(fileName);
		if (Glib::file_test(fileName, Glib::FILE_TEST_IS_DIR)) {
			if (parentRow != NULL) {
				newRow[columnModel.type] = DIRECTORY_TYPE;
			}

			std::string fullEntryName, entry;
			Glib::Dir directory(fileName);
			std::list<std::string> entries (directory.begin(), directory.end());
			std::list<std::string>::iterator i;

			i = entries.begin();
			while (i != entries.end()) {
				entry = *i;
				if (entry == "." || entry == "..") {
					continue;
				}

				fullEntryName = fileName + G_DIR_SEPARATOR_S + entry;
				addFileAsChild(fullEntryName, &newRow);
				++i;
			}

		} else {
			newRow[columnModel.type] = FILE_TYPE;
			struct stat fileStat;
			int err = g_stat(fileName.c_str(), &fileStat);
			if (err) {
				cout << "DSMCCOCTreeView::addFileAsChild Warning!!! ";
				cout << "gstat failure = '" << err << "'" << endl;
				//newRow[columnModel.size] = _("Error");

			} else {
				double fileSize = fileStat.st_size/1024.0;
				//double fileSize = fileStat.st_size;
				std::stringstream str;
				str << std::setprecision(1) << std::fixed << fileSize;
				newRow[columnModel.size] = Glib::filename_to_utf8(
						str.str()+"K");
			}
		}

		//add ior
		if (oi != NULL) {
			newRow[columnModel.ior] = oi->ior;
		}
		return serviceDomain;
	}

	short DSMCCOCTreeView::addNewDirectory(
			string filePath, std::map<string, ObjectInfo*>* iors) {

		if (isAlreadyChild(filePath)) {
			cout << "DSMCCOCTreeView::addNewDirectory Warning! ";
			cout << " trying to add an existent child: '" << filePath;
			cout << "'" << endl;
			return -1;
		}

		short ret;
		ObjectInfo* oi;
		std::map<std::string, ObjectInfo*>::iterator i;

		currentObjectsIor = iors;
		removeChildren(filePath);
		ret = addFileAsChild(filePath);
		i = currentObjectsIor->begin();
		while (i != currentObjectsIor->end()) {
			oi = i->second;
			delete oi;
			oi = NULL;
			++i;
		}
		delete currentObjectsIor;
		currentObjectsIor = NULL;
		return ret;
	}

	void DSMCCOCTreeView::removeChildren(string parentFilePath) {
		Gtk::TreeIter iter = refTreeModel->children().begin();
		Glib::ustring uStrfile = Glib::filename_to_utf8(parentFilePath);
		while (iter) {
			Gtk::TreeRow row = *iter;
			Glib::ustring originalFile = row[columnModel.originalFile];
			if(originalFile.length() <= uStrfile.length()){
				iter++;
				continue;
			}

			if (uStrfile == originalFile.substr(0, uStrfile.length())) {
				if (previewFrame!=NULL) {
					previewFrame->removeCarousel(
							row[columnModel.serviceDomain]);
				}

				iter = refTreeModel->erase(iter);
				continue;
			}
			iter++;
		}
	}

	bool DSMCCOCTreeView::isAlreadyChild(string fileName) {
		Gtk::TreeIter iter = refTreeModel->children().begin();
		Glib::ustring uStrfile = Glib::filename_to_utf8(fileName);
		while (iter) {
			Gtk::TreeRow row = *iter;
			Glib::ustring originalFile = row[columnModel.originalFile];
			if(uStrfile.length() < originalFile.length()){
				iter++;
				continue;
			}

			if(originalFile == uStrfile.substr(0, originalFile.length())) {
				return true;
			}
			iter++;
		}
		return false;
	}

	void DSMCCOCTreeView::addStreamEventObject(
			int serviceDomain, int eventId, std::string eventName) {

		std::string strEventId;

		Gtk::TreeIter iter = refTreeModel->children().begin();
		while (iter) {
			Gtk::TreeRow row = *iter;

			if (row[columnModel.serviceDomain] ==
					(unsigned int)serviceDomain) {

				std::cout << "adding stream..."<<serviceDomain;
				std::cout << " " << intToStrHexa(eventId) << " ";
				std::cout << eventName << std::endl;

				Gtk::TreeRow newRow = *(refTreeModel->append(row->children()));
				newRow[columnModel.name] = Glib::filename_to_utf8(eventName);
				newRow[columnModel.type] = STREAM_EVENT_TYPE;
				Gtk::TreeRow eventIdRow = *(refTreeModel->append(
						newRow->children()));

				strEventId = intToStrHexa(eventId);
				eventIdRow[columnModel.name] = Glib::filename_to_utf8(
						_("Event ID: ") + strEventId);
				return;
			}
			iter++;
		}
	}
}
}
}
}
