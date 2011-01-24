#ifndef LOGOMODELCOLUMN_H_
#define LOGOMODELCOLUMN_H_

#include <string>
using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
  	namespace gui {
	//Tree model columns:
		class LogoModelColumn : public Gtk::TreeModel::ColumnRecord {
			private:
			public:
				Gtk::TreeModelColumn<string> programNumber;
				Gtk::TreeModelColumn<string> logoFilename;

				LogoModelColumn() {
					add(programNumber);
					add(logoFilename);
				}
		};
  	}
  }
 }
}

#endif /*LOGOMODELCOLUMN_H_*/
