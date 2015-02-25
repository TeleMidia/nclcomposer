#include "../../include/gui/DSMCCPage.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	DSMCCPage::DSMCCPage(MuxTreeView* muxTreeView) : addSEButton(_("Add SE")),
			sendSEButton(_("Send SEs")),
			xmlButton(_("Set XML File")),
			multiplexButton(_("Multiplex")) {

		Gtk::VPaned* pane;
		Gtk::VPaned* pane2;

		Gtk::VBox* vBox;
		Gtk::VBox* vBox2;
		Gtk::VBox* vBox3;

		Gtk::Table* table;

		Gtk::Alignment* align;
		Gtk::Alignment* align2;
		Gtk::Alignment* align3;

		this->muxTreeView = muxTreeView;

		currentCommandTag = "";
		currentXmlFileName = "";

		dsmccStreamEventView = new DSMCCSETreeView();
		dsmccObjectCarouselFrame = new DSMCCOCPreviewFrame();
		dsmccObjectCarouselView = new DSMCCOCTreeView();

		dsmccObjectCarouselView->setPreviewFrame(dsmccObjectCarouselFrame);

		dsmccStreamEventFrame = new DSMCCStreamEventFrame(dsmccStreamEventView);

		/////LEFT PANEL
		leftFrame.set_label("DSM-CC Stream Events");
		leftFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

		pane2 = manage(new Gtk::VPaned());
		leftFrame.add(*pane2);

		leftScrolledWindow.add(*dsmccStreamEventView);
		leftScrolledWindow.set_policy(
			    Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

		leftScrolledWindow.set_border_width(5);

		vBox = manage(new Gtk::VBox(false, 0));
		table = manage(new Gtk::Table(2,4));
  		table->set_row_spacings(4);
		table->set_col_spacings(2);
		table->set_border_width(2);

		isEditing = manage(new Gtk::CheckButton("NCL Editing Command"));
		isEditing->set_flags(Gtk::CAN_FOCUS);
		isEditing->set_relief(Gtk::RELIEF_NORMAL);
		isEditing->set_mode(true);
		isEditing->set_active(false);
		isEditing->set_alignment(0.5, 0.5);
		isEditing->show();
		isEditing->signal_clicked().connect(
				sigc::mem_fun(*this, &DSMCCPage::on_isEditing_clicked));

		baseIdLabel = manage(new Gtk::Label("Base Id: "));
		baseIdLabel->set_justify(Gtk::JUSTIFY_LEFT);
		baseIdLabel->set_no_show_all(true);
		baseIdEntry.set_text("0x6");
		baseIdEntry.set_no_show_all(true);

		documentIdLabel = manage(new Gtk::Label("Document Id: "));
		documentIdLabel->set_justify(Gtk::JUSTIFY_LEFT);
		documentIdLabel->set_no_show_all(true);
		documentIdEntry.set_no_show_all(true);

		compositeIdLabel = manage(new Gtk::Label("Composite Id: "));
		compositeIdLabel->set_justify(Gtk::JUSTIFY_LEFT);
		compositeIdLabel->set_no_show_all(true);
		compositeIdEntry.set_no_show_all(true);

		label = manage(new Gtk::Label("NCL Command: "));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		label->set_no_show_all(true);

		align = manage(new Gtk::Alignment(
				Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, 0.3, 0.0));

		align->add(xmlButton);
		xmlButton.set_no_show_all(true);
		xmlButton.signal_clicked().connect(sigc::mem_fun(*this,
					&DSMCCPage::on_xmlButton_clicked));

		initializeNCLCommands();
		commandsCombo.set_no_show_all(true);
		commandsCombo.signal_changed().connect(
				sigc::mem_fun(*this, &DSMCCPage::on_combo_changed));

		vBox->pack_start(*isEditing, Gtk::PACK_SHRINK, 0);
		vBox->pack_start(*table, Gtk::PACK_SHRINK, 0);

		table->attach(*label,0,1,0,1);
		table->attach(commandsCombo,1,2,0,1);

		table->attach(*baseIdLabel,0,1,1,2);
		table->attach(baseIdEntry,1,2,1,2);

		table->attach(*documentIdLabel,0,1,2,3);
		table->attach(documentIdEntry,1,2,2,3);

		table->attach(*compositeIdLabel,0,1,3,4);
		table->attach(compositeIdEntry,1,2,3,4);

		vBox->pack_start(*align, Gtk::PACK_SHRINK,2);
		vBox->pack_start(*dsmccStreamEventFrame);

		addSEButton.signal_clicked().connect(sigc::mem_fun(*this,
					&DSMCCPage::on_addSEButton_clicked));

		align2 = manage(new Gtk::Alignment(
				Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, 0.3, 0.0));

		align2->add(addSEButton);

		align3 = manage(new Gtk::Alignment(
				Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER, 0.3, 0.0));

		/*align3->add(sendSEButton);
		sendSEButton.signal_clicked().connect(sigc::mem_fun(*this,
					&DSMCCPage::on_sendSEButton_clicked));*/

		vBox3 = manage(new Gtk::VBox(false, 0));
		vBox3->pack_start(*align2, Gtk::PACK_SHRINK, 0);
		vBox3->pack_start(leftScrolledWindow, Gtk::PACK_EXPAND_WIDGET, 0);
		vBox3->pack_start(*align3, Gtk::PACK_SHRINK, 0);

		pane2->add1(*vBox);
		pane2->add2(*vBox3);
		pane2->set_position(300);

		/////RIGHT PANEL
		rightScrolledWindow.add(*dsmccObjectCarouselView);
		rightScrolledWindow.set_policy(
			    Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

		rightScrolledWindow.set_border_width(5);

		//adding to pane
		pane = manage(new Gtk::VPaned());
		pane->add1(rightScrolledWindow);
		pane->add2(*dsmccObjectCarouselFrame);
		pane->set_position(450);

		// Two Buttons
		Gtk::HBox* hBox = manage(new Gtk::HBox(false, 0));
		vBox2 = manage(new Gtk::VBox(false, 0));

		multiplexButton.set_size_request(-1,30);
		multiplexButton.signal_clicked().connect(sigc::mem_fun(*this,
					&DSMCCPage::on_multiplexButton_clicked));

		hBox->pack_start(multiplexButton, Gtk::PACK_SHRINK, 0);
		vBox2->pack_start(*hBox, Gtk::PACK_SHRINK, 0);
		vBox2->pack_start(*pane, Gtk::PACK_EXPAND_WIDGET, 0);

		/* Set the frames label */
		rightFrame.set_label("DSM-CC Object Carousel");

		/* Set the style of the frame */
		rightFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
		rightFrame.add(*vBox2);

		//// ADDING LEFT AND RIGHT PANEL
		dsmccPanel.set_position(370);
		dsmccPanel.add1(leftFrame);
		dsmccPanel.add2(rightFrame);

		muxBox.pack_start(dsmccPanel);
		currentEventId = 0;

		ocDirs = new vector<string>;
	}

	DSMCCPage::~DSMCCPage() {

	}

	void DSMCCPage::initializeNCLCommands() {
		map<string, int>::iterator i;
		commandMap = new map<string, int>;

		(*commandMap)["openBase"] = 0x00; //baseId, location
		(*commandMap)["activateBase"] = 0x01; //baseId
		(*commandMap)["deactivateBase"] = 0x02; //baseId
		(*commandMap)["saveBase"] = 0x03; //baseId, location
		(*commandMap)["closeBase"] = 0x04; //baseId
		(*commandMap)["addDocument"] = 0x05; //baseId, {uri,ior}+
		(*commandMap)["removeDocument"] = 0x06; //baseId, documentId
		(*commandMap)["startDocument"] = 0x07;//baseId, documentId, interfaceId
		(*commandMap)["stopDocument"] = 0x08; //baseId, documentId
		(*commandMap)["pauseDocument"] = 0x09; //baseId, documentId
		(*commandMap)["resumeDocument"] = 0x0A; //baseId, documentId

		(*commandMap)["addRegion"] = 0x0B;	//baseId, documentId, regionBaseId,
											//regionId, xmlRegion

		(*commandMap)["removeRegion"] = 0x0C; //baseId, documentId, regionId,
		(*commandMap)["addRegionBase"] = 0x0D;	//baseId, documentId,
												//xmlRegionBase

		(*commandMap)["removeRegionBase"] = 0x0E;	//baseId, documentId,
													//regionBaseId

		(*commandMap)["addRule"] = 0x0F; //baseId, documentId, xmlRule
		(*commandMap)["removeRule"] = 0x10; //baseId, documentId, ruleId
		(*commandMap)["addRuleBase"] = 0x11;	//baseId, documentId,
												//xmlRuleBase

		(*commandMap)["removeRuleBase"] = 0x12;//baseId, documentId, ruleBaseId
		(*commandMap)["addConnector"] = 0x13;
		(*commandMap)["removeConnector"] = 0x14;
		(*commandMap)["addConnectorBase"] = 0x15;
		(*commandMap)["removeConnectorBase"] = 0x16;
		(*commandMap)["addDescriptor"] = 0x17;
		(*commandMap)["removeDescriptor"] = 0x18;
		(*commandMap)["addDescriptorSwicth"] = 0x19;
		(*commandMap)["removeDescriptorSwicth"] = 0x1A;
		(*commandMap)["addDescriptorBase"] = 0x1B;
		(*commandMap)["removeDescriptorBase"] = 0x1C;
		(*commandMap)["addTransition"] = 0x1D;
		(*commandMap)["removeTransition"] = 0x1E;
		(*commandMap)["addTransitionBase"] = 0x1F;
		(*commandMap)["removeTransitionBase"] = 0x20;
		(*commandMap)["addImportBase"] = 0x21;
		(*commandMap)["removeImportBase"] = 0x22;
		(*commandMap)["addImportedDocumentBase"] = 0x23;
		(*commandMap)["removeImportedDocumentBase"] = 0x24;
		(*commandMap)["addImportNCL"] = 0x25;
		(*commandMap)["removeImportNCL"] = 0x26;
		(*commandMap)["addNode"] = 0x27;	//baseId, documentId, compositeId,
											// {uri,ior}+

		(*commandMap)["removeNode"] = 0x28;	//baseId, documentId, compositeId,
											//nodeId

		(*commandMap)["addInterface"] = 0x29;
		(*commandMap)["removeInterface"] = 0x2A;
		(*commandMap)["addLink"] = 0x2B;
		(*commandMap)["removeLink"] = 0x2C;
		(*commandMap)["setPropertyValue"] = 0x2D;

		i = commandMap->begin();
		while (i != commandMap->end()) {
			commandsCombo.append_text(i->first);
			++i;
		}
	}

	Gtk::VBox* DSMCCPage::getPageBox() {
		return &muxBox;
	}

	void DSMCCPage::updateView(){
		dsmccObjectCarouselFrame->updateProgramNumbers();
	}

	string DSMCCPage::getFilePath(string uri) {
#ifdef WIN32
		if (uri.find_last_of("\\") != std::string::npos) {
			return uri.substr(0, uri.find_last_of("\\"));
		}
#else
		if (uri.find_last_of("/") != std::string::npos) {
			return uri.substr(0, uri.find_last_of("/"));
		}
#endif
		return "";
	}

	void DSMCCPage::add() {
		/*Gtk::FileChooserDialog dialog(
			    "Please choose a root directory to add.", Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

		dialog.set_current_folder("/misc");

		//Show the dialog and wait for a user response:
		int result = dialog.run();

		string fileName;

		if(result == Gtk::RESPONSE_OK){
			fileName = dialog.get_filename();
			Gtk::MessageDialog mDialog("Arquivo Selecionado!",false,
					Gtk::MESSAGE_QUESTION,
					Gtk::BUTTONS_OK);
			mDialog.set_secondary_text(Glib::filename_to_utf8(fileName));
			mDialog.run();

			dsmcccoView.addNewDirectory(&fileName);
		}*/
		dsmccObjectCarouselView->onAddNewDirectoryClick();
	}

	void DSMCCPage::hideAllEditingWidgets() {
		baseIdLabel->set_no_show_all(true);
		baseIdLabel->hide();
		baseIdEntry.set_no_show_all(true);
		baseIdEntry.hide();

		documentIdLabel->set_no_show_all(true);
		documentIdLabel->hide();
		documentIdEntry.set_no_show_all(true);
		documentIdEntry.hide();

		compositeIdLabel->set_no_show_all(true);
		compositeIdLabel->hide();
		compositeIdEntry.set_no_show_all(true);
		compositeIdEntry.hide();

		xmlButton.set_no_show_all(true);
		xmlButton.hide();
	}

	void DSMCCPage::on_isEditing_clicked() {
		if (isEditing->get_active()) {
			label->set_no_show_all(false);
			label->show_now();

			commandsCombo.set_no_show_all(false);
			commandsCombo.show_now();

			dsmccStreamEventFrame->setIdAndPrivateDataAsEditable(false);

			//sendSEButton.set_no_show_all(true);
			//sendSEButton.hide();

		} else {
			label->set_no_show_all(true);
			label->hide();

			commandsCombo.hide_all();
			commandsCombo.set_no_show_all(true);

			hideAllEditingWidgets();

			//sendSEButton.set_no_show_all(false);
			//sendSEButton.show_now();

			dsmccStreamEventFrame->setIdAndPrivateDataAsEditable(true);
		}
	}

	void DSMCCPage::on_combo_changed() {
		string text, hexa;

		hideAllEditingWidgets();

		text = commandsCombo.get_active_text();
		if (commandMap->count(text) == 0) {
			return;
		}

		currentCommandTag = intToStrHexa((*commandMap)[text]);

		baseIdLabel->set_no_show_all(false);
		baseIdLabel->show_now();
		baseIdEntry.set_no_show_all(false);
		baseIdEntry.show_now();

		if (text.substr(0, 3) == "add") {
			if (text == "addNode") {
				documentIdLabel->set_no_show_all(false);
				documentIdLabel->show_now();
				documentIdEntry.set_no_show_all(false);
				documentIdEntry.show_now();

				compositeIdLabel->set_no_show_all(false);
				compositeIdLabel->show_now();
				compositeIdEntry.set_no_show_all(false);
				compositeIdEntry.show_now();
			}

			xmlButton.set_no_show_all(false);
			xmlButton.show_now();
		}

		cout << "combo sel '" << commandsCombo.get_active_text() << "'" << endl;
	}

	void DSMCCPage::on_xmlButton_clicked() {
		Gtk::FileChooserDialog dialog(
			    "Please select xml file to add command.",
			    Gtk::FILE_CHOOSER_ACTION_OPEN);

		int result;

		currentXmlFileName = "";

		//Add response buttons the the dialog:
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
		dialog.set_current_folder(Glib::get_home_dir());

		//Add filters, so that only certain file types can be selected:
		Gtk::FileFilter filter_ncl;
		filter_ncl.set_name("ncl files");
		filter_ncl.add_pattern("*.ncl");
		dialog.add_filter(filter_ncl);

		Gtk::FileFilter filter_xml;
		filter_xml.set_name("xml files");
		filter_xml.add_pattern("*.xml");
		dialog.add_filter(filter_xml);

		//Show the dialog and wait for a user response:
		result = dialog.run();

		if (result == Gtk::RESPONSE_OK) {
			currentXmlFileName = dialog.get_filename();
		}
	}

	void DSMCCPage::on_addSEButton_clicked() {
		unsigned int esPid, cId, cTag;
		short sd;
		ObjectInfo* oi;
		map<string, ObjectInfo*>* objectsIor;
		map<string, ObjectInfo*>::iterator i;
		string filePath, fileIor, privateData, combo, tsFile, line, srcPath;
		ifstream fis;

		currentEventId++;
		dsmccStreamEventFrame->setEventId(currentEventId);
		combo = commandsCombo.get_active_text();
		privateData = dsmccStreamEventFrame->getCurrentPrivateData();
		filePath = getFilePath(currentXmlFileName);

		cout << "DSMCCPage::on_addSEButton_clicked ";
		cout << "currentEventId = '" << currentEventId << "' ";
		cout << "currentXmlFileName = '" << currentXmlFileName << "' ";
		cout << "combo = '" << combo << "' ";
		cout << "privateData = '" << privateData << "' ";
		cout << "filePath = '" << filePath << "' ";
		cout << endl;

		if (combo == "addDocument" || combo == "addNode") {
			esPid = Muxer::getInstance()->getPat()->getNextPid();
			cId = Muxer::getInstance()->getPat()->getNextCarouselId();
			cTag = Muxer::getInstance()->getPat()->getCarouselComponentTag();

			tsFile = OC_ES_PATH + intToStrHexa(esPid) + ".ts";

			cout << "DSMCCPage::on_addSEButton_clicked call createOCStream";
			cout << endl;
			OCStreamFactory::createOCStream(
					filePath, tsFile, esPid, cId, cTag);

			cout << "DSMCCPage::on_addSEButton_clicked call createOCStream ok";
			cout << endl;
			ocDirs->push_back(filePath);
			objectsIor = OCStreamFactory::getObjectsIor();
			if (objectsIor->count(currentXmlFileName) != 0) {
				oi = (*objectsIor)[currentXmlFileName];
				fileIor = oi->ior;

			} else {
				cout << "DsmccPage addSe Warning! Cant find '";
				cout << currentXmlFileName << "' object with path '";
				cout << filePath << "'" << endl;
			}

			cout << "DSMCCPage::on_addSEButton_clicked call addNewDir '";
			cout << filePath << "'" << endl;
			sd = dsmccObjectCarouselView->addNewDirectory(filePath, objectsIor);
			cout << "DSMCCPage::on_addSEButton_clicked call calladdSEObjtect";
			cout << endl;
			dsmccObjectCarouselView->addStreamEventObject(
					sd, currentEventId, "gingaEditingCommand");

			privateData = currentCommandTag + privateData +
					", x-sbtvd://" + filePath + ", " + fileIor;

			cout << "DSMCCPage::on_addSEButton_clicked call openNCL";
			cout << endl;
			fis.open(currentXmlFileName.c_str(), ifstream::in);
			if (fis.is_open()) {
				while (fis.good()) {
					fis >> line;
					if (line.find("src=\"") != std::string::npos) {
						line = line.substr(
								line.find("src=") + 5, line.length());

						line = line.substr(0, line.find_first_of("\""));
						srcPath = getFilePath(line);
						if (!hasParent(srcPath)) {
							esPid = Muxer::getInstance()->getPat()->
									getNextPid();

							cId = Muxer::getInstance()->getPat()->
									getNextCarouselId();

							cTag = Muxer::getInstance()->getPat()->
									getCarouselComponentTag();

							tsFile = OC_ES_PATH + intToStrHexa(esPid) + ".ts";

							OCStreamFactory::createOCStream(
									srcPath, tsFile, esPid, cId, cTag);

							ocDirs->push_back(srcPath);
							objectsIor = OCStreamFactory::getObjectsIor();
							if (objectsIor->count(line) != 0) {
								oi = (*objectsIor)[line];
								fileIor = oi->ior;
							}

							sd = dsmccObjectCarouselView->addNewDirectory(
									srcPath, objectsIor);

							privateData = privateData + ", x-sbtvd://" +
									srcPath + ", " + fileIor;
						}
					}
				}
			}

			cout << "DSMCCPage::on_addSEButton_clicked call fis.close" << endl;
			fis.close();
			cout << "DSMCCPage::on_addSEButton_clicked call setPriDat" << endl;
			dsmccStreamEventFrame->setPrivateData(privateData);
		}

		cout << "DSMCCPage::on_addSEButton_clicked call createSE" << endl;
		dsmccStreamEventFrame->createStreamEvent();
		cout << "DSMCCPage::on_addSEButton_clicked clearing values" << endl;

		//reseting current SE values
		currentCommandTag = "";
		currentXmlFileName = "";
		dsmccStreamEventFrame->clearSEFields();
		isEditing->set_active(false);
		cout << "DSMCCPage::on_addSEButton_clicked all done!" << endl;
	}

	bool DSMCCPage::hasParent(string child) {
		vector<string>::iterator i;

		i = ocDirs->begin();
		while (i != ocDirs->end()) {
			if (isChild(*i, child)) {
				return true;
			}
			++i;
		}
		return false;
	}

	bool DSMCCPage::isChild(string parent, string child) {
#ifdef WIN32
		if (child.find("\\") == std::string::npos) {
			return true;
		}

		if (child.substr(2, 1) != "\\") {
			return true;
		}

#else
		if (child.find("/") == std::string::npos) {
			return true;
		}

		if (child.substr(0, 1) != "/") {
			return true;
		}
#endif

		if (child.length() >= parent.length()) {
			if (child.substr(0, parent.length()) == parent) {
				return true;
			}
		}
		return false;
	}

	void DSMCCPage::on_sendSEButton_clicked() {

	}

	void DSMCCPage::on_multiplexButton_clicked() {
		vector<CarouselInfo*>* ocInfos;
		vector<SEInfo*>* seInfos;
		ocInfos = dsmccObjectCarouselFrame->getCarouselsInfo();
		seInfos = dsmccStreamEventView->getStreamEventsInfo();
		if (ocInfos != NULL) {
			Muxer::getInstance()->muxCarousels(ocInfos);
			Muxer::getInstance()->muxStreamEvents(seInfos);
			muxTreeView->updateGrid();
			dsmccObjectCarouselView->clearCarousels();
			dsmccObjectCarouselFrame->clearCarousels();
			dsmccStreamEventView->clearStreamEvents();
		}
	}
}
}
}
}
