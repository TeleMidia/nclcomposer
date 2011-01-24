#ifndef LOGOTREEVIEW_H_
#define LOGOTREEVIEW_H_

#include <gtkmm.h>
#include <glibmm/i18n.h>

#include <vector>
#include <iostream>

#include "LogoModelColumn.h"
#include "../mpeg/si/Sdt.h"
#include "../mpeg/si/Cdt.h"
#include "../mpeg/si/Pat.h"
#include "../mpeg/Muxer.h"
#include "../functions.h"
#include "ProgramNumberComboBoxModel.h"

using namespace org::isdtv::contentProvider::mpeg;
//using namespace org::isdtv::util;
using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
	class LogoTreeView : public Gtk::TreeView {
		private:
			Glib::RefPtr<Gtk::TreeStore> refTreeModel;
			LogoModelColumn columnModel;
			Gtk::Menu popupMenu;
			
			Glib::RefPtr<Gtk::ListStore> refComboBoxModel;
			ProgramNumberComboBoxModel comboBoxModel;
			
		public:
			
		protected:

		public:
	
			LogoTreeView();
			virtual ~LogoTreeView();
			void updateProgramNumbers();
			bool on_button_press_event(GdkEventButton* event);
			void onAddButtonClick(Gtk::TreeIter iter);
			void addRow();
			void removeRow();
			bool validatePNG();
			bool encodeLogo();
			void onEditingProgramNumberCell(
					const Glib::ustring& path_string,
					const Glib::ustring& new_text);
	};
   }
  }
 }
}

#endif /*LOGOTREEVIEW_H_*/
