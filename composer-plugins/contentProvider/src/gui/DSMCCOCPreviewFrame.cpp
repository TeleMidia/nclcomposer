#include "../../include/gui/DSMCCOCPreviewFrame.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	DSMCCOCPreviewFrame::DSMCCOCPreviewFrame() {
	  	this->muxer = Muxer::getInstance();

//infoFrameModel = new DSMCCOCInfoFrame();
	  	refTreeModel = Gtk::ListStore::create(columnModel);
		carouselTree.set_model(refTreeModel);

		refComboBoxModel = Gtk::ListStore::create(comboBoxModel);
		Gtk::TreeModel::Row row = *(refComboBoxModel->append());
		row[comboBoxModel.programNumberChoice] = "0x10";
		row = *(refComboBoxModel->append());
		row[comboBoxModel.programNumberChoice] = "0x15";

		Gtk::TreeView::Column* column = manage(new Gtk::TreeView::Column(_("Program Number")));
		Gtk::CellRendererCombo* cellRenderer = manage(new Gtk::CellRendererCombo());
		column->pack_start(*cellRenderer);
		carouselTree.append_column(*column);

		carouselTree.append_column(_("PID"), columnModel.pid);
		carouselTree.append_column_numeric(_("Service Domain"), columnModel.serviceDomain,"0x%X");
		//carouselTree.append_column_editable(_("PID"), columnModel.pid);
		//carouselTree.append_column_numeric_editable(_("Program Stream"), columnModel.programStream,"0x%X");

#ifdef GLIBMM_PROPERTIES_ENABLED
		column->add_attribute(cellRenderer->property_text(),columnModel.programNumber);
		cellRenderer->property_model() = refComboBoxModel;
		cellRenderer->property_text_column() = 0;
		cellRenderer->property_editable() = true;
