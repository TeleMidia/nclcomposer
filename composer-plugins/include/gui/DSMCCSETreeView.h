#ifndef DSMCCSETREEVIEW_H_
#define DSMCCSETREEVIEW_H_

#include <gtkmm.h>
#include <glibmm/i18n.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

#include "../functions.h"
using namespace ::org::isdtv::util;

#include "../mpeg/Muxer.h"
using namespace ::org::isdtv::contentProvider::mpeg;

#include "DSMCCSEModelColumn.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	class DSMCCSETreeView : public Gtk::TreeView  {
		private:
			Glib::RefPtr<Gtk::TreeStore> refTreeModel;
			DSMCCSEModelColumn columnModel;
			Gtk::Menu popupMenu;

		public:
			DSMCCSETreeView();
			virtual ~DSMCCSETreeView();

		protected:
			virtual bool on_button_press_event(GdkEventButton* event);

		public:
			void addNewEvent(
					unsigned int eventId,
					unsigned int npt, string privateData);

			void removeEvent(unsigned int eventId);
			void onRemoveButtonClick();
			std::vector<SEInfo*>* getStreamEventsInfo();
			void clearStreamEvents();
	};
}
}
}
}

#endif /*DSMCCSETREEVIEW_H_*/
