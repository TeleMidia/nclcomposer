#ifndef EPGPAGE_H_
#define EPGPAGE_H_

#include <gtkmm.h>

#include "EPGTreeView.h"
#include "LogoTreeView.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	class EPGPage {
		private:
			EPGTreeView* epgTree;
			LogoTreeView* logoTree;
			Gtk::VBox epgBox;
			Gtk::Button multiplexButton;

		public:
			EPGPage();
			virtual ~EPGPage();
			Gtk::VBox* getPageBox();
			virtual void updateView();
			void on_multiplexButton_clicked();

		private:

	};
}
}
}
}

#endif /*EPGPAGE_H_*/
