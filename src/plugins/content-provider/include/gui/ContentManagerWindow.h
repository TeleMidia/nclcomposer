#ifndef CONTENTMANAGER_H_
#define CONTENTMANAGER_H_

#include <gtkmm.h>
#include <glibmm/i18n.h>
#include "../mpeg/Muxer.h"
using namespace org::isdtv::contentProvider::mpeg;

#include "MuxPage.h"
#include "DSMCCPage.h"
#include "AITPage.h"
#include "EPGPage.h"
#include "ResynchPage.h"

#include <iostream>
#include <string>
using namespace std;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	class ContentManagerWindow : public Gtk::Window {
		private:
			MuxPage* muxPage;
			DSMCCPage* dsmccPage;
			AITPage* aitPage;
			EPGPage* epgPage;
			ResynchPage* resynchPage;

			//widgets
			Gtk::VBox bigBox;
			Gtk::Notebook notes;

			Gtk::Label labelTables;
			Gtk::Label labelEpg;
			Gtk::Label labelDsmcc;
			Gtk::HButtonBox buttonBox;
			Gtk::Button quitButton;

			//menu widgets
			Gtk::VBox menuBox;
			Glib::RefPtr<Gtk::UIManager> refUIManager;
			Glib::RefPtr<Gtk::ActionGroup> refActionGroup;
			Glib::RefPtr<Gtk::RadioAction> refChoiceOne, refChoiceTwo;

		public:
			ContentManagerWindow();
			virtual ~ContentManagerWindow();

		protected:
			void initializeWindowSettings();
			void initializeMenuSettings();
			void initializeNotesSettings();

			//menu event handlers
			void onMenuFileNewGeneric();
			void onMenuFileSaveAs();
			void onMenuFileAdd();
			void onMenuFileRemove();
			void onMenuFileQuit();
			void onMenuOthers();
			void onMenuChoicesOne();
			void onMenuChoicesTwo();

			//Note change handler
			void onSwitchNotebookPage(
					GtkNotebookPage* page, guint page_num);
	};
}
}
}
}

#endif /*CONTENTMANAGER_H_*/
