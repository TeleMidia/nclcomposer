#ifndef SERVICELISTDESCRIPTOR_H_
#define SERVICELISTDESCRIPTOR_H_

using namespace std;

#include "../../Descriptor.h"
using namespace ::org::isdtv::contentProvider::mpeg;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
 
	typedef struct {
		unsigned short serviceId;
		unsigned char serviceType;
	} ItemData;
	
	class ServiceListDescriptor : public Descriptor {
	private:
	
	protected:
		vector<ItemData*>* items;
	public:
		ServiceListDescriptor();
		~ServiceListDescriptor();
		void insertService(unsigned short serviceId, unsigned char serviceType);
		bool getService(
				unsigned int position,
				unsigned short *serviceId,
				unsigned char *serviceType);
		int encodeServiceList(char* section);
};
}
}
}
}
}

#endif /*SERVICELISTDESCRIPTOR_H_*/
