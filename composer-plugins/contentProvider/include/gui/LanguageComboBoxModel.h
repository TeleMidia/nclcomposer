#ifndef LANGUAGECOMBOBOXMODEL_H_
#define LANGUAGECOMBOBOXMODEL_H_

#include <gtkmm/treemodel.h>

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
   	class LanguageComboBoxModel : public Gtk::TreeModel::ColumnRecord {
			private:
			public:
				Gtk::TreeModelColumn<Glib::ustring> code;
				//Gtk::TreeModelColumn<Glib::ustring> name;
	
				LanguageComboBoxModel() {
					add(code);
					//add(name);
				}
	};
   }
  }
 }
}

#endif /*LANGUAGECOMBOBOXMODEL_H_*/
