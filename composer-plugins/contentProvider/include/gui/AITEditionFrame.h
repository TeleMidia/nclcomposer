#ifndef AITEDITIONFRAME_H_
#define AITEDITIONFRAME_H_

#include <gtkmm.h>
#include <glibmm/i18n.h>

#include "AITTreeView.h"
#include "../model.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
	class AITEditionFrame : public Gtk::Frame {
	////////CONSTANTS
		public:
			const static Glib::ustring AUTOSTART_CONTROL_CODE;
			const static Glib::ustring PRESENT_CONTROL_CODE;
			const static Glib::ustring DESTROY_CONTROL_CODE;
			const static Glib::ustring KILL_CONTROL_CODE;
			const static Glib::ustring PREFETCH_CONTROL_CODE;
			const static Glib::ustring REMOTE_CONTROL_CODE;
		
			const static Glib::ustring DVB_J_APPLICATION_TYPE;
			const static Glib::ustring DVB_HTML_APPLICATION_TYPE;
			
			const static Glib::ustring OC_APPLICATION_PROTOCOL;
			const static Glib::ustring IP_APPLICATION_PROTOCOL;
			
			
			const static Glib::ustring UNKNOWN;
	///TYPES DEFINITION
		protected:
			class ControlCodeModelColumns : public Gtk::TreeModel::ColumnRecord{
				public:
					Gtk::TreeModelColumn<model::AITApplication::ControlCodeType> controlCode;
					Gtk::TreeModelColumn<Glib::ustring> text;
					ControlCodeModelColumns(){add(controlCode); add(text);}
			};
			
			class ApplicationTypeModelColumns : public Gtk::TreeModel::ColumnRecord{
				public:
					Gtk::TreeModelColumn<model::AITStream::ApplicationType> applicationType;
					Gtk::TreeModelColumn<Glib::ustring> text;
					ApplicationTypeModelColumns(){add(applicationType); add(text);}
			};
			
			class ProtocolModelColumns : public Gtk::TreeModel::ColumnRecord{
				public:
					Gtk::TreeModelColumn<model::AITApplication::TransportProtocolType> protocol;
					Gtk::TreeModelColumn<Glib::ustring> text;
					ProtocolModelColumns(){add(protocol); add(text);}
			};
			
	////////VIEW
		private:	
			ControlCodeModelColumns controlCodeMC;
			ApplicationTypeModelColumns applicationTypeMC;
			ProtocolModelColumns protocolMC;

			AITTreeView* aitTree;
			
		//GUI WIDGETS
			//AIT
			Gtk::Entry pidEntry;
			Gtk::Entry aitVersionEntry;
			Gtk::CheckButton bitStufingCheckBt;
			
			//APPLICATION
			Gtk::Entry appNameEntry;
			Gtk::Entry appOrgIDEntry;
			Gtk::Entry appIDEntry;
			
			Glib::RefPtr<Gtk::ListStore> controlCodeComboModel; 
			Gtk::ComboBox controlCodeCombo;
			
			Glib::RefPtr<Gtk::ListStore> appTypeComboModel; 
			Gtk::ComboBox appTypeCombo;
			
			Glib::RefPtr<Gtk::ListStore> appProtocolComboModel;
			Gtk::ComboBox appProtocolCombo;
			
			//Gtk::Entry appVersionEntry;
			Gtk::Entry appMajorVersionEntry;
			Gtk::Entry appMinorVersionEntry;
			Gtk::Entry appMicroVersionEntry;
			Gtk::CheckButton appRemoteCheckBt;
			Gtk::Entry appLocatorEntry;
			Gtk::Entry appBaseDirEntry;
			Gtk::Entry appExtClasspathEntry;
			Gtk::Entry appParamsEntry;
			Gtk::Entry appInitClassEntry;
			
			Gtk::Button addButton;
			Gtk::Button clearButton;
			
			void populateCombos();
		public:
			AITEditionFrame(AITTreeView* aitTree);
			virtual ~AITEditionFrame();
			
			void onAddButtonClicked();
			void onClearButtonClicked();
			
			static const Glib::ustring* getControlCodeAsString(model::AITApplication::ControlCodeType controlCode);
			static const Glib::ustring* getApplicationTypeAsString(model::AITStream::ApplicationType applicationType);
			static const Glib::ustring* getProtocolAsString(model::AITApplication::TransportProtocolType protocol);
	};
   }
  }
 }
}

#endif /*AITEDITIONFRAME_H_*/

