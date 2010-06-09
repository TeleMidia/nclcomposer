#ifndef EXTENDEDEVENTDESCRIPTOR_H_
#define EXTENDEDEVENTDESCRIPTOR_H_

#include <iostream>
#include <vector>
using namespace std;

#include "../../Descriptor.h"
using namespace ::org::isdtv::contentProvider::mpeg;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
 
	typedef struct {
		unsigned char fieldLength;
		char fieldName[255];
		unsigned char textLength;
		char text[255];
	} ItemData;

	class ExtendedEventDescriptor : public Descriptor {
	private:
		
	protected:
		unsigned char descriptorNumber;
		unsigned char lastDescriptorNumber;
		char languageCode[3];
		unsigned char lengthOfItems;
		vector<ItemData*>* items;
		unsigned char textLength;
		char text[255];
	public:
		ExtendedEventDescriptor();
		~ExtendedEventDescriptor();
};
}
}
}
}
}
#endif /*EXTENDEDEVENTDESCRIPTOR_H_*/
