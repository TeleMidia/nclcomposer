#ifndef DSMCCOCPREVIEWMODELCOLUMN_H_
#define DSMCCOCPREVIEWMODELCOLUMN_H_
#include <gtkmm/treemodel.h>

#include "DSMCCOCModelColumn.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
  	class DSMCCOCPreviewModelColumn : public Gtk::TreeModel::ColumnRecord {
		private:
			
		public:
			Gtk::TreeModelColumn<service_domain_type> serviceDomain;
			Gtk::TreeModelColumn<Glib::ustring> pid;
			Gtk::TreeModelColumn<Glib::ustring> programNumber;
			Gtk::TreeModelColumn<Glib::ustring> componentTag;

			DSMCCOCPreviewModelColumn() {
				add(serviceDomain);
				add(pid);
				add(programNumber);
				add(componentTag);
			}
	};
  }
 }
}
#endif /*DSMCCOCPREVIEWMODELCOLUMN_H_*/
