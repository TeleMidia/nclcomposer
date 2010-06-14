#include "../../include/gui/ProgressDialog.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
	ProgressDialog::ProgressDialog(Glib::ustring* title, Gtk::Window& parent)
	:	Gtk::Dialog(*title,parent,true), 
		status(_("Starting...")), 
		vBox(false,5), alignment(0.5,0.5,0,0) {
		
		//set_default_size(800, 640);
		
		set_border_width(3);
		set_transient_for(parent);
		set_title(*title);
		set_modal(true);
		set_resizable(false);
		property_destroy_with_parent() = true;
		property_allow_grow() = false;
		
		add(vBox);
		
		vBox.set_border_width(5);
		vBox.pack_start(status, Gtk::PACK_SHRINK);
		
		vBox.pack_start(separator, Gtk::PACK_SHRINK);
		vBox.pack_start(alignment, Gtk::PACK_SHRINK, 5);
		alignment.add(progressBar);
		
		show_all_children();
	}

	ProgressDialog::~ProgressDialog() {
	}
	
	void ProgressDialog::setStatusText(Glib::ustring* statusText) {
		status.set_text(*statusText);
	}
	
	void ProgressDialog::pulse(){
		progressBar.pulse();
	}
	
	void ProgressDialog::setFraction(double fraction){
		progressBar.set_fraction(fraction);
	}
   }
  }
 }
}
