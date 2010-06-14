#ifndef MUXMODELCOLUMN_H_
#define MUXMODELCOLUMN_H_

#include <gtkmm/treemodel.h>

namespace org {
 namespace isdtv {
  namespace contentProvider {
	//Tree model columns:
	class MuxModelColumn : public Gtk::TreeModel::ColumnRecord {
		public:
			Gtk::TreeModelColumn<int> id;
			Gtk::TreeModelColumn<Glib::ustring> name;
			Gtk::TreeModelColumn<bool> transmit;

			MuxModelColumn() {
				add(id);
				add(name);
				add(transmit);
			}
	};
  }
 }
}

#endif /*MODELCOLUMN_H_*/
