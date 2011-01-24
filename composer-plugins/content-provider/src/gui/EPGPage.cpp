#include "../../include/gui/EPGPage.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
	EPGPage::EPGPage() : multiplexButton(_("Multiplex")) {
		epgTree = manage(new EPGTreeView());
		logoTree = manage(new LogoTreeView());
		
		Gtk::VPaned* pane = manage(new Gtk::VPaned());
		pane->set_position(500);

		Gtk::ScrolledWindow* treeScrollWindow = manage(new Gtk::ScrolledWindow());
		treeScrollWindow->add(*epgTree);
		treeScrollWindow->set_policy(
			    Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
		treeScrollWindow->set_border_width(5);
		
		multiplexButton.set_size_request(-1,30);
		
		multiplexButton.signal_clicked().connect(sigc::mem_fun(*this,
							&EPGPage::on_multiplexButton_clicked));

		Gtk::HBox* hBox = manage(new Gtk::HBox(false, 0));
		hBox->pack_start(multiplexButton, Gtk::PACK_SHRINK, 0);
		epgBox.pack_start(*hBox, Gtk::PACK_SHRINK, 0);

		pane->add1(*treeScrollWindow);
		pane->add2(*logoTree);
  		
  		epgBox.pack_start(*pane);
	}
	
	EPGPage::~EPGPage() {
	}

	Gtk::VBox* EPGPage::getPageBox(){
		return &epgBox;
	}

	void EPGPage::updateView(){
		epgTree->updateProgramNumbers();
		logoTree->updateProgramNumbers();
	}

	void EPGPage::on_multiplexButton_clicked() {
		epgTree->encodeEIT();
		logoTree->encodeLogo();

		Muxer::getInstance()->setHasSdt(true);
		Muxer::getInstance()->setHasEit(true);
		Muxer::getInstance()->setHasCdt(true);
	}
   }
  }
 }
}
