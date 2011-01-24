#include "../../include/gui/MuxPage.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {
	MuxPage::MuxPage() {

		//LEFT
		//Add the TreeView, inside a ScrolledWindow
		muxTreeView = new MuxTreeView();
		scrolledWindow.add(*muxTreeView);

		//Only show the scrollbars when they are necessary:
		scrolledWindow.set_policy(
			    Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

		scrolledWindow.set_border_width(5);

		//RIGHT
		serverFrame.set_label("Transport Stream Server");
		serverFrame.set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
		serverFrame.set_border_width(2);

		repeat = manage(new Gtk::CheckButton("Send TS in Loop"));

		Gtk::Table* table = manage(new Gtk::Table(2,4));
		table->set_row_spacings(4);
		table->set_col_spacings(2);
		table->set_border_width(4);

		table->attach(*repeat,0,1,0,1);

		Gtk::Label* label = manage(new Gtk::Label("Protocol: "));
		Gtk::HBox* hBox = manage(new Gtk::HBox());
		hBox->pack_start(*label, Gtk::PACK_SHRINK, 0);
		hBox->pack_start(protocolsCombo, Gtk::PACK_SHRINK, 0);
		table->attach(*hBox,0,1,1,2);

		portEntry.set_width_chars(5);
		destEntry.set_width_chars(19);
		destLabel = manage(new Gtk::Label("Destination: "));
		Gtk::HBox* hBox2 = manage(new Gtk::HBox());
		hBox2->pack_start(*destLabel, Gtk::PACK_SHRINK, 0);
		hBox2->pack_start(destEntry, Gtk::PACK_SHRINK, 0);
		portLabel = manage(new Gtk::Label("Port: "));
		Gtk::HBox* hBox3 = manage(new Gtk::HBox());
		hBox3->pack_start(*portLabel, Gtk::PACK_SHRINK, 0);
		hBox3->pack_start(portEntry, Gtk::PACK_SHRINK, 0);
		table->attach(*hBox2,0,1,2,3);
		table->attach(*hBox3,1,2,2,3);

		Gtk::HBox* hBox4 = manage(new Gtk::HBox());
		hBox4->pack_start(startServerButton, Gtk::PACK_SHRINK, 0);
		hBox4->pack_start(stopServerButton, Gtk::PACK_SHRINK, 0);
		table->attach(*hBox4,0,1,3,4);
		startServerButton.signal_clicked().connect(
				sigc::mem_fun(*this, &MuxPage::on_startServerButton_clicked));

		stopServerButton.signal_clicked().connect(
				sigc::mem_fun(*this, &MuxPage::on_stopServerButton_clicked));

		Gtk::VBox* vBox = manage(new Gtk::VBox());
		vBox->pack_start(*table, Gtk::PACK_SHRINK, 0);
		serverFrame.add(*vBox);

		Gtk::HBox* hBox5 = manage(new Gtk::HBox());
		hBox5->pack_start(serverFrame, Gtk::PACK_SHRINK, 0);

		pane = manage(new Gtk::HPaned());
		pane->set_position(970);
		pane->add1(scrolledWindow);
		pane->add2(*hBox5);

		muxBox.pack_start(*pane);

		protocolsCombo.append_text("UDP Unicast");
		protocolsCombo.append_text("UDP Multicast");
		protocolsCombo.append_text("RTP Multicast");
		protocolsCombo.append_text("Bluetooth Broadcast");
		protocolsCombo.append_text("DTA-102 Broadcast");
		protocolsCombo.append_text("DTA-160 Broadcast");

		protocolsCombo.signal_changed().connect(
				sigc::mem_fun(*this, &MuxPage::on_combo_changed));

		sending = false;

		stopServerButton.hide_all();
		stopServerButton.set_no_show_all(true);

		startServerButton.set_label("start");
		stopServerButton.set_label("stop");

		startServerButton.set_size_request(100, 30);
		stopServerButton.set_size_request(100, 30);
	}

	MuxPage::~MuxPage() {

	}

	void MuxPage::onMenuFileSaveAs() {
		Gtk::Window* parentWindow;

		if (sending) {
			parentWindow = dynamic_cast<Gtk::Window*>(
					muxTreeView->get_toplevel());

			Gtk::MessageDialog dialog(*parentWindow,
					"You must stop the server first",
					false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);

			dialog.run();
			return;
		}

		muxTreeView->onMenuFileSaveAs();
	}

	void MuxPage::onMenuFileAdd() {
		muxTreeView->onAddButtonClick();
	}

	void MuxPage::onMenuFileRemove() {
		muxTreeView->onRemoveButtonClick();
	}

	Gtk::VBox* MuxPage::getPageBox() {
		return &muxBox;
	}

	MuxTreeView* MuxPage::getMuxTreeView() {
		return muxTreeView;
	}

	void MuxPage::on_startServerButton_clicked() {
		Gtk::Window* parentWindow;

		if (sending) {
			parentWindow = dynamic_cast<Gtk::Window*>(
					muxTreeView->get_toplevel());

			Gtk::MessageDialog dialog(*parentWindow,
					"Server is already sending the ts file",
					false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);

			dialog.run();
			return;
		}

		startServerButton.hide_all();
		startServerButton.set_no_show_all(true);

		stopServerButton.set_no_show_all(false);
		stopServerButton.show_now();
		stopServerButton.set_size_request(100, 30);

		startServerButton.set_label("start");
		stopServerButton.set_label("stop");

		sending = true;
		Thread::start();
	}

	void MuxPage::on_stopServerButton_clicked() {
		sending = false;
		stopServerButton.hide_all();
		stopServerButton.set_no_show_all(true);
		startServerButton.set_no_show_all(false);
		startServerButton.show_now();
		startServerButton.set_size_request(100, 30);

		startServerButton.set_label("start");
		stopServerButton.set_label("stop");
	}

	void MuxPage::on_combo_changed() {
		string text;

		portLabel->set_no_show_all(false);
		portLabel->show_now();
		portEntry.set_no_show_all(false);
		portEntry.show_now();

		destLabel->set_no_show_all(false);
		destLabel->show_now();
		destEntry.set_no_show_all(false);
		destEntry.show_now();

		text = protocolsCombo.get_active_text();
		if (text == "UDP Unicast") {
			portLabel->hide_all();
			portLabel->set_no_show_all(true);
			portEntry.hide_all();
			portEntry.set_no_show_all(true);

			destLabel->hide_all();
			destLabel->set_no_show_all(true);
			destEntry.hide_all();
			destEntry.set_no_show_all(true);
			pane->set_position(pane->get_position() + 65);

		} else if (text == "UDP Multicast") {
			destEntry.set_text("224.10.10.2");
			portEntry.set_text("40000");
		}
	}

	void MuxPage::run() {
		SocketServer* server = NULL;
		int fdin, portNumber;
		char* groupAddr;
		string activeText;

		activeText = protocolsCombo.get_active_text();
		portNumber = (int)stof(portEntry.get_text());
		groupAddr = (char*)(((string)(destEntry.get_text())).c_str());

		cout << "MuxPage starting... ";
		if (activeText == "UDP Unicast") {
			cout << "unicast server" << endl;
			server = new UnicastServer(9000);

		} else if (activeText == "UDP Multicast") {
			cout << "multicast server" << endl;
			server = new MulticastServer(groupAddr, portNumber);
		}

		if (server != NULL && server->createSocket()) {
			do {
				cout << "MuxPage::run opening ts file." << endl;
#ifdef WIN32
				// Open media file to serve
				fdin = open((char*)
						Muxer::getInstance()->getTSFileName().c_str(),
						O_RDONLY | O_BINARY, 0644);
#else
				// Open media file to serve
				fdin = open((char*)
						Muxer::getInstance()->getTSFileName().c_str(),
						O_RDONLY, 0644);
#endif

				if (fdin <= 0) {
					perror("MuxPage::run open ts error");
					delete server;
					server = NULL;
					sending = false;
					return;
				}

				Muxer::getInstance()->muxToSock(fdin, server);
				cout << "MuxPage::run muxer done! closing ts file." << endl;
				close(fdin);

			} while (repeat->get_active() && sending);

		} else {
			cout << "create socket failed" << endl;
		}

		if (server != NULL) {
			delete server;
			server = NULL;
		}
		sending = false;
		on_stopServerButton_clicked();
	}
}
}
}
}
