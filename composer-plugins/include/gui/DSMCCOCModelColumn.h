#ifndef DSMCCOCMODELCOLUMN_H_
#define DSMCCOCMODELCOLUMN_H_
#include <gtkmm/treemodel.h>

#include <string>

namespace org {
 namespace isdtv {
  namespace contentProvider {
  	
  	typedef unsigned int service_domain_type;
	//Tree model columns:
	class DSMCCOCModelColumn : public Gtk::TreeModel::ColumnRecord {
		private:
			
		public:
			//Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> >  thumbnail;
			Gtk::TreeModelColumn<service_domain_type> serviceDomain;
			Gtk::TreeModelColumn<Glib::ustring> serviceDomainText;
			Gtk::TreeModelColumn<Glib::ustring> name;
			Gtk::TreeModelColumn<Glib::ustring> ior;
			Gtk::TreeModelColumn<Glib::ustring> size;
			Gtk::TreeModelColumn<Glib::ustring> type;
			Gtk::TreeModelColumn<Glib::ustring> originalFile;

			DSMCCOCModelColumn() {
				add(serviceDomain);
				add(serviceDomainText);
				add(name);
				add(ior);
				add(size);
				add(type);
				add(originalFile);
			}
	};
  }
 }
}
#endif /*DSMCCOCMODELCOLUMN_H_*/
