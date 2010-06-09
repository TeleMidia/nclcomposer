/*
 * ResynchPage.h
 *
 *  Created on: 22/07/2008
 *      Author: Felippe Nagato
 */

#ifndef RESYNCHPAGE_H_
#define RESYNCHPAGE_H_

#include <gtkmm.h>

#include <iostream>
#include <vector>
#include <string>
#include "../mpeg/resynch/Joiner.h"
using namespace std;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	class ResynchPage {
		private:
			Gtk::VBox resynchBox;
			Gtk::Button resynchButton;
			Gtk::CheckButton* tsIdChkBox, *pIdsChkBox, *versionChkBox;
			Gtk::CheckButton* originalChkBox;
			Gtk::Entry input1Entry, input2Entry, outputEntry;
			Gtk::Label* input1Label, *input2Label, *outputLabel;

		public:
			ResynchPage();
			virtual ~ResynchPage();
			Gtk::VBox* getPageBox();
			void on_resynchButton_clicked();

		private:

	};
}
}
}
}

#endif /* RESYNCHPAGE_H_ */
