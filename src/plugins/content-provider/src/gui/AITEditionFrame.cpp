#include "../../include/gui/AITEditionFrame.h"


namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
   	
   	//PUBLIC CONSTANTS
   	const Glib::ustring AITEditionFrame::AUTOSTART_CONTROL_CODE = _("autostart");
	const Glib::ustring AITEditionFrame::PRESENT_CONTROL_CODE = _("present");
	const Glib::ustring AITEditionFrame::DESTROY_CONTROL_CODE = _("destroy");
	const Glib::ustring AITEditionFrame::KILL_CONTROL_CODE = _("kill");
	const Glib::ustring AITEditionFrame::PREFETCH_CONTROL_CODE = _("prefetch");
	const Glib::ustring AITEditionFrame::REMOTE_CONTROL_CODE = _("remote");

	const Glib::ustring AITEditionFrame::DVB_J_APPLICATION_TYPE = _("dvb-j");
	const Glib::ustring AITEditionFrame::DVB_HTML_APPLICATION_TYPE = _("dvb-html");
	
	const Glib::ustring AITEditionFrame::OC_APPLICATION_PROTOCOL = _("OC");
	const Glib::ustring AITEditionFrame::IP_APPLICATION_PROTOCOL = _("IP");
   	
   	
   	const Glib::ustring AITEditionFrame::UNKNOWN = _("unknown");
   	
	AITEditionFrame::AITEditionFrame(AITTreeView* aitTree) 
	:	addButton(_("Add")) ,
		clearButton(_("Clear")) {
			
		this->aitTree = aitTree;
		
	/////// AIT INFORMATION FRAME
		Gtk::Frame* aitInformationFrame = manage(new Gtk::Frame());
		aitInformationFrame->set_label(_("AIT Information"));
  		aitInformationFrame->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
  		
  		Gtk::Table* table = manage(new Gtk::Table(2,3));
  		table->set_row_spacings(4);
		table->set_col_spacings(2);
		table->set_border_width(2);
  		aitInformationFrame->add(*table);
  		
  		Gtk::Label* label = manage(new Gtk::Label(_("PID: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,0,1);
		table->attach(pidEntry,1,2,0,1);
		
		label = manage(new Gtk::Label(_("Version: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,1,2);
		table->attach(aitVersionEntry,1,2,1,2);
		
		label = manage(new Gtk::Label(_("Bit Stuffing: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,2,3);
		table->attach(bitStufingCheckBt,1,2,2,3);
		
		
	//////APPLICATION FRAME
		Gtk::Frame* newApplicationFrame = manage(new Gtk::Frame());
		newApplicationFrame->set_label(_("New Application"));
  		newApplicationFrame->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
		
		Gtk::VBox* vBox = manage(new Gtk::VBox(false,1));
		newApplicationFrame->add(*vBox);
		
		table = manage(new Gtk::Table(2,13));
		table->set_row_spacings(4);
		table->set_col_spacings(2);
		table->set_border_width(2);
		vBox->pack_start(*table,Gtk::PACK_SHRINK,2);
		
		Gtk::HBox* hBox = manage(new Gtk::HBox(false,10));
		hBox->set_border_width(10);
		vBox->pack_start(*hBox,Gtk::PACK_SHRINK,2);
		
		hBox->pack_start(addButton);
		hBox->pack_start(clearButton);
		addButton.signal_clicked().connect( sigc::mem_fun(*this,
					&AITEditionFrame::onAddButtonClicked) );
		clearButton.signal_clicked().connect( sigc::mem_fun(*this,
					&AITEditionFrame::onClearButtonClicked) );
		
		label = manage(new Gtk::Label(_("Name: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,0,1);
		table->attach(appNameEntry,1,2,0,1);
		
		label = manage(new Gtk::Label(_("Organization ID: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,1,2);
		table->attach(appOrgIDEntry,1,2,1,2);
		
		label = manage(new Gtk::Label(_("Application ID: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,2,3);
		table->attach(appIDEntry,1,2,2,3);
		
		label = manage(new Gtk::Label(_("Control-Code: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,3,4);
		table->attach(controlCodeCombo,1,2,3,4);
		
		label = manage(new Gtk::Label(_("Type: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,4,5);
		table->attach(appTypeCombo,1,2,4,5);
		
		label = manage(new Gtk::Label(_("Version: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,5,6);
		//table->attach(appVersionEntry,1,2,5,6);
		
		Gtk::HBox* hbox = manage(new Gtk::HBox(false,0));
		table->attach(*hbox,1,2,5,6);
		appMajorVersionEntry.set_width_chars(4);
		hbox->pack_start(appMajorVersionEntry);
		label = manage(new Gtk::Label("."));
		hbox->pack_start(*label);
		appMinorVersionEntry.set_width_chars(4);
		hbox->pack_start(appMinorVersionEntry);
		label = manage(new Gtk::Label("."));
		hbox->pack_start(*label);
		appMicroVersionEntry.set_width_chars(4);
		hbox->pack_start(appMicroVersionEntry);
		
		label = manage(new Gtk::Label(_("Protocol: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,6,7);
		table->attach(appProtocolCombo,1,2,6,7);
		
		label = manage(new Gtk::Label(_("Remote flag: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,7,8);
		table->attach(appRemoteCheckBt,1,2,7,8);
		
		label = manage(new Gtk::Label(_("Locator: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,8,9);
		table->attach(appLocatorEntry,1,2,8,9);
		
		label = manage(new Gtk::Label(_("Base Directory: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,9,10);
		table->attach(appBaseDirEntry,1,2,9,10);
		
		label = manage(new Gtk::Label(_("Ext. Classpath: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,10,11);
		table->attach(appExtClasspathEntry,1,2,10,11);
		
		label = manage(new Gtk::Label(_("Parameters: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,11,12);
		table->attach(appParamsEntry,1,2,11,12);
		
		label = manage(new Gtk::Label(_("Initial Class: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,12,13);
		table->attach(appInitClassEntry,1,2,12,13);
		
	////// DEPLOYING FRAMES
		Gtk::VBox* vBox2 = manage(new Gtk::VBox(false,1));
  		vBox2->pack_start(*aitInformationFrame,Gtk::PACK_SHRINK,2);
		vBox2->pack_start(*newApplicationFrame,Gtk::PACK_SHRINK,2);
		add(*vBox2);
		
  		set_shadow_type(Gtk::SHADOW_NONE);
  		
		populateCombos();
	}
	
	AITEditionFrame::~AITEditionFrame() {
		delete aitTree;
	}
	
	void AITEditionFrame::populateCombos(){
		// CONTROL-CODE COMBO
		controlCodeComboModel = Gtk::ListStore::create(controlCodeMC);
		controlCodeCombo.set_model(controlCodeComboModel);
		controlCodeCombo.pack_start(controlCodeMC.text);
		Gtk::TreeRow row = *(controlCodeComboModel->append());
		row[controlCodeMC.controlCode] = model::AITApplication::AUTOSTART_CONTROL_CODE;
		row[controlCodeMC.text] = *getControlCodeAsString(model::AITApplication::AUTOSTART_CONTROL_CODE);
		
		row = *(controlCodeComboModel->append());
		row[controlCodeMC.controlCode] = model::AITApplication::PRESENT_CONTROL_CODE;
		row[controlCodeMC.text] = *getControlCodeAsString(model::AITApplication::PRESENT_CONTROL_CODE);
		
		row = *(controlCodeComboModel->append());
		row[controlCodeMC.controlCode] = model::AITApplication::DESTROY_CONTROL_CODE;
		row[controlCodeMC.text] = *getControlCodeAsString(model::AITApplication::DESTROY_CONTROL_CODE);
		
		row = *(controlCodeComboModel->append());
		row[controlCodeMC.controlCode] = model::AITApplication::KILL_CONTROL_CODE;
		row[controlCodeMC.text] = *getControlCodeAsString(model::AITApplication::KILL_CONTROL_CODE);
		
		row = *(controlCodeComboModel->append());
		row[controlCodeMC.controlCode] = model::AITApplication::PREFETCH_CONTROL_CODE;
		row[controlCodeMC.text] = *getControlCodeAsString(model::AITApplication::PREFETCH_CONTROL_CODE);
		
		row = *(controlCodeComboModel->append());
		row[controlCodeMC.controlCode] = model::AITApplication::REMOTE_CONTROL_CODE;
		row[controlCodeMC.text] = *getControlCodeAsString(model::AITApplication::REMOTE_CONTROL_CODE);
		
		//APPLICATIONTYPE COMBO
		appTypeComboModel = Gtk::ListStore::create(applicationTypeMC);
		appTypeCombo.set_model(appTypeComboModel);
		appTypeCombo.pack_start(applicationTypeMC.text);
		row = *(appTypeComboModel->append());
		row[applicationTypeMC.applicationType] = model::AITStream::DVB_J_APPLICATION;
		row[applicationTypeMC.text] = *getApplicationTypeAsString(model::AITStream::DVB_J_APPLICATION);
		
		row = *(appTypeComboModel->append());
		row[applicationTypeMC.applicationType] = model::AITStream::DVB_HTML_APPLICATION;
		row[applicationTypeMC.text] = *getApplicationTypeAsString(model::AITStream::DVB_HTML_APPLICATION);
		
		//TRANSPORT PROTOCOL COMBO
		appProtocolComboModel = Gtk::ListStore::create(protocolMC);
		appProtocolCombo.set_model(appProtocolComboModel);
		appProtocolCombo.pack_start(protocolMC.text);
		row = *(appProtocolComboModel->append());
		row[protocolMC.protocol] = model::AITApplication::OC_PROTOCOL_TYPE;
		row[protocolMC.text] = OC_APPLICATION_PROTOCOL;
		
		row = *(appProtocolComboModel->append());
		row[protocolMC.protocol] = model::AITApplication::IP_PROTOCOL_TYPE;
		row[protocolMC.text] = IP_APPLICATION_PROTOCOL;
	}
	
	const Glib::ustring* AITEditionFrame::getControlCodeAsString(model::AITApplication::ControlCodeType controlCode){
		switch(controlCode) {
			case model::AITApplication::AUTOSTART_CONTROL_CODE:
				return &AUTOSTART_CONTROL_CODE;
			case model::AITApplication::PRESENT_CONTROL_CODE:
				return &PRESENT_CONTROL_CODE;
			case model::AITApplication::DESTROY_CONTROL_CODE:
				return &DESTROY_CONTROL_CODE;
			case model::AITApplication::KILL_CONTROL_CODE:
				return &KILL_CONTROL_CODE;
			case model::AITApplication::PREFETCH_CONTROL_CODE:
				return &PREFETCH_CONTROL_CODE;
			case model::AITApplication::REMOTE_CONTROL_CODE:
				return &REMOTE_CONTROL_CODE;
			default:
				return &UNKNOWN;
		}
	}
	
	const Glib::ustring* AITEditionFrame::getApplicationTypeAsString(model::AITStream::ApplicationType applicationType){
		switch(applicationType) {
			case model::AITStream::DVB_J_APPLICATION:
				return &DVB_J_APPLICATION_TYPE;
			case model::AITStream::DVB_HTML_APPLICATION:
				return &DVB_HTML_APPLICATION_TYPE;
			default:
				return &UNKNOWN;
		}
	}
	
	const Glib::ustring* AITEditionFrame::getProtocolAsString(model::AITApplication::TransportProtocolType protocol){
		switch(protocol) {
			case model::AITApplication::OC_PROTOCOL_TYPE:
				return &OC_APPLICATION_PROTOCOL;
			case model::AITApplication::IP_PROTOCOL_TYPE:
				return &IP_APPLICATION_PROTOCOL;
			default:
				return &UNKNOWN;
		}
	}
	
	void AITEditionFrame::onAddButtonClicked(){
		
		model::AITProceduralApplication* application = new model::AITProceduralApplication();
		
		application->setOrganizationID(util::strHexaToInt(appOrgIDEntry.get_text().raw())); 
		application->setApplicationID(util::strHexaToInt(appIDEntry.get_text().raw()));
		Gtk::TreeIter iter = controlCodeCombo.get_active();
		if(iter){
			Gtk::TreeRow row = *iter;
			application->setControlCode(row[controlCodeMC.controlCode]);
		}
		
		application->setVersion(util::strHexaToInt(appMajorVersionEntry.get_text().raw()),
				util::strHexaToInt(appMinorVersionEntry.get_text().raw()),util::strHexaToInt(appMicroVersionEntry.get_text().raw()));
		iter = appProtocolCombo.get_active();
		if(iter){
			Gtk::TreeRow row = *iter;
			application->setProtocol(row[protocolMC.protocol]);
		}
		//appRemoteCheckBt
		application->setName(appNameEntry.get_text());
		application->setLocator(appLocatorEntry.get_text());
		application->setBaseDirectory(appBaseDirEntry.get_text());
		application->setInitialClass(appInitClassEntry.get_text());
		application->setParameter(appParamsEntry.get_text());
		application->setClasspathExtension(appExtClasspathEntry.get_text());
		
		//model::AITProceduralApplication* application = new model::
		
		
		model::AITStream::ApplicationType appType = model::AITStream::DVB_J_APPLICATION;
		iter = appTypeCombo.get_active();
		if(iter){
			Gtk::TreeRow row = *iter;
			appType = row[applicationTypeMC.applicationType];
		}
		
		
		model::ElementaryStreamsSet* streamsSet = aitTree->getModel();
		if(streamsSet==NULL) {
			streamsSet = new model::ElementaryStreamsSet();
			aitTree->setModel(streamsSet);
		}else
			std::cerr<<"yes model"<< std::endl;
		model::AITStream* tableStream;
		int pid = util::strHexaToInt(pidEntry.get_text().raw());
		model::ElementaryStream* stream = streamsSet->getStreamByPID(pid);
		if(stream != NULL){ //IF EXISTS A STREAM WITH THIS PID
			if(stream->getType() != model::ElementaryStream::AIT_ELEMENTARY_STREAM){ //IF THE  TYPE OF THE STREAM IS AIT
				Gtk::Window* parentWindow = dynamic_cast<Gtk::Window*>(get_toplevel());
				if(parentWindow){
					Gtk::MessageDialog dialog(*parentWindow,_("The PID exists with another stream type!"), false,
							Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
					dialog.set_secondary_text(_("Do you wish to continue replacing it?"));
					if(dialog.run() == Gtk::RESPONSE_CANCEL)
						return;
				}else{
					Gtk::MessageDialog dialog(_("The PID exists with another stream type!"), false,
							Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
					dialog.set_secondary_text(_("Do you wish to continue replacing it?"));
					if(dialog.run() == Gtk::RESPONSE_CANCEL)
						return;
				}
				
				streamsSet->removeStream(stream);
				tableStream = new model::AITStream(appType);
				tableStream->addApplication(application);
				//stream = new model::ElementaryStream(table);
				tableStream->setPID(pid);
				streamsSet->addStream(tableStream);
			}else{
				tableStream = dynamic_cast<model::AITStream*>(stream);
				if(tableStream!=NULL){
					if(tableStream->getApplicationType() != appType){
						Gtk::Window* parentWindow = dynamic_cast<Gtk::Window*>(get_toplevel());
						if(parentWindow){
							Gtk::MessageDialog dialog(*parentWindow,_("This PID represents a stream that signals another type of application!"), false,
									Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
							dialog.set_secondary_text(_("Do you wish to continue replacing it?"));
							if(dialog.run() == Gtk::RESPONSE_CANCEL)
								return;
						}else{
							Gtk::MessageDialog dialog(_("This PID represents a stream that signals another type of application!"), false,
									Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
							dialog.set_secondary_text(_("Do you wish to continue replacing it?"));
							if(dialog.run() == Gtk::RESPONSE_CANCEL)
								return;
						}
						streamsSet->removeStream(stream);
						tableStream = new model::AITStream(appType);
						tableStream->addApplication(application);
						//stream = new model::ElementaryStream(table);
						tableStream->setPID(pid);
						streamsSet->addStream(tableStream);
					}else{
						tableStream->addApplication(application);
					}
				}
			}
		}else{
			tableStream = new model::AITStream(appType);
			tableStream->addApplication(application);
			//stream = new model::ElementaryStream(table);
			tableStream->setPID(pid);
			streamsSet->addStream(tableStream);
		}
	}
	
	void AITEditionFrame::onClearButtonClicked(){
		appNameEntry.set_text("");
		appOrgIDEntry.set_text("");
		appIDEntry.set_text("");
		controlCodeCombo.unset_active();
		appTypeCombo.unset_active();
		//appVersionEntry.set_text("");
		appMajorVersionEntry.set_text("");
		appMinorVersionEntry.set_text("");
		appMicroVersionEntry.set_text("");
		appProtocolCombo.unset_active();
		appRemoteCheckBt.set_active(false);
		appLocatorEntry.set_text("");
		appBaseDirEntry.set_text("");
		appExtClasspathEntry.set_text("");
		appParamsEntry.set_text("");
		appInitClassEntry.set_text("");
	}
   }
  }
 }
}
