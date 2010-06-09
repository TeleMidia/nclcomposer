#ifndef MUXTREESTORE_H_
#define MUXTREESTORE_H_

#include <gtkmm.h>
#include <iostream>
#include "MuxModelColumn.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
	class MuxTreeStore : public Gtk::TreeStore  {
		private:
			const MuxModelColumn* columns;
		protected:
			MuxTreeStore(const MuxModelColumn& columns);
			virtual bool row_drop_possible_vfunc(const Gtk::TreeModel::Path& dest_path, const Gtk::SelectionData& selection_data) const;
		
		public:
			MuxTreeStore();
			virtual ~MuxTreeStore();
			bool row_drop_possible(const Gtk::TreeModel::Path& dest_path, const Gtk::SelectionData& selection_data) const;
			
			static Glib::RefPtr<MuxTreeStore> create(const MuxModelColumn& columns);
	};
   }
  }
 }
}

#endif /*MUXTREESTORE_H_*/
