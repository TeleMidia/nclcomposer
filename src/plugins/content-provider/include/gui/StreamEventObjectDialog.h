#ifndef STREAMEVENTOBJECTDIALOG_H_
#define STREAMEVENTOBJECTDIALOG_H_

#include <gtkmm.h>
#include <glibmm/i18n.h>
#include <string>
#include <sstream>
#include <iostream>

namespace org {
namespace isdtv {
namespace contentProvider {
namespace gui {

	typedef struct {
		int eventID;
		std::string	eventName;
		int serviceDomain;
   	} StreamEventObject_Type;

	class StreamEventObjectDialog : public Gtk::Dialog {
		private:
			Gtk::Entry eventNameEntry;
			Gtk::Entry eventIDEntry;
			Gtk::Entry serviceDomainEntry;

			void initializeGUI();
			int convertFromString(Glib::ustring* value);

		public:
			StreamEventObjectDialog(int serviceDomain);
			StreamEventObjectDialog();
			virtual ~StreamEventObjectDialog();
			StreamEventObject_Type getStreamEventObject();
	};
   }
  }
 }
}

#endif /*STREAMEVENTOBJECTDIALOG_H_*/
