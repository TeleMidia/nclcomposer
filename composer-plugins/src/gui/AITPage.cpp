#include "../../include/gui/AITPage.h"


namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
	AITPage::AITPage() {
		aitTree = manage(new AITTreeView());
		editionFrame = manage(new AITEditionFrame(aitTree));
		
		Gtk::HPaned* pane = manage(new Gtk::HPaned());
		pane->set_position(925);

		Gtk::ScrolledWindow* treeScrollWindow = manage(new Gtk::ScrolledWindow());
		treeScrollWindow->add(*aitTree);
		treeScrollWindow->set_policy(
			    Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
		treeScrollWindow->set_border_width(5);;
		
		pane->add1(*treeScrollWindow);
		pane->add2(*editionFrame);
  		
  		aitBox.pack_start(*pane);
	}
	
	AITPage::~AITPage() {
	}
	
	Gtk::VBox* AITPage::getPageBox(){
		return &aitBox;
	}
   }
  }
 }
}
