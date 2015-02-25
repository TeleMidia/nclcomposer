#ifndef DSMCCSTREAMEVENTFRAME_H_
#define DSMCCSTREAMEVENTFRAME_H_

#include <gtkmm.h>
#include <glibmm/i18n.h>
#include <sstream>
#include <iostream>

#include "DSMCCSETreeView.h"

#include "../functions.h"
using namespace ::org::isdtv::util;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	class DSMCCStreamEventFrame : public Gtk::Frame {
		private:
			DSMCCSETreeView* streamEventTree;
			Gtk::Entry nptEntry;

			Gtk::Label* eventIdLabel;
			Gtk::Entry eventIdEntry;

			Gtk::Label* otherPrivateDataLabel;
			Gtk::Entry otherPrivateData;

		public:
			DSMCCStreamEventFrame(DSMCCSETreeView* tree);
			virtual ~DSMCCStreamEventFrame();
			void clearSEFields();
			void createStreamEvent();
			virtual void onComboChanged();
			void setEventId(int eventId);
			string getCurrentPrivateData();
			void setPrivateData(string data);
			void setIdAndPrivateDataAsEditable(bool editable);
	};
}
}
}
}

#endif /*DSMCCSTREAMEVENTFRAME_H_*/
