#ifndef EPGMODELCOLUMN_H_
#define EPGMODELCOLUMN_H_

#include <string>
using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
  	namespace gui {
	//Tree model columns:
		class EPGModelColumn : public Gtk::TreeModel::ColumnRecord {
			private:
			public:
				Gtk::TreeModelColumn<string> start;
				Gtk::TreeModelColumn<string> duration;
				Gtk::TreeModelColumn<string> language;
				Gtk::TreeModelColumn<string> name;
				Gtk::TreeModelColumn<string> description;
				Gtk::TreeModelColumn<string> programNumber;

				EPGModelColumn() {
					add(start);
					add(duration);
					add(language);
					add(name);
					add(description);
					add(programNumber);
				}
		};
  	}
  }
 }
}

#endif /*EPGMODELCOLUMN_H_*/
