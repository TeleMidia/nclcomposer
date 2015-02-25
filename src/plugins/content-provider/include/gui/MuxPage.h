#ifndef MUXPAGE_H_
#define MUXPAGE_H_

#include <gtkmm.h>

#include "MuxTreeView.h"
#include "DSMCCOCPreviewFrame.h"

#include "../Thread.h"
using namespace br::pucrio::telemidia::util;

#include "../server/UnicastServer.h"
#include "../server/MulticastServer.h"
using namespace org::isdtv::network::server;

#include <string>
#include <vector>
#include <iostream>
using namespace std;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
   	class MuxPage : public Thread {
   		private:
   			//widgets
   			MuxTreeView* muxTreeView;
   			Gtk::VBox muxBox;;
			Gtk::ScrolledWindow scrolledWindow;

			Gtk::Frame serverFrame;
			Gtk::CheckButton* repeat;
			Gtk::Label* destLabel;
			Gtk::Entry destEntry;
			Gtk::Label* portLabel;
			Gtk::Entry portEntry;
			Gtk::Button startServerButton;
			Gtk::Button stopServerButton;
			Gtk::ComboBoxText protocolsCombo;
			Gtk::HPaned* pane;
			bool sending;

   		public:
   			MuxPage();
   			virtual ~MuxPage();
   			Gtk::VBox* getPageBox();
   			MuxTreeView* getMuxTreeView();
   			void onMenuFileSaveAs();
   			void onMenuFileAdd();
   			void onMenuFileRemove();

		private:
			void on_startServerButton_clicked();
			void on_stopServerButton_clicked();
			void on_combo_changed();
			void run();
   	};
}
}
}
}

#endif /*MUXPAGE_H_*/