#else
		column->add_attribute(*cellRenderer,"text",columnModel.programNumber;
		cellRenderer->set_property("model", refComboBoxModel);
		cellRenderer->set_property("editable", true);
		cellRenderer->set_property("text_column", 0);
#endif
		cellRenderer->signal_edited().connect( sigc::mem_fun(*this, &DSMCCOCPreviewFrame::onEditingProgramNumberCell) );

		scrolledWindow.add(carouselTree);
		scrolledWindow.set_policy(
				Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

		scrolledWindow.set_border_width(5);

		/*Gtk::HPaned* pane = manage(new Gtk::HPaned());
		pane->pack1(scrolledWindow,false,true);
		createInfoFrame();

		Gtk::VBox* vBox = manage(new Gtk::VBox());

		vBox->pack_start(infoFrame, Gtk::PACK_EXPAND_WIDGET,0);

		pane->pack2(*vBox,false,true);
		cout << "PANE WIDTH = '" << pane->get_width() << "'" << endl;
		pane->set_position(300);

		set_label(_("Generated Object Carousel"));
	  	set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
	  	add(*pane);*/
		add(scrolledWindow);
	  	//infoFrameModel->notify();
	}

	DSMCCOCPreviewFrame::~DSMCCOCPreviewFrame() {
		//infoFrameModel->dettach(this);
	}
/*
	void DSMCCOCPreviewFrame::createInfoFrame() {
		Gtk::HBox* mainBox = new Gtk::HBox(true,2);
		Gtk::VBox* VBox = new Gtk::VBox();
		Gtk::Label* auxLabel = new Gtk::Label(_(" Number of TS packages: "));
		auxLabel->set_justify(Gtk::JUSTIFY_LEFT);
		nrTS = new Gtk::Label("0");
		nrTS->set_justify(Gtk::JUSTIFY_RIGHT);
		Gtk::HBox* hBox = new Gtk::HBox();
		hBox->pack_start(*auxLabel, Gtk::PACK_SHRINK);
		hBox->pack_start(*nrTS, Gtk::PACK_SHRINK);
		VBox->pack_start(*hBox, Gtk::PACK_EXPAND_WIDGET);

		auxLabel = new Gtk::Label(_(" Size of TS packages: "));
		auxLabel->set_justify(Gtk::JUSTIFY_LEFT);
		szTS = new Gtk::Label("0K");
		szTS->set_justify(Gtk::JUSTIFY_RIGHT);
		hBox = new Gtk::HBox();
		hBox->pack_start(*auxLabel, Gtk::PACK_SHRINK);
		hBox->pack_start(*szTS, Gtk::PACK_SHRINK);
		VBox->pack_start(*hBox, Gtk::PACK_EXPAND_WIDGET);

		auxLabel = new Gtk::Label(_(" Number of Sessions: "));
		auxLabel->set_justify(Gtk::JUSTIFY_LEFT);
		nrSess = new Gtk::Label("0");
		nrSess->set_justify(Gtk::JUSTIFY_RIGHT);
		hBox = new Gtk::HBox();
		hBox->pack_start(*auxLabel, Gtk::PACK_SHRINK);
		hBox->pack_start(*nrSess, Gtk::PACK_SHRINK);
		VBox->pack_start(*hBox, Gtk::PACK_EXPAND_WIDGET);

		auxLabel = new Gtk::Label(_(" Size of sessions: "));
		auxLabel->set_justify(Gtk::JUSTIFY_LEFT);
		szSess = new Gtk::Label("0K");
		szSess->set_justify(Gtk::JUSTIFY_RIGHT);
		hBox = new Gtk::HBox();
		hBox->pack_start(*auxLabel, Gtk::PACK_SHRINK);
		hBox->pack_start(*szSess, Gtk::PACK_SHRINK);
		VBox->pack_start(*hBox, Gtk::PACK_EXPAND_WIDGET);

		auxLabel = new Gtk::Label(_(" Number of modules: "));
		auxLabel->set_justify(Gtk::JUSTIFY_LEFT);
		nrMod = new Gtk::Label("0");
		nrMod->set_justify(Gtk::JUSTIFY_RIGHT);
		hBox = new Gtk::HBox();
		hBox->pack_start(*auxLabel, Gtk::PACK_SHRINK);
		hBox->pack_start(*nrMod, Gtk::PACK_SHRINK);
		VBox->pack_start(*hBox, Gtk::PACK_EXPAND_WIDGET);

		auxLabel = new Gtk::Label(_(" Size of modules: "));
		auxLabel->set_justify(Gtk::JUSTIFY_LEFT);
		szMod = new Gtk::Label("0K");
		szMod->set_justify(Gtk::JUSTIFY_RIGHT);
		hBox = new Gtk::HBox();
		hBox->pack_start(*auxLabel, Gtk::PACK_SHRINK);
		hBox->pack_start(*szMod, Gtk::PACK_SHRINK);
		VBox->pack_start(*hBox, Gtk::PACK_EXPAND_WIDGET);

		mainBox->pack_start(*VBox);

		infoFrame.set_label(_("Object Carousel Info"));
		infoFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
		infoFrame.add(*mainBox);
	}

	void DSMCCOCPreviewFrame::updateInfo() {
		DSMCCOCInfoFrameModel* model = getModel();
		std::stringstream str1;
		str1 << model->getNumberOfTS();
		nrTS->set_text(Glib::filename_to_utf8(str1.str()));
		std::stringstream str2;
		str2 << model->getNumberOfSessions();
		nrSess->set_text(Glib::filename_to_utf8(str2.str()));
		std::stringstream str3;
		str3 << model->getNumberOfModules();
		nrMod->set_text(Glib::filename_to_utf8(str3.str()));
		std::stringstream str4;
		str4 << std::setprecision(1) << std::fixed << ((double)model->getTSSize())/1024.0;
		szTS->set_text(Glib::filename_to_utf8(str4.str()+"K"));
		std::stringstream str5;
		str5 << std::setprecision(1) << std::fixed << ((double)model->getSessionsSize())/1024.0;
		szSess->set_text(Glib::filename_to_utf8(str5.str()+"K"));
		std::stringstream str6;
		str6 << std::setprecision(1) << std::fixed << ((double)model->getModulesSize())/1024.0;
		szMod->set_text(Glib::filename_to_utf8(str6.str()+"K"));
	}

	DSMCCOCInfoFrameModel* DSMCCOCPreviewFrame::getModel() {
		return infoFrameModel;
	}*/

	std::vector<CarouselInfo*>* DSMCCOCPreviewFrame::getCarouselsInfo() {
		Gtk::TreeIter iter;
		Gtk::TreeRow row;
		service_domain_type sd;
		Glib::ustring pid, programNumber, componentTag;
		Gtk::Window* parentWindow;
		std::string msg;
		std::vector<CarouselInfo*>* infos;
		CarouselInfo* info;

		infos = new std::vector<CarouselInfo*>;
		iter = refTreeModel->children().begin();
		while (iter) {
			row = *iter;
			pid = row[columnModel.pid];
			sd = row[columnModel.serviceDomain];
			programNumber = row[columnModel.programNumber];
			componentTag = row[columnModel.componentTag];
			if (!Muxer::getInstance()->getPat()->hasProgramNumber(
					strHexaToInt(programNumber))) {

				msg = "Invalid program number for service domain '" +
						intToStrHexa(sd) + "'";

				parentWindow = dynamic_cast<Gtk::Window*>(get_toplevel());
				Gtk::MessageDialog dialog(*parentWindow,
						msg, false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);

				dialog.run();
				std::vector<CarouselInfo*>::iterator i;
				i = infos->begin();
				while (i != infos->end()) {
					if (*i != NULL) {
						delete *i;
					}
					++i;
				}
				delete infos;
				return NULL;
			}
			info = new CarouselInfo;
			info->pid = strHexaToInt(pid);
			info->myId = sd;
			info->programNumber = strHexaToInt(programNumber);
			info->componentTag = strHexaToInt(componentTag);
			infos->push_back(info);
			iter++;
		}

		return infos;
	}

	void DSMCCOCPreviewFrame::clearCarousels() {
		refTreeModel->clear();
	}

	void DSMCCOCPreviewFrame::addNewCarousel(
			unsigned int pid,
			service_domain_type serviceDomain,
			unsigned int componentTag) {

		Gtk::TreeIter iter;
		Gtk::TreeRow row;
		service_domain_type rowServiceDomain;

		iter = refTreeModel->children().begin();
		row = *iter;
		while (iter) { // CHECK IF the name already exists
			rowServiceDomain = row[columnModel.serviceDomain];
			if (serviceDomain == rowServiceDomain) {
				return;
			}
			iter++;
		}

		Gtk::TreeRow newRow = *(refTreeModel->append());
		newRow[columnModel.pid] = intToStrHexa(pid);
		newRow[columnModel.serviceDomain] = serviceDomain;
		newRow[columnModel.componentTag] = intToStrHexa(componentTag);
		newRow[columnModel.programNumber] = intToStrHexa(
				Muxer::getInstance()->getPat()->getFirstProgramNumber());
	}

	void DSMCCOCPreviewFrame::removeCarousel(
			service_domain_type serviceDomain) {

		Gtk::TreeIter iter = refTreeModel->children().begin();
		while(iter){
			Gtk::TreeRow row = *iter;
			service_domain_type rowServiceDomain = row[columnModel.serviceDomain];
			if(serviceDomain == rowServiceDomain){
				iter = refTreeModel->erase(iter);
				continue;
			}
			iter++;
		}
	}

	void DSMCCOCPreviewFrame::onEditingProgramNumberCell(
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

	void DSMCCOCPreviewFrame::updateProgramNumbers() {
		std::map<unsigned int, Pmt*>::iterator iter;
		std::map<unsigned int, Pmt*>* programs;
		programs = muxer->getProgramsInfo();
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
			row[comboBoxModel.programNumberChoice] = Glib::filename_to_utf8("0x" + str.str());

			++iter;
		}
	}
}
}
}
}
