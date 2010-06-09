#include "../../include/gui/ResynchPage.h"

using namespace org::isdtv::contentProvider::mpeg::resynch;

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
   ResynchPage::ResynchPage() {

		Gtk::VPaned* pane = manage(new Gtk::VPaned());
		pane->set_position(500);

		tsIdChkBox = manage(new Gtk::CheckButton("Keep original TSID"));
		pIdsChkBox = manage(new Gtk::CheckButton("Keep original PIDs"));
		versionChkBox = manage(new Gtk::CheckButton("Increment version number"));
		originalChkBox = manage(new Gtk::CheckButton("Use all originals (it's not a copy) *Continuity counter still working! )"));
		input1Label = manage(new Gtk::Label("Input 1:"));
		input2Label = manage(new Gtk::Label("Input 2:"));
		outputLabel = manage(new Gtk::Label("Output:"));

		resynchButton.set_size_request(100,30);
		resynchButton.set_label("Resynch");
		resynchButton.signal_clicked().connect(sigc::mem_fun(*this,
							&ResynchPage::on_resynchButton_clicked));

		Gtk::HBox* hBox1 = manage(new Gtk::HBox());
		hBox1->pack_start(*input1Label, Gtk::PACK_SHRINK, 0);
		hBox1->pack_start(input1Entry, Gtk::PACK_SHRINK, 0);

		Gtk::HBox* hBox2 = manage(new Gtk::HBox());
		hBox2->pack_start(*input2Label, Gtk::PACK_SHRINK, 0);
		hBox2->pack_start(input2Entry, Gtk::PACK_SHRINK, 0);

		Gtk::HBox* hBox3 = manage(new Gtk::HBox());
		hBox3->pack_start(*outputLabel, Gtk::PACK_SHRINK, 0);
		hBox3->pack_start(outputEntry, Gtk::PACK_SHRINK, 0);

		Gtk::VBox* vBox = manage(new Gtk::VBox());
		vBox->pack_start(*originalChkBox, Gtk::PACK_SHRINK, 0);
		vBox->pack_start(*tsIdChkBox, Gtk::PACK_SHRINK, 0);
		vBox->pack_start(*pIdsChkBox, Gtk::PACK_SHRINK, 0);
		vBox->pack_start(*versionChkBox, Gtk::PACK_SHRINK, 0);
		vBox->pack_start(*hBox1, Gtk::PACK_SHRINK, 0);
		vBox->pack_start(*hBox2, Gtk::PACK_SHRINK, 0);
		vBox->pack_start(*hBox3, Gtk::PACK_SHRINK, 0);

		Gtk::HBox* hBox = manage(new Gtk::HBox(false, 0));
		hBox->pack_start(resynchButton, Gtk::PACK_SHRINK, 0);

		resynchBox.pack_start(*vBox, Gtk::PACK_SHRINK, 0);
		resynchBox.pack_start(*hBox, Gtk::PACK_SHRINK, 0);

		resynchBox.pack_start(*pane);
	}

   ResynchPage::~ResynchPage() {
	}

	Gtk::VBox* ResynchPage::getPageBox(){
		return &resynchBox;
	}

	void ResynchPage::on_resynchButton_clicked() {
		Joiner j;
		j.addFile(input1Entry.get_text());
		j.addFile(input2Entry.get_text());
		j.setKeepTsId(tsIdChkBox->get_active());
		j.setKeepPId(pIdsChkBox->get_active());
		j.setIncrementVersionNumber(versionChkBox->get_active());
		j.setOriginal(originalChkBox->get_active());
		if (j.joinFiles(outputEntry.get_text()) == 0) {
			cout << "Operation completed." << endl;
		}
	}
   }
  }
 }
}
