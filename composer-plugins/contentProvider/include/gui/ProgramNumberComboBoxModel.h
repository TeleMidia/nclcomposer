#ifndef PROGRAMNUMBERCOMBOBOXMODEL_H_
#define PROGRAMNUMBERCOMBOBOXMODEL_H_
#include <gtkmm/treemodel.h>

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
   	class ProgramNumberComboBoxModel : public Gtk::TreeModel::ColumnRecord {
			private:
			public:
				Gtk::TreeModelColumn<Glib::ustring> programNumberChoice;
	
				ProgramNumberComboBoxModel() {
					add(programNumberChoice);
				}
	};
   }
  }
 }
}
#endif /*PROGRAMNUMBERCOMBOBOXMODEL_H_*/
