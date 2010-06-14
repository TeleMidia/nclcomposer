#ifndef MUXTREEVIEW_H_
#define MUXTREEVIEW_H_

#include <gtkmm.h>
#include <glibmm/i18n.h>

#include "../mpeg/Muxer.h"
using namespace org::isdtv::contentProvider::mpeg;

#include "../mpeg/si/Pat.h"
#include "../mpeg/si/Pmt.h"
using namespace org::isdtv::contentProvider::mpeg::si;

#include "../mpeg/dsmcc/PMTStreamFactory.h"

#include "MuxTreeStore.h"
#include "MuxModelColumn.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>
using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
	class MuxTreeView : public Gtk::TreeView {
		private:
			Glib::RefPtr<MuxTreeStore> refTreeModel;
			Gtk::Menu muxPopupMenu;
			MuxModelColumn grid;

		public:
			MuxTreeView();
			virtual ~MuxTreeView();

			void updateGrid();
			void onMenuFileSaveAs();
			void onAddButtonClick();
			void onRemoveButtonClick();

		protected:
			// Override Signal handler:
			virtual bool on_button_press_event(GdkEventButton *event);
			//virtual void on_menu_file_popup_generic();

			// event handlers
			virtual void onTreeViewRowActivated(
				    const Gtk::TreeModel::Path& path,
				    Gtk::TreeViewColumn* column);
	};
   }
  }
 }
}

#endif /*MUXTREEVIEW_H_*/
