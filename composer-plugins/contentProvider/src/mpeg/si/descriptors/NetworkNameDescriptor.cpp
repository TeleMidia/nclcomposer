#include "../../../../include/mpeg/si/descriptors/NetworkNameDescriptor.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
	NetworkNameDescriptor::NetworkNameDescriptor() {
		descriptorTag = 0x40;
	}
	
	NetworkNameDescriptor::~NetworkNameDescriptor() {
		
	}
	
	void NetworkNameDescriptor::setName(string networkName) {
		if (networkName.length() > 255)
		{
			descriptorLength = 255;
		} else {
			descriptorLength = networkName.length();
		}
		text.assign(networkName, 0, descriptorLength);
	}
	
	string NetworkNameDescriptor::getName(void) {
		return text; 
	}
	
	int NetworkNameDescriptor::encodeNetworkName(char* section) {
		int pos = 0;
	
		section[pos] = getDescriptorTag();
		pos++;
		section[pos] = getDescriptorLength();
		pos++;
		memcpy(section + pos, text.c_str(), text.length());
		pos = pos + text.length();
	
		return pos;
	}
}
}
}
}
}
