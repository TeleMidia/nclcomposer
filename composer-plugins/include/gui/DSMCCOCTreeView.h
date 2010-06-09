#ifndef DSMCCOCTREEVIEW_H_
#define DSMCCOCTREEVIEW_H_

#include <gtkmm.h>
#include <gtkmm/treemodel.h>
#include <glibmm/i18n.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <glib.h>
#include <glib/gstdio.h>

#ifndef WIN_32
//	#include <sys/stat.h>
#endif

#include <map>
#include <list>
#include <string>
using namespace std;

#include "DSMCCOCModelColumn.h"
#include "DSMCCOCPreviewFrame.h"
#include "StreamEventObjectDialog.h"
#include "../model/ElementaryStream.h"
#include "../model/ElementaryStreamsSet.h"
#include "../model/DSMCCCarouselStream.h"
#include "../mpeg/dsmcc/OCStreamFactory.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	class DSMCCOCTreeView : public Gtk::TreeView {
		private:
			DSMCCOCPreviewFrame* previewFrame;

			const static Glib::ustring FILE_TYPE;
			const static Glib::ustring DIRECTORY_TYPE;
			const static Glib::ustring SERVICE_GATEWAY_TYPE;
			const static Glib::ustring STREAM_EVENT_TYPE;

			Glib::RefPtr<Gtk::TreeStore> refTreeModel;
			DSMCCOCModelColumn columnModel;
			Gtk::Menu popupMenu;

			model::ElementaryStreamsSet* model;
			std::map<std::string, ObjectInfo*>* currentObjectsIor;

		public:
			DSMCCOCTreeView();
			DSMCCOCTreeView(DSMCCOCPreviewFrame* previewFrame);
			virtual ~DSMCCOCTreeView();
			void clearCarousels();

		private:
			short addFileAsChild(
					string fileName, Gtk::TreeModel::Row* parentRow = NULL);

			void removeChildren(string parentFilePath);
			bool isAlreadyChild(string filePath);
			service_domain_type generateServiceDomainID();

		protected:
		    virtual bool on_button_press_event(GdkEventButton* event);

		public:
			void setModel(model::ElementaryStreamsSet* model);
			model::ElementaryStreamsSet* getModel();
			void refresh();

			/**
			 * Adiciona o arquivo ou diretorio como filho da linha
			 * atualmente selecionada.
			 */
			short addNewDirectory(
					string filePath, std::map<string, ObjectInfo*>* iors);

			void setPreviewFrame(DSMCCOCPreviewFrame* previewFrame);

			/**
			 * Abre Janela para adicionar diretório desde a raiz.
			 */
			virtual void onAddNewDirectoryClick();
			virtual void onAddStreamEventClick();
			virtual void removeSelectedNodeFromTree();
			virtual void addStreamEventObject(
					int serviceDomain, int eventId, string eventName);

	};
}
}
}
}

#endif /*DSMCCOCTREEVIEW_H_*/
