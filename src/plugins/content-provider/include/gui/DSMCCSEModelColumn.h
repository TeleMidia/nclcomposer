#ifndef DSMCCSEMODELCOLUMN_H_
#define DSMCCSEMODELCOLUMN_H_
#include <gtkmm/treemodel.h>

namespace org {
 namespace isdtv {
  namespace contentProvider {
  	namespace gui {
	//Tree model columns:
		class DSMCCSEModelColumn : public Gtk::TreeModel::ColumnRecord {
			private:
			public:
				Gtk::TreeModelColumn<Glib::ustring> programNumber;
				Gtk::TreeModelColumn<Glib::ustring> pid;
				Gtk::TreeModelColumn<Glib::ustring> componentTag;
				Gtk::TreeModelColumn<Glib::ustring> eventId;
				Gtk::TreeModelColumn<Glib::ustring> npt;
				Gtk::TreeModelColumn<Glib::ustring> privateData;

				DSMCCSEModelColumn() {
					add(programNumber);
					add(pid);
					add(componentTag);
					add(eventId);
					add(npt);
					add(privateData);
				}
		};
  	}
  }
 }
}
#endif /*DSMCCSEMODELCOLUMN_H_*/
