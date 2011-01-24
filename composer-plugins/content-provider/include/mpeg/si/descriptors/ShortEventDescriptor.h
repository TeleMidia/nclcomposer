#ifndef SHORTEVENTDESCRIPTOR_H_
#define SHORTEVENTDESCRIPTOR_H_

#include <string>
#include <cstring>
#include "../../Descriptor.h"
using namespace ::org::isdtv::contentProvider::mpeg;

using namespace std;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {

    class ShortEventDescriptor : public Descriptor {
	private:
		
	protected:
		char languageCode[3];
		unsigned char eventNameLength;
		char eventName[255];
		unsigned char descriptionLength;
		char description[255];
	public:
		ShortEventDescriptor();
		~ShortEventDescriptor();
		void setLanguageCode(string language);
		void setEventName(string name, unsigned char len);
		void setDescription(string text, unsigned char len);
		unsigned char getEventNameLength();
		unsigned char getDescriptionLength();
		char * getLanguageCode();
		char * getEventName();
		char * getDescription();
};
}
}
}
}
}

#endif /*SHORTEVENTDESCRIPTOR_H_*/
