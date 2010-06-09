#include "../../../../include/mpeg/si/descriptors/LogoTransmissionDescriptor.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
	LogoTransmissionDescriptor::LogoTransmissionDescriptor() {
		descriptorTag = 0xCF;
		descriptorLength = 1;
	}

	LogoTransmissionDescriptor::~LogoTransmissionDescriptor() {
		
	}

	void LogoTransmissionDescriptor::setType(unsigned char type) {
		logoType = type;
		switch (logoType) {
		case 0x01:
			descriptorLength = 7;
			break;
		case 0x02:
			descriptorLength = 3;
			break;
		default:
			break;
		}
	}

	unsigned char LogoTransmissionDescriptor::getType() {
		return logoType;
	}

	void LogoTransmissionDescriptor::setLogoId(unsigned short id) {
		logoId = id;
	}

	unsigned short LogoTransmissionDescriptor::getLogoId() {
		return logoId;
	}

	void LogoTransmissionDescriptor::setLogoVersion(unsigned short version) {
		logoVersion = version;
	}

	unsigned short LogoTransmissionDescriptor::getLogoVersion() {
		return logoVersion;
	}

	void LogoTransmissionDescriptor::setDownloadDataId(unsigned short id) {
		downloadDataId = id;
	}

	unsigned short LogoTransmissionDescriptor::getDownloadDataId() {
		return downloadDataId;
	}

	void LogoTransmissionDescriptor::setName(string name) {
		logoName = name;
		descriptorLength = (logoName.length() + 1);
		logoType = 0x03;
	}

	string LogoTransmissionDescriptor::getName() {
		return logoName;
	}

	unsigned char LogoTransmissionDescriptor::getNameLength() {
		return logoName.length();
	}
}
}
}
}
}
