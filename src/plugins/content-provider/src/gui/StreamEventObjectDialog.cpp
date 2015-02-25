#include "../../include/gui/StreamEventObjectDialog.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace gui {
	StreamEventObjectDialog::StreamEventObjectDialog() {
		initializeGUI();
	}
	
	StreamEventObjectDialog::StreamEventObjectDialog(int serviceDomain) {
		initializeGUI();
		std::stringstream str ;
		str<< std::hex << std::uppercase << serviceDomain;
		serviceDomainEntry.set_text(Glib::filename_to_utf8(str.str()));
#ifdef GLIBMM_PROPERTIES_ENABLED
		serviceDomainEntry.property_editable() = false;
#else
		serviceDomainEntry.set_property("editable", false);
#endif
	}
	
	void StreamEventObjectDialog::initializeGUI(){
		Gtk::Table* table = manage(new Gtk::Table(3,2));
		table->set_row_spacings(6);
		table->set_col_spacings(2);
		table->set_border_width(2);
		Gtk::Label* serviceDomainLabel = manage(new Gtk::Label(_("Service Domain: ")));
		Gtk::Label* eventIDLabel = manage(new Gtk::Label(_("Event ID: ")));
		Gtk::Label* eventNameLabel = manage(new Gtk::Label(_("Event name: ")));
		table->attach(*serviceDomainLabel,0,1,0,1);
		table->attach(serviceDomainEntry,1,2,0,1);
		table->attach(*eventIDLabel,0,1,1,2);
		table->attach(eventIDEntry,1,2,1,2);
		table->attach(*eventNameLabel,0,1,2,3);
		table->attach(eventNameEntry,1,2,2,3);
		get_vbox()->pack_start(*table);
		add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
		add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		
		show_all_children(true);
	}
	
	StreamEventObjectDialog::~StreamEventObjectDialog() {
	}
	int StreamEventObjectDialog::convertFromString(Glib::ustring* value){
		std::stringstream str1(value->raw());
		int intValue;
		bool success=false;
		if(value->find("0x") != Glib::ustring::npos){
			success = (str1 >> std::hex >> intValue);
			if(!success)
				intValue = -1; 
		}else{
			success = str1 >> intValue;
			if(!success)
				intValue = -1;
		}
		return intValue;
	}
	
	StreamEventObject_Type StreamEventObjectDialog::getStreamEventObject() {
		StreamEventObject_Type steo;
		Glib::ustring* tempStr = new Glib::ustring(eventIDEntry.get_text().raw());
		steo.eventID =  convertFromString(tempStr);
		delete tempStr;
		steo.eventName = (std::string)(eventNameEntry.get_text().raw());
		tempStr = new Glib::ustring(serviceDomainEntry.get_text().raw());
		steo.serviceDomain = convertFromString(tempStr);
		delete tempStr;
		return steo;
	}
   }
  }
 }
}
