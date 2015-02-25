#include "../../include/gui/ContentManagerWindow.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	ContentManagerWindow::ContentManagerWindow() :
		    labelTables("PSIs e SIs"),
		    labelEpg("EPG"),
		    labelDsmcc("DSM-CC"),
		    quitButton(Gtk::Stock::QUIT) {

		muxPage = new MuxPage();
		dsmccPage = new DSMCCPage(muxPage->getMuxTreeView());
		aitPage = new AITPage();
		epgPage = new EPGPage();
		resynchPage = new ResynchPage();

		initializeWindowSettings();
		initializeMenuSettings();
		initializeNotesSettings();

		show_all_children(true);
	}

	ContentManagerWindow::~ContentManagerWindow() {

	}

	void ContentManagerWindow::initializeWindowSettings() {
		set_title("Cousins' Content Provider");
		set_border_width(3);
		set_default_size(1100, 750);
		maximize();

		add(bigBox);

		bigBox.pack_start(menuBox, Gtk::PACK_SHRINK);
		bigBox.pack_start(notes);
	}

	void ContentManagerWindow::initializeMenuSettings() {
		//Create actions for menus and toolbars:
		refActionGroup = Gtk::ActionGroup::create();

		//File menu:
		refActionGroup->add(Gtk::Action::create("FileMenu", _("_File")));
		refActionGroup->add(Gtk::Action::create("FileNew", Gtk::Stock::NEW));
	    refActionGroup->add(Gtk::Action::create(
	    		"FileSaveAs", Gtk::Stock::SAVE_AS),
	    		sigc::mem_fun(*this, &ContentManagerWindow::onMenuFileSaveAs));

		refActionGroup->add(Gtk::Action::create(
				"FileAdd", Gtk::Stock::ADD),
				sigc::mem_fun(*this, &ContentManagerWindow::onMenuFileAdd));

	    refActionGroup->add(Gtk::Action::create(
	    		"FileRemove", Gtk::Stock::REMOVE),
	    		sigc::mem_fun(*this, &ContentManagerWindow::onMenuFileRemove));

		refActionGroup->add(Gtk::Action::create(
				"FileQuit", Gtk::Stock::QUIT),
			    sigc::mem_fun(*this, &ContentManagerWindow::onMenuFileQuit));

		//Edit menu:
		refActionGroup->add(Gtk::Action::create("EditMenu", _("_Edit")));
		refActionGroup->add(Gtk::Action::create("EditCopy", Gtk::Stock::COPY),
			    sigc::mem_fun(*this, &ContentManagerWindow::onMenuOthers));

		refActionGroup->add(Gtk::Action::create("EditPaste", Gtk::Stock::PASTE),
			    sigc::mem_fun(*this, &ContentManagerWindow::onMenuOthers));

		refActionGroup->add(Gtk::Action::create("EditSomething", "Something"),
			    Gtk::AccelKey("<control><alt>S"),
			    sigc::mem_fun(*this, &ContentManagerWindow::onMenuOthers));

		//Choices menu, to demonstrate Radio items
		refActionGroup->add(Gtk::Action::create("ChoicesMenu", "Choices"));

		Gtk::RadioAction::Group group_userlevel;
		refChoiceOne = Gtk::RadioAction::create(
			    group_userlevel, "ChoiceOne", "One");

		refActionGroup->add(refChoiceOne,
			    sigc::mem_fun(*this, &ContentManagerWindow::onMenuChoicesOne));

		refChoiceTwo = Gtk::RadioAction::create(
			    group_userlevel, "ChoiceTwo", "Two");

		refActionGroup->add(refChoiceTwo,
			    sigc::mem_fun(*this, &ContentManagerWindow::onMenuChoicesTwo));

		//Help menu:
		refActionGroup->add(Gtk::Action::create("HelpMenu", _("Help")));
		refActionGroup->add(Gtk::Action::create("HelpAbout", Gtk::Stock::HELP),
			    sigc::mem_fun(*this, &ContentManagerWindow::onMenuOthers));

		refUIManager = Gtk::UIManager::create();
		refUIManager->insert_action_group(refActionGroup);

		add_accel_group(refUIManager->get_accel_group());

		//Layout the actions in a menubar and toolbar:

		Glib::ustring uiInfo =
			"<ui>"
			"  <menubar name='MenuBar'>"
			"    <menu action='FileMenu'>"
			"      <menuitem action='FileNew'/>"
			"      <menuitem action='FileSaveAs'/>"
			"      <menuitem action='FileAdd'/>"
			"      <menuitem action='FileRemove'/>"
			"      <menuitem action='FileQuit'/>"
			"    </menu>"
			"    <menu action='EditMenu'>"
			"      <menuitem action='EditCopy'/>"
			"      <menuitem action='EditPaste'/>"
			"      <menuitem action='EditSomething'/>"
			"    </menu>"
			"    <menu action='ChoicesMenu'>"
			"      <menuitem action='ChoiceOne'/>"
			"      <menuitem action='ChoiceTwo'/>"
			"    </menu>"
			"    <menu action='HelpMenu'>"
			"      <menuitem action='HelpAbout'/>"
			"    </menu>"
			"  </menubar>"
			"  <toolbar  name='ToolBar'>"
			"    <toolitem action='FileNew'/>"
			"    <toolitem action='FileSaveAs'/>"
			"    <toolitem action='FileAdd'/>"
			"    <toolitem action='FileRemove'/>"
			"    <toolitem action='FileQuit'/>"
			"  </toolbar>"
			"</ui>";

#ifdef GLIBMM_EXCEPTIONS_ENABLED
		try {
			refUIManager->add_ui_from_string(uiInfo);

		} catch (const Glib::Error& ex) {
			cerr << "building menus failed: " <<  ex.what();
		}

#else
		auto_ptr<Glib::Error> ex;
		refUIManager->add_ui_from_string(uiInfo, ex);
		if (ex.get()) {
			cerr << "building menus failed: " <<  ex->what();
		}

#endif //GLIBMM_EXCEPTIONS_ENABLED

		//Get the menubar and toolbar widgets, and add them to container widget:
		Gtk::Widget* pMenubar = refUIManager->get_widget("/MenuBar");

		if (pMenubar) {
			menuBox.pack_start(*pMenubar, Gtk::PACK_SHRINK);
		}

		Gtk::Widget* pToolbar = refUIManager->get_widget("/ToolBar") ;

		if (pToolbar) {
			menuBox.pack_start(*pToolbar, Gtk::PACK_SHRINK);
		}
	}

	void ContentManagerWindow::initializeNotesSettings() {
		notes.set_border_width(2);

		//Add the Notebook notes:
		notes.append_page(*(muxPage->getPageBox()), _("Muxer"));
		notes.append_page(labelTables, _("PSI/SI Tables"));
		//notes.append_page(labelEpg, _("EPG"));
		notes.append_page(*(epgPage->getPageBox()), "EPG");
		//notes.append_page(labelResynch, _("Resynch"));
				notes.append_page(*(resynchPage->getPageBox()), "Resynch");
		//notes.append_page(labelDsmcc, "DSM-CC");
		notes.append_page(*(dsmccPage->getPageBox()), "DSM-CC");
		//Gtk::Label* aitLabel = manage(new Gtk::Label("AIT"));
		notes.append_page(*(aitPage->getPageBox()), "AIT");
		notes.signal_switch_page().connect(sigc::mem_fun(
				*this,&ContentManagerWindow::onSwitchNotebookPage));
	}

	void ContentManagerWindow::onMenuFileSaveAs() {
		if (notes.get_current_page() == 0) {
			muxPage->onMenuFileSaveAs();
		}
	}

	void ContentManagerWindow::onMenuFileAdd() {
		if (notes.get_current_page() == 0) {
			muxPage->onMenuFileAdd();

		} else if (notes.get_current_page() == 3) {
			dsmccPage->add();
		}
	}

	void ContentManagerWindow::onMenuFileRemove() {
		if (notes.get_current_page() == 0) {
			muxPage->onMenuFileRemove();
		}
	}

	void ContentManagerWindow::onMenuFileQuit() {
		hide(); //Closes the main window to stop the Gtk::Main::run().
	}

	void ContentManagerWindow::onMenuFileNewGeneric() {
		std::cout << "A File|New menu item was selected." << std::endl;
	}

	void ContentManagerWindow::onMenuOthers() {
		std::cout << "A menu item was selected." << std::endl;
	}

	void ContentManagerWindow::onMenuChoicesOne() {
		Glib::ustring message;
		if (refChoiceOne->get_active()) {
			message = "Choice 1 was selected.";

		} else {
			message = "Choice 1 was deselected";
		}

		std::cout << message << std::endl;
	}

	void ContentManagerWindow::onMenuChoicesTwo() {
		Glib::ustring message;
		if (refChoiceTwo->get_active()) {
			message = "Choice 2 was selected.";

		} else {
			message = "Choice 2 was deselected";
		}

		std::cout << message << std::endl;
	}

	void ContentManagerWindow::onSwitchNotebookPage(
			GtkNotebookPage* page, guint page_num) {

		if (page_num==2) {
			epgPage->updateView();
		}

		if (page_num==3) {
			dsmccPage->updateView();
		}

	}
}
}
}
}
