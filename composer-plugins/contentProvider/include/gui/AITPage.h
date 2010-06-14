#ifndef AITPAGE_H_
#define AITPAGE_H_

#include <gtkmm.h>
#include "AITTreeView.h"
#include "AITEditionFrame.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
	class AITPage {
		private:
			AITTreeView* aitTree;
			AITEditionFrame* editionFrame;
			
			Gtk::VBox aitBox;		
		public:
			AITPage();
			virtual ~AITPage();
			
			Gtk::VBox* getPageBox();
	};
   }
  }
 }
}

#endif /*AITPAGE_H_*/
