#ifndef AITMODELCOLUMN_H_
#define AITMODELCOLUMN_H_

namespace org {
 namespace isdtv {
  namespace contentProvider {
  	namespace gui {
	//Tree model columns:
		class AITModelColumn : public Gtk::TreeModel::ColumnRecord {
			private:
			public:
				Gtk::TreeModelColumn<Glib::ustring> name;
				Gtk::TreeModelColumn<int> pid;
				Gtk::TreeModelColumn<bool> stuffing;
	
				AITModelColumn() {
					add(name);
					add(pid);
					add(stuffing);
				}
		};
  	}
  }
 }
}

#endif /*AITMODELCOLUMN_H_*/
