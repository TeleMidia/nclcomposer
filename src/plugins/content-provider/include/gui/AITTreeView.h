#ifndef AITTREEVIEW_H_
#define AITTREEVIEW_H_

#include <gtkmm.h>
#include <glibmm/i18n.h>

#include <vector>
#include "AITModelColumn.h"
#include "../functions.h"
#include "../model.h"

//using namespace org::isdtv::util;
using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
	class AITTreeView : public Gtk::TreeView, 
						public model::observers::ElementaryStreamsSetObserver,
						public model::observers::AITTableObserver {
		private:
			const static Glib::ustring NAME_LABEL;
			const static Glib::ustring ORGANIZATION_ID_LABEL;
			const static Glib::ustring APPLICATION_ID_LABEL;
			const static Glib::ustring CONTROL_CODE_LABEL;
			const static Glib::ustring APPLICATION_TYPE_LABEL;
			const static Glib::ustring APPLICATION_VERSION_LABEL;
			const static Glib::ustring APPLICATION_PROTOCOL_LABEL;
			const static Glib::ustring APPLICATION_REMOTE_FLAG_LABEL;
			const static Glib::ustring APPLICATION_LOCATOR_LABEL;
			const static Glib::ustring APPLICATION_BASE_DIRECTORY_LABEL;
			const static Glib::ustring APPLICATION_EXTENSION_CLASSPATH_LABEL;
			const static Glib::ustring APPLICATION_PARAMETERS_LABEL;
			const static Glib::ustring APPLICATION_INITIAL_CLASS_LABEL;
			
			Glib::RefPtr<Gtk::TreeStore> refTreeModel;
			AITModelColumn columnModel;
			Gtk::Menu popupMenu;
			
			model::ElementaryStreamsSet* streamsSet;
			
			bool containsPID(int pid);
			void refresh();
		public:
			const static int AUTOSTART_CONTROL_CODE;
			const static int PRESENT_CONTROL_CODE;
			const static int DESTROY_CONTROL_CODE;
			const static int KILL_CONTROL_CODE;
			const static int PREFETCH_CONTROL_CODE;
			const static int REMOTE_CONTROL_CODE;
		
			const static int DVB_J_APPLICATION_TYPE;
			const static int DVB_HTML_APPLICATION_TYPE;
			
			const static int OC_APPLICATION_PROTOCOL;
			const static int IP_APPLICATION_PROTOCOL;
			
		protected:
			bool on_button_press_event(GdkEventButton* event);
			
		public:
		
			AITTreeView();
			virtual ~AITTreeView();
			
			bool addNewApplication(	int pid, bool stuffing, Glib::ustring* appName, int orgId, 
					int appId, int code,int type,Glib::ustring* version,int protocol,
					bool remote,Glib::ustring* locator,Glib::ustring* baseDir,
					Glib::ustring* extClasspath,Glib::ustring* parameters, Glib::ustring* init);
			
			void setModel(model::ElementaryStreamsSet* elementaryStreams);
			model::ElementaryStreamsSet* getModel();
			
			virtual void onRemoveButtonCLicked();
			
		//////OBSERVER
			virtual void onElementaryStreamRemoved(model::ElementaryStreamsSet* source, model::ElementaryStream* stream);
			virtual void onElementaryStreamAdded(model::ElementaryStreamsSet* source, model::ElementaryStream* stream);
			virtual void onApplicationRemoved(model::AITStream* source, model::AITApplication* application);
			virtual void onApplicationAdded(model::AITStream* source, model::AITApplication* application);
	};
   }
  }
 }
}

#endif /*AITTREEVIEW_H_*/
