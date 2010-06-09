#ifndef DSMCCPAGE_H_
#define DSMCCPAGE_H_

#include <gtkmm.h>

#include "../mpeg/Muxer.h"
using namespace org::isdtv::contentProvider::mpeg;

#include "../mpeg/dsmcc/OCStreamFactory.h"
#include "../mpeg/dsmcc/SEDStreamFactory.h"

#include "MuxTreeView.h"
#include "DSMCCOCTreeView.h"
#include "DSMCCSETreeView.h"
//#include "DSMCCOCPreviewFrame.h"
#include "DSMCCStreamEventFrame.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	class DSMCCPage {
		private:
			MuxTreeView* muxTreeView;
			DSMCCOCTreeView* dsmccObjectCarouselView;
			DSMCCSETreeView* dsmccStreamEventView;
			DSMCCOCPreviewFrame* dsmccObjectCarouselFrame;
			DSMCCStreamEventFrame* dsmccStreamEventFrame;
			Gtk::VBox muxBox;
			Gtk::HPaned dsmccPanel;

			Gtk::Label* label;
			Gtk::CheckButton* isEditing;

			map<string, int>* commandMap;
			Gtk::ComboBoxText commandsCombo;

			Gtk::Label* baseIdLabel;
			Gtk::Entry baseIdEntry;

			Gtk::Label* documentIdLabel;
			Gtk::Entry documentIdEntry;

			Gtk::Label* compositeIdLabel;
			Gtk::Entry compositeIdEntry;

			Gtk::Frame leftFrame;
		    Gtk::ScrolledWindow leftScrolledWindow;
		    Gtk::Frame rightFrame;
			Gtk::ScrolledWindow rightScrolledWindow;

			string currentCommandTag;
			string currentXmlFileName;

			Gtk::Button addSEButton;
			Gtk::Button sendSEButton;
			Gtk::Button xmlButton;
			Gtk::Button multiplexButton;

			int currentEventId;
			vector<string>* ocDirs;

		public:
			DSMCCPage(MuxTreeView* muxTreeView);
			virtual ~DSMCCPage();
			Gtk::VBox* getPageBox();
			virtual void add();
			virtual void updateView();

		private:
			string getFilePath(string uri);
			bool hasParent(string child);
			bool isChild(string parent, string child);
			void initializeNCLCommands();
			void hideAllEditingWidgets();
			void on_isEditing_clicked();
			void on_combo_changed();
			void on_xmlButton_clicked();
			void on_addSEButton_clicked();
			void on_sendSEButton_clicked();
			void on_multiplexButton_clicked();
	};
}
}
}
}

#endif /*DSMCCPAGE_H_*/
