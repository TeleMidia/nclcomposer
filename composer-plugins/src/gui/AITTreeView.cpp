#include "../../include/gui/AITTreeView.h"
#include "../../include/gui/AITEditionFrame.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
   	//PUBLIC CONSTANTS
   	const int AITTreeView::AUTOSTART_CONTROL_CODE = 0;
	const int AITTreeView::PRESENT_CONTROL_CODE = 1;
	const int AITTreeView::DESTROY_CONTROL_CODE = 2;
	const int AITTreeView::KILL_CONTROL_CODE = 3;
	const int AITTreeView::PREFETCH_CONTROL_CODE = 4;
	const int AITTreeView::REMOTE_CONTROL_CODE = 5;

	const int AITTreeView::DVB_J_APPLICATION_TYPE = 0;
	const int AITTreeView::DVB_HTML_APPLICATION_TYPE = 1;
	
	const int AITTreeView::OC_APPLICATION_PROTOCOL = 0;
	const int AITTreeView::IP_APPLICATION_PROTOCOL = 1;
	
   	//PRIVATE CONSTANTS
   	const Glib::ustring AITTreeView::NAME_LABEL = _("NAME: ");
   	const Glib::ustring AITTreeView::ORGANIZATION_ID_LABEL = _("ORGANIZATION ID: ");
	const Glib::ustring AITTreeView::APPLICATION_ID_LABEL = _("APPLICATION ID: ");
	const Glib::ustring AITTreeView::CONTROL_CODE_LABEL = _("CONTROL-CODE: ");
	const Glib::ustring AITTreeView::APPLICATION_TYPE_LABEL = _("TYPE: ");
	const Glib::ustring AITTreeView::APPLICATION_VERSION_LABEL = _("VERSION: ");
	const Glib::ustring AITTreeView::APPLICATION_PROTOCOL_LABEL = _("PROTOCOL: ");
	const Glib::ustring AITTreeView::APPLICATION_REMOTE_FLAG_LABEL = _("REMOTE-FLAG: ");
	const Glib::ustring AITTreeView::APPLICATION_LOCATOR_LABEL = _("LOCATOR: ");
	const Glib::ustring AITTreeView::APPLICATION_BASE_DIRECTORY_LABEL = _("BASE DIR: ");
	const Glib::ustring AITTreeView::APPLICATION_EXTENSION_CLASSPATH_LABEL = _("EXT CLASSPATH: ");
	const Glib::ustring AITTreeView::APPLICATION_PARAMETERS_LABEL = _("PARAMETERS: ");
	const Glib::ustring AITTreeView::APPLICATION_INITIAL_CLASS_LABEL = _("INIT CLASS: "); 
   	
	AITTreeView::AITTreeView() {
		refTreeModel = Gtk::TreeStore::create(columnModel);
		set_model(refTreeModel);
		streamsSet = NULL;
		//Add the TreeView's view columns:fileName
		append_column(_("AIT"), columnModel.name);
		//append_column_numeric_editable(_("PID"), columnModel.pid,"0x%X");
		//append_column(_("Stuffing"), columnModel.stuffing);
		
		Gtk::Menu::MenuList& menulist = popupMenu.items();
		menulist.push_back(
			    Gtk::Menu_Helpers::MenuElem(_("_Remove"), sigc::mem_fun(*this,
			    	    &AITTreeView::onRemoveButtonCLicked)));
	}
	
	AITTreeView::~AITTreeView() {
	}
	
	bool AITTreeView::containsPID(int pid){
		Gtk::TreeIter iter = refTreeModel->children().begin();
		
		while(iter){
			Gtk::TreeRow row = *iter;
			if(row[columnModel.pid] == pid){
				return true;
			}
			iter++;
		}
		return false;
	}
	
	bool AITTreeView::addNewApplication(	int pid, bool stuffing, Glib::ustring* appName, int orgId, 
					int appId, int code,int type,Glib::ustring* version,int protocol,
					bool remote,Glib::ustring* locator,Glib::ustring* baseDir,
					Glib::ustring* extClasspath,Glib::ustring* parameters, Glib::ustring* init){
		Gtk::TreeRow newRow;
		
		//FINDING PID
		if(!containsPID(pid)){  //IF not found create PID ROW
			newRow = *(refTreeModel->append());
			newRow[columnModel.pid] = pid;
			newRow[columnModel.name] = _("PID: ") + util::intToStrHexa(pid);
			newRow = *(refTreeModel->append(newRow->children()));
		}else{
			Gtk::TreeIter iter = refTreeModel->children().begin();
			while(iter){
				Gtk::TreeRow row = *iter;
				if(row[columnModel.pid] == pid){
					newRow = *(refTreeModel->append(row->children()));
				}
				iter++;
			}
		}
		
		newRow[columnModel.name] = NAME_LABEL + *appName;
		Gtk::TreeRow newRow2 = *(refTreeModel->append(newRow->children()));
		string strHexa = util::intToStrHexa(orgId);
		newRow2[columnModel.name] = ORGANIZATION_ID_LABEL + strHexa;

		newRow2 = *(refTreeModel->append(newRow->children()));
		strHexa = util::intToStrHexa(appId);
		newRow2[columnModel.name] = APPLICATION_ID_LABEL + strHexa;

		newRow2 = *(refTreeModel->append(newRow->children()));
		strHexa = util::intToStrHexa(code); //TODO: substuir por switch.
		newRow2[columnModel.name] = CONTROL_CODE_LABEL + strHexa;

		newRow2 = *(refTreeModel->append(newRow->children()));
		strHexa = util::intToStrHexa(type); //TODO: substuir por switch.
		newRow2[columnModel.name] = APPLICATION_TYPE_LABEL + strHexa;

		newRow2 = *(refTreeModel->append(newRow->children()));
		newRow2[columnModel.name] = APPLICATION_VERSION_LABEL + *version;
		newRow2 = *(refTreeModel->append(newRow->children()));
		strHexa = util::intToStrHexa(type); //TODO: substuir por switch.
		newRow2[columnModel.name] = APPLICATION_PROTOCOL_LABEL + strHexa;

		return true;
	}
	
	void AITTreeView::refresh() {
		refTreeModel->clear();
		vector<model::ElementaryStream*> streams = streamsSet->getStreams();
		
		for(unsigned int i=0; i < streams.size(); i++){
			if(streams[i] == NULL || 
					streams[i]->getType() != model::ElementaryStream::AIT_ELEMENTARY_STREAM)
				continue;
			
			Gtk::TreeRow pidRow = *(refTreeModel->append());
			pidRow[columnModel.pid] = streams[i]->getPID();
			string strTemp = util::intToStrHexa(streams[i]->getPID());
			pidRow[columnModel.name] = _("PID: ") + strTemp;

			model::AITStream* table = dynamic_cast<model::AITStream*>(streams[i]);
			if(table!=NULL){
				vector<model::AITApplication*> apps = table->getApplications();
				for(unsigned int j=0;j<apps.size();j++){
					model::AITApplication* application = apps[j];
					if(application == NULL)
						continue;
					Gtk::TreeRow newRow = *(refTreeModel->append(pidRow->children()));
					newRow[columnModel.name] = NAME_LABEL + application->getName();
					Gtk::TreeRow newRow2 = *(refTreeModel->append(newRow->children()));
					strTemp = util::intToStrHexa(application->getOrganizationID());
					newRow2[columnModel.name] = ORGANIZATION_ID_LABEL + strTemp;

					newRow2 = *(refTreeModel->append(newRow->children()));
					strTemp = util::intToStrHexa(application->getApplicationID());
					newRow2[columnModel.name] = APPLICATION_ID_LABEL + strTemp;

					newRow2 = *(refTreeModel->append(newRow->children()));
					newRow2[columnModel.name] = CONTROL_CODE_LABEL + *AITEditionFrame::getControlCodeAsString(application->getControlCode());
					newRow2 = *(refTreeModel->append(newRow->children()));
					newRow2[columnModel.name] = APPLICATION_TYPE_LABEL + *AITEditionFrame::getApplicationTypeAsString(table->getApplicationType());
					
					model::AITApplication::VersionType version = application->getVersion();
					newRow2 = *(refTreeModel->append(newRow->children()));
					//strTemp = itoa(version.majorVersion); 
					newRow2[columnModel.name] = APPLICATION_VERSION_LABEL + util::itos(version.major) + "." +util::itos(version.minor) +
								"." + util::itos(version.micro);
					newRow2 = *(refTreeModel->append(newRow->children()));
					newRow2[columnModel.name] = APPLICATION_PROTOCOL_LABEL + *AITEditionFrame::getProtocolAsString(application->getProtocol());
				}
			}
		}
	}
	
	void AITTreeView::setModel(model::ElementaryStreamsSet* elementaryStreams){
		if(streamsSet != NULL)
			streamsSet->detachStreamsObserver(this);
		this->streamsSet = elementaryStreams;
		streamsSet->attachStreamsObserver(this);
		refresh();
	}
	
	model::ElementaryStreamsSet* AITTreeView::getModel() {
		return streamsSet;
	}
	
	void AITTreeView::onRemoveButtonCLicked() {
		Glib::RefPtr<Gtk::TreeSelection> refTreeSelection = 
				get_selection();
		Gtk::TreeIter iter = refTreeSelection->get_selected();
		if(iter){
			Gtk::Window* parentWindow = dynamic_cast<Gtk::Window*>(get_toplevel());
			if(parentWindow){
				Gtk::MessageDialog dialog(*parentWindow,_("Do you really wich to remove the AIT?"), false,
						Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
				dialog.set_secondary_text(_("If you continue all the AIT and its application will be removed."));
				if(dialog.run() == Gtk::RESPONSE_CANCEL)
					return;
			}else{
				Gtk::MessageDialog dialog(_("Do you really wich to remove the AIT?"), false,
						Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
				dialog.set_secondary_text(_("If you continue all the AIT and its application will be removed."));
				if(dialog.run() == Gtk::RESPONSE_CANCEL)
					return;
			}
			
			Gtk::TreeRow row = *iter;
			while(row.parent()) { //Finding the root
				iter = row.parent();
				row = *iter;
			}
			int pid = row[columnModel.pid];
			model::ElementaryStreamsSet* esSet = getModel();
			
			model::ElementaryStream* stream = esSet->getStreamByPID(pid);
			if(stream != NULL){
				esSet->removeStream(stream);
			}
			
			refTreeModel->erase(iter);
		}	
	}
	
	bool AITTreeView::on_button_press_event(GdkEventButton* event){
		TreeView::on_button_press_event(event);
		if((event->type == GDK_BUTTON_PRESS) && (event->button == 3)){
			popupMenu.popup(event->button,event->time);
			return true;
		}
		return false;
	}
	
	void AITTreeView::onElementaryStreamRemoved(model::ElementaryStreamsSet* source, model::ElementaryStream* stream){
		if(source != streamsSet)
			return;
		if(stream->getType() == model::ElementaryStream::AIT_ELEMENTARY_STREAM){
			model::AITStream* table = dynamic_cast<model::AITStream*>(stream);
			if(table != NULL)
				table->detachTableObserver(this);
		}
		refresh();//TODO optimize
	}
	
	void AITTreeView::onElementaryStreamAdded(model::ElementaryStreamsSet* source, model::ElementaryStream* stream){
		if(source != streamsSet)
			return;
		if(stream->getType() == model::ElementaryStream::AIT_ELEMENTARY_STREAM){
			model::AITStream* table = dynamic_cast<model::AITStream*>(stream);
			if(table!=NULL)
				table->attachTableObserver(this);
		}
		refresh();//TODO optimize
	}
	
	void AITTreeView::onApplicationRemoved(model::AITStream* source, model::AITApplication* application) {
		//TODO optimize
		refresh();
	}
	
	void AITTreeView::onApplicationAdded(model::AITStream* source, model::AITApplication* application) {
		//TODO optimize
		refresh();
	}
	
   }
  }
 }
}
