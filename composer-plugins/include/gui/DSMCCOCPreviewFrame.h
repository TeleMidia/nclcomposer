#ifndef DSMCCOCPREVIEWFRAME_H_
#define DSMCCOCPREVIEWFRAME_H_

#include <gtkmm.h>
#include <gtkmm/treemodel.h>
#include <glibmm/i18n.h>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "DSMCCOCPreviewModelColumn.h"
//#include "DSMCCOCInfoFrame.h"
#include "ProgramNumberComboBoxModel.h"

#include "../functions.h"
using namespace org::isdtv::util;

#include "../mpeg/Muxer.h"
using namespace org::isdtv::contentProvider::mpeg;

#include "../mpeg/si/Pat.h"
#include "../mpeg/si/Pmt.h"
using namespace org::isdtv::contentProvider::mpeg::si;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	class DSMCCOCPreviewFrame : public Gtk::Frame {
		private:
			Muxer* muxer;

			Gtk::TreeView carouselTree;

			Gtk::Label* nrTS;
			Gtk::Label* szTS;

			Gtk::Label* nrSess;
			Gtk::Label* szSess;

			Gtk::Label* nrMod;
			Gtk::Label* szMod;

			Gtk::Frame infoFrame;
		    Gtk::ScrolledWindow scrolledWindow;

		    Glib::RefPtr<Gtk::ListStore> refTreeModel;
		    Glib::RefPtr<Gtk::ListStore> refComboBoxModel;

		    ProgramNumberComboBoxModel comboBoxModel;

		    //DSMCCOCInfoFrame* infoFrameModel;
		    DSMCCOCPreviewModelColumn columnModel;

		public:
			DSMCCOCPreviewFrame();
			virtual ~DSMCCOCPreviewFrame();
/*
		private:
		    void createInfoFrame();

		public:
			virtual void updateInfo();
			DSMCCOCInfoFrameModel* getModel();
			*/
			void clearCarousels();
			std::vector<CarouselInfo*>* getCarouselsInfo();
			void addNewCarousel(
					unsigned int pid,
					service_domain_type serviceDomain,
					unsigned int componentTag);

			void removeCarousel(service_domain_type serviceDomain);
			void onEditingProgramNumberCell(
					const Glib::ustring& path_string,
					const Glib::ustring& new_text);

			void updateProgramNumbers();
	};
}
}
}
}

#endif /*DSMCCOCPREVIEWFRAME_H_*/
