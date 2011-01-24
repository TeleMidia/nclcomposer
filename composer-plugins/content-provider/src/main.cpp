#include "../include/gui/ContentManagerWindow.h"
using namespace org::isdtv::contentProvider::gui;

#include "../include/server/MulticastServer.h"
using namespace org::isdtv::network::server;

int main(int argc, char* argv[]) {
	/*MulticastServer* m;
	m = new MulticastServer("224.10.10.2", 40000);
	if (m->createSocket()) {
		while (true) {
			if (m->send("teste12345", 10) < 0) {
				break;
			}
		}
	}
	cout << "Server done" << endl;*/

	Gtk::Main kit(argc, argv);

	ContentManagerWindow contentManager;

	Gtk::Main::run(contentManager);

	cout << "Window closed" << endl;
	return 0;
}
