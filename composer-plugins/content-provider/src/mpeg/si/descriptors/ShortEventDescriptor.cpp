#include "../../../../include/mpeg/si/descriptors/ShortEventDescriptor.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
	ShortEventDescriptor::ShortEventDescriptor() {
		descriptorTag = 0x4D;
		eventNameLength = 0;
		descriptionLength = 0;
	}

	ShortEventDescriptor::~ShortEventDescriptor() {
		
	}

	void ShortEventDescriptor::setLanguageCode(string language) {
		strncpy(languageCode, language.c_str(), 3);
	}

	void ShortEventDescriptor::setEventName(string name, unsigned char len) {
		eventNameLength = len;
		memset(eventName, 0, 255);
		memcpy(eventName, name.c_str(), eventNameLength);
		descriptorLength = (eventNameLength + descriptionLength + 5);
	}

	void ShortEventDescriptor::setDescription(string text, unsigned char len) {
		descriptionLength = len;
		memset(description, 0, 255);
		memcpy(description, text.c_str(), descriptionLength);
		descriptorLength = (eventNameLength + descriptionLength + 5);
	}

	unsigned char ShortEventDescriptor::getEventNameLength() {
		return eventNameLength;
	}

	unsigned char ShortEventDescriptor::getDescriptionLength() {
		return descriptionLength;
	}

	char * ShortEventDescriptor::getLanguageCode() {
		return languageCode;
	}

	char * ShortEventDescriptor::getEventName() {
		return eventName;
	}

	char * ShortEventDescriptor::getDescription() {
		return description;
	}
}
}
}
}
}
