#include "../../include/gui/DSMCCStreamEventFrame.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	DSMCCStreamEventFrame::DSMCCStreamEventFrame(DSMCCSETreeView* tree) {
		streamEventTree = tree;

		//set_size_request(get_width(),-1);
		set_label(_("New Stream Event"));
  		set_shadow_type(Gtk::SHADOW_ETCHED_OUT);

		Gtk::VBox* vBox = new Gtk::VBox(false, 1);
		add(*vBox);

		Gtk::Table* table = manage(new Gtk::Table(2,3));
		vBox->pack_start(*table,Gtk::PACK_SHRINK,2);
		table->set_row_spacings(4);
		table->set_col_spacings(2);
		table->set_border_width(2);

		eventIdLabel = manage(new Gtk::Label(_("Event ID: ")));
		eventIdLabel->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*eventIdLabel,0,1,0,1);
		table->attach(eventIdEntry,1,2,0,1);

		Gtk::Label* label = manage(new Gtk::Label(_("Time Ref: ")));
		label->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*label,0,1,2,3);
		table->attach(nptEntry,1,2,2,3);
		nptEntry.set_text("0");

		otherPrivateDataLabel = manage(new Gtk::Label(_("Private Data: ")));
		otherPrivateDataLabel->set_justify(Gtk::JUSTIFY_LEFT);
		table->attach(*otherPrivateDataLabel,0,1,3,4);
		table->attach(otherPrivateData,1,2,3,4);
	}

	DSMCCStreamEventFrame::~DSMCCStreamEventFrame() {
		
	}

	/*void DSMCCStreamEventFrame::onBrowseButtonClick(){
		Gtk::FileChooserDialog dialog(_("Please choose a file"),
				Gtk::FILE_CHOOSER_ACTION_OPEN);
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
		
		if(dialog.run() == Gtk::RESPONSE_OK){
			privateDataFile.set_text(dialog.get_filename());
		}
	}*/

	void DSMCCStreamEventFrame::clearSEFields() {
		eventIdEntry.set_text("");
		nptEntry.set_text("0");
		otherPrivateData.set_text("");
	}

	void DSMCCStreamEventFrame::onComboChanged() {
		
	}

	void DSMCCStreamEventFrame::createStreamEvent() {
		//DEVIA ADICIONAR AO MODELO E O MODELO ACIONAR OS OBSERVADORES (ARVORE)
		//O MODELO TESTA os parametros...

		std::string strEventId;
		unsigned int eventId;
		Gtk::Window* parentWindow;

		parentWindow = dynamic_cast<Gtk::Window*>(get_toplevel());
		strEventId = eventIdEntry.get_text();
		eventId = strHexaToInt(strEventId);
		if (eventId < 0) {
			Gtk::MessageDialog dialog(
					*parentWindow,
					_("The EVENT ID is not valid! Entered eventId is ") +
						strEventId,
					false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK,true);

			dialog.run();
			return;
		}

		if (eventId < 0 || eventId > 255) {
			Gtk::MessageDialog dialog(
					*parentWindow,
					_("The EVENT ID limited exceeded!"),
					false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK,true);

			dialog.set_secondary_text(_(
					"It should be an integer between \"0\" and \"255\"."));

			dialog.run();
			return;
		}

		std::string strNpt;
		unsigned int npt;

		strNpt = nptEntry.get_text();
		npt = strHexaToInt(strNpt);
		cout << "strNpt = '" << strNpt << "'" << endl;
		if (npt < 0) {
			Gtk::MessageDialog dialog(
					_("The NPT is not valid!"), false,
					Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK,false);

			dialog.set_secondary_text(
					_("It should be an integer greater then \"0\"."));

			dialog.run();
			return;
		}

		streamEventTree->addNewEvent(
				eventId, npt, otherPrivateData.get_text().raw());
	}

	void DSMCCStreamEventFrame::setEventId(int eventId) {
		eventIdEntry.set_text(intToStrHexa(eventId));
	}

	string DSMCCStreamEventFrame::getCurrentPrivateData() {
		return otherPrivateData.get_text();
	}

	void DSMCCStreamEventFrame::setPrivateData(string data) {
		otherPrivateData.set_text(data);
	}

	void DSMCCStreamEventFrame::setIdAndPrivateDataAsEditable(bool editable) {
		nptEntry.set_text("0");

		eventIdEntry.set_text("");
		eventIdEntry.set_editable(editable);
		eventIdEntry.set_no_show_all(editable);
		eventIdLabel->set_no_show_all(editable);

		otherPrivateData.set_text("");
		otherPrivateData.set_editable(editable);
		otherPrivateData.set_no_show_all(editable);
		otherPrivateDataLabel->set_no_show_all(editable);

		if (editable) {
			eventIdLabel->show_now();
			eventIdEntry.show_now();
			otherPrivateDataLabel->show_now();
			otherPrivateData.show_now();

		} else {
			eventIdLabel->hide();
			eventIdEntry.hide();
			otherPrivateDataLabel->hide();
			otherPrivateData.hide();
		}
	}
}
}
}
}
