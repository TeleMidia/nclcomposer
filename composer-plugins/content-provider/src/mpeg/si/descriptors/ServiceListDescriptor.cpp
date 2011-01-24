#include "../../../../include/mpeg/si/descriptors/ServiceListDescriptor.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
	ServiceListDescriptor::ServiceListDescriptor() {
		descriptorTag = 0x41;
		items = new vector<ItemData*>;
	}
	
	ServiceListDescriptor::~ServiceListDescriptor() {
		if (items != NULL) {
			items->clear();
			delete items;
			items = NULL;
		}
	}
	
	void ServiceListDescriptor::insertService(
			unsigned short serviceId,
			unsigned char serviceType) {
		ItemData* ido;
		ido = new ItemData;	
	
		ido->serviceId = serviceId;
		ido->serviceType = serviceType;
	
		//push back into vector;
		items->push_back((ItemData*) ido);
		descriptorLength = (3 * items->size());
	}
	
	bool ServiceListDescriptor::getService(
			unsigned int position,
			unsigned short* serviceId,
			unsigned char* serviceType) {
		if (position < items->size()) {
			*serviceId = (*items)[position]->serviceId;
			*serviceType = (*items)[position]->serviceType;
			return true;
		}
		return false;
	}
	
	int ServiceListDescriptor::encodeServiceList(char* section) {
		int pos = 0;
		vector<ItemData*>::iterator it;
	
		section[pos] = getDescriptorTag();
		pos++;
		section[pos] = getDescriptorLength();
		pos++;
		it = items->begin();
		while (it != items->end()) {
			section[pos] = ((*it)->serviceId & 0xFF00) >> 8;
			pos++;
			section[pos] = ((*it)->serviceId & 0xFF);
			pos++;
			section[pos] = (*it)->serviceType;
			pos++;
			it++;
		}
		return pos;
	}
}
}
}
}
}
