#include "../../include/mpeg/Descriptor.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
	Descriptor::Descriptor() {
		
	}
	
	Descriptor::~Descriptor() {
		
	}
	
	unsigned char Descriptor::getDescriptorTag() {
		return descriptorTag;
	}
	
	unsigned char Descriptor::getDescriptorLength() {
		return descriptorLength;
	}
	
	void Descriptor::setDescriptorLength(unsigned char length) {
		descriptorLength = length;
	}
	
	Descriptor* Descriptor::getDescriptor(
			vector<Descriptor*>* descriptors, unsigned char Tag) {
		vector<Descriptor*>::iterator dit;
		dit = descriptors->begin();
		while (dit != descriptors->end()) {
			if ((*dit)->getDescriptorTag() == Tag) {
				return *dit;
			}
			dit++;
		}
		return NULL;
	}
	
	vector<Descriptor*>* Descriptor::getDescriptors(
			vector<Descriptor*>* descriptors, unsigned char Tag) {
		vector<Descriptor*>* result;
		vector<Descriptor*>::iterator dit;
		result = new vector<Descriptor*>;
		dit = descriptors->begin();
		while (dit != descriptors->end()) {
			if ((*dit)->getDescriptorTag() == Tag) {
				result->push_back(*dit);
			}
			dit++;
		}
		if (result->size() == 0) {
			delete result;
			result = NULL;
		}
		return result;
	}
	
	int Descriptor::getDescriptorsLength(vector<Descriptor*>* descriptors) {
		int len = 0;
		vector<Descriptor*>::iterator dit;
		dit = descriptors->begin();
		while (dit != descriptors->end()) {
			len = len + (*dit)->getDescriptorLength() + 2;
			dit++;
		}
		return len;
	}
}
}
}
}
