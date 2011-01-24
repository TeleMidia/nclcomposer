#include "../../include/gui/DSMCCSETreeView.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	DSMCCSETreeView::DSMCCSETreeView() {
		refTreeModel = Gtk::TreeStore::create(columnModel);
		set_model(refTreeModel);

		//Add the TreeView's view columns:
		//append_column(_("PN"), columnModel.programNumber);
		append_column(_("PID"), columnModel.pid);
		append_column(_("Event Id"), columnModel.eventId);
		append_column(_("NPT"), columnModel.npt);
		append_column(_("Private Data"), columnModel.privateData);

		Gtk::Menu::MenuList& menulist = popupMenu.items();
		menulist.push_back(Gtk::Menu_Helpers::MenuElem(
				_("_Remove"),
				sigc::mem_fun(*this, &DSMCCSETreeView::onRemoveButtonClick)));
	}

	DSMCCSETreeView::~DSMCCSETreeView() {
		
	}

	bool DSMCCSETreeView::on_button_press_event(GdkEventButton* event) {
		TreeView::on_button_press_event(event);
		if ((event->type == GDK_BUTTON_PRESS) && (event->button == 3)) {
			popupMenu.popup(event->button,event->time);
			return true;
		}
		return false;
	}

	void DSMCCSETreeView::onRemoveButtonClick() {
		Glib::RefPtr<Gtk::TreeSelection> refTreeSelection;
		Gtk::TreeIter iter;
		Gtk::TreeRow row;

		refTreeSelection = get_selection();
		iter = refTreeSelection->get_selected();
		refTreeModel->erase(iter);
	}

	void DSMCCSETreeView::addNewEvent(
			unsigned int eventId, unsigned int npt, string privateData) {

		Gtk::TreeIter iter;
		Gtk::TreeRow row;
		Glib::ustring rowStr;
		Gtk::Window* parentWindow;
		std::string componentTag;

		componentTag = "0x21";
		iter = refTreeModel->children().begin();
		while (iter) { //CHECK if the eventID already exists.
			row = *iter;
			rowStr = row[columnModel.eventId];
			if ((unsigned int)strHexaToInt(rowStr) == eventId) {
				parentWindow = dynamic_cast<Gtk::Window*>(get_toplevel());
				Gtk::MessageDialog dialog(
						*parentWindow,
						_("EVENT ID already exists!"), false,
						Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);

				dialog.set_secondary_text(
						_("It must exist only distincts events id."));

				dialog.run();
				return;
			}
			iter++;
		}

		row = *(refTreeModel->append());
		row[columnModel.eventId] = intToStrHexa(eventId);
		row[columnModel.npt] = intToStrHexa(npt);
		row[columnModel.privateData] = privateData;
		row[columnModel.componentTag] = componentTag;

		unsigned int pid, cTag;
		std::string tsFile;

		pid = Muxer::getInstance()->getPat()->getNextPid();
		tsFile = SE_ES_PATH + intToStrHexa(pid) + ".ts";
		cTag = strHexaToInt(componentTag);

		SEDStreamFactory::createSEDStream(
				tsFile, pid, cTag, eventId, npt, privateData);

		row[columnModel.programNumber] = intToStrHexa(Muxer::getInstance()->
				getPat()->getFirstProgramNumber());

		row[columnModel.pid] = intToStrHexa(pid);

		cout << "strEventId = '" << eventId << "' strNpt = '" << npt;
		cout << "' privateData '" << privateData << "'" << endl;
	}

	void DSMCCSETreeView::removeEvent(unsigned int eventId) {
		Gtk::TreeIter iter;
		Glib::ustring rowStr;
		Gtk::TreeRow row;

		iter = refTreeModel->children().begin();
		while (iter) {
			row = *iter;
			rowStr = row[columnModel.eventId];
			if ((unsigned int)strHexaToInt(rowStr) == eventId) {
				refTreeModel->erase(iter);
				return;
			}
			iter++;
		}
	}

	std::vector<SEInfo*>* DSMCCSETreeView::getStreamEventsInfo() {
		Gtk::TreeIter iter;
		Gtk::TreeRow row;
		Glib::ustring programNumber, pid, eventId, componentTag;
		Gtk::Window* parentWindow;
		std::string msg;
		std::vector<SEInfo*>* infos;
		SEInfo* info;

		infos = new std::vector<SEInfo*>;
		iter = refTreeModel->children().begin();
		row = *iter;
		while (iter) {
			programNumber = row[columnModel.programNumber];
			pid = row[columnModel.pid];
			eventId = row[columnModel.eventId];
			componentTag = row[columnModel.componentTag];

			if (!Muxer::getInstance()->getPat()->hasProgramNumber(
					strHexaToInt(programNumber))) {

				msg = "Invalid program number for event Id '" + eventId + "'";

				parentWindow = dynamic_cast<Gtk::Window*>(get_toplevel());
				Gtk::MessageDialog dialog(*parentWindow,
						msg, false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);

				dialog.run();
				std::vector<SEInfo*>::iterator i;
				i = infos->begin();
				while (i != infos->end()) {
					if (*i != NULL) {
						delete *i;
					}
					++i;
				}
				delete infos;
				return NULL;
			}
			info = new SEInfo;
			info->programNumber = strHexaToInt(programNumber);
			info->pid = strHexaToInt(pid);
			info->myId = strHexaToInt(eventId);
			info->componentTag = strHexaToInt(componentTag);
			infos->push_back(info);
			iter++;
		}

		return infos;
	}

	void DSMCCSETreeView::clearStreamEvents() {
		refTreeModel->clear();
	}
}
}
}
}
