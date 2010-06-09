#include "../../../../include/mpeg/si/descriptors/LinkageDescriptor.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
	LinkageDescriptor::LinkageDescriptor() {
		descriptorTag = 0x4A;
	}

	LinkageDescriptor::~LinkageDescriptor() {
		
	}
	
	void LinkageDescriptor::setTransportStreamId(
			unsigned short transportStreamId) {
		this->transportStreamId = transportStreamId;
	}

	unsigned short LinkageDescriptor::getTransportStreamId(void) {
		return transportStreamId;
	}

	void LinkageDescriptor::setOriginalNetworkId(
			unsigned short originalNetworkId) {
		this->originalNetworkId = originalNetworkId;
	}

	unsigned short LinkageDescriptor::getOriginalNetworkId(void) {
		return originalNetworkId;
	}

	void LinkageDescriptor::setServiceId(unsigned short serviceId) {
		this->serviceId = serviceId;
	}

	unsigned short LinkageDescriptor::getServiceId(void) {
		return serviceId;
	}

	void LinkageDescriptor::setLinkageType(unsigned char linkageType) {
		this->linkageType = linkageType;
	}

	unsigned char LinkageDescriptor::getLinkageType(void) {
		return linkageType;
	}

	void LinkageDescriptor::setPrivateData(char* data, unsigned char length) {
		if (length > 248) {
			length = 248;
		}
		memcpy(privateData, data, length);
		privateDataLength = length;
	}

	unsigned char LinkageDescriptor::getPrivateData(char* data) {
		memcpy(data, privateData, privateDataLength);
		return privateDataLength;
	}

	int LinkageDescriptor::encodeLinkage(char* section) {
		int pos = 0;

		section[pos] = getDescriptorTag();
		pos++;
		section[pos] = getDescriptorLength();
		pos++;
		section[pos] = (getTransportStreamId() & 0xFF00) >> 8;
		pos++;
		section[pos] = (getTransportStreamId() & 0xFF);
		pos++;
		section[pos] = (getOriginalNetworkId() & 0xFF00) >> 8;
		pos++;
		section[pos] = (getOriginalNetworkId() & 0xFF);
		pos++;
		section[pos] = (getServiceId() & 0xFF00) >> 8;
		pos++;
		section[pos] = (getServiceId() & 0xFF);
		pos++;
		section[pos] = getLinkageType();
		pos++;
		memcpy(section + pos, privateData, privateDataLength);
		pos = pos + privateDataLength;

		return pos;
	}
}
}
}
}
}
