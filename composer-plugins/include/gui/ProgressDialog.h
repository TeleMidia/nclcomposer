#ifndef PROGRESSDIALOG_H_
#define PROGRESSDIALOG_H_

#include <gtkmm.h>
#include <glibmm/i18n.h>

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
	class ProgressDialog : public Gtk::Dialog {
		private:
			Gtk::ProgressBar progressBar;
			Gtk::HSeparator	 separator;
			Gtk::Label		 status;
			
			Gtk::VBox 		 vBox;
			Gtk::Alignment	 alignment;
		public:
			ProgressDialog(Glib::ustring* title, Gtk::Window& parent);
			virtual ~ProgressDialog();
			void setStatusText(Glib::ustring* statusText);
			
			void pulse();
			void setFraction(double fraction);
	};
   }
  }
 }
}

#endif /*PROGRESSDIALOG_H_*/
