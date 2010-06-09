#include "../../include/gui/MuxTreeStore.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
	MuxTreeStore::MuxTreeStore() {
		//set_column_types()
	}
	MuxTreeStore::MuxTreeStore(const MuxModelColumn& modelColumns)
	: Gtk::TreeStore(modelColumns){
		columns = &modelColumns;
	}
	
	MuxTreeStore::~MuxTreeStore() {
	}
	
	bool MuxTreeStore::row_drop_possible(const Gtk::TreeModel::Path& dest_path, const Gtk::SelectionData& selection_data) const{
		std::cout << "teste." << std::endl;
		return false;
	}
	bool MuxTreeStore::row_drop_possible_vfunc(const Gtk::TreeModel::Path& dest_path, const Gtk::SelectionData& selection_data) const {
		Gtk::TreePath path;
		Glib::RefPtr<TreeModel> model;
		if(Gtk::TreePath::get_from_selection_data(selection_data,model,path)){
			Gtk::TreeIter iter = model->get_iter(path);
			Gtk::TreeRow row = *iter;
			Glib::ustring str = row->get_value(columns->name);
			if(path.size() == dest_path.size())
				return true;
		}
		return false;
	}
	
	Glib::RefPtr<MuxTreeStore> MuxTreeStore::create(const MuxModelColumn& columns){
		Glib::RefPtr<MuxTreeStore> refMuxTreeStore(new MuxTreeStore(columns));
		std::cout << "Created new MuxTreeStore." << std::endl;
		return refMuxTreeStore;
	}
   }
  }
 }
}
