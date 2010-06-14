#ifndef EPGTREEVIEW_H_
#define EPGTREEVIEW_H_

#include <gtkmm.h>
#include <glibmm/i18n.h>

#include <vector>
#include <iostream>

#include "EPGModelColumn.h"
#include "../mpeg/si/Eit.h"
#include "../mpeg/si/Pat.h"
#include "../mpeg/Muxer.h"
#include "../functions.h"
#include "ProgramNumberComboBoxModel.h"
#include "LanguageComboBoxModel.h"

using namespace org::isdtv::contentProvider::mpeg;
//using namespace org::isdtv::util;
using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
	class EPGTreeView : public Gtk::TreeView {
		private:
			Glib::RefPtr<Gtk::TreeStore> refTreeModel;
			EPGModelColumn columnModel;
			Gtk::Menu popupMenu;
			
			Glib::RefPtr<Gtk::ListStore> refComboBoxModel;
			ProgramNumberComboBoxModel comboBoxModel;
			
			Glib::RefPtr<Gtk::ListStore> refComboBoxModelLang;
			LanguageComboBoxModel comboBoxModelLang;
			
		public:
			
		protected:

		public:
	
			EPGTreeView();
			virtual ~EPGTreeView();
			void updateProgramNumbers();
			bool on_button_press_event(GdkEventButton* event);
			void addRow();
			void removeRow();
			void encodeEIT();
			void onEditingProgramNumberCell(
					const Glib::ustring& path_string,
					const Glib::ustring& new_text);

			void onEditingLanguageCell(
					const Glib::ustring& path_string,
					const Glib::ustring& new_text);
	};
   }
  }
 }
}


#endif /*EPGTREEVIEW_H_*/
