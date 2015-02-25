#ifndef LINKAGEDESCRIPTOR_H_
#define LINKAGEDESCRIPTOR_H_

#include <string.h>
using namespace std;

#include "../../Descriptor.h"
using namespace ::org::isdtv::contentProvider::mpeg;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {

	class LinkageDescriptor : public Descriptor {
	private:

	protected:
		unsigned short transportStreamId;
		unsigned short originalNetworkId;
		unsigned short serviceId;
		unsigned char linkageType;
		char privateData[248];
		unsigned char privateDataLength;
	public:
		LinkageDescriptor();
		~LinkageDescriptor();
		void setTransportStreamId(unsigned short transportStreamId);
		unsigned short getTransportStreamId(void);
		void setOriginalNetworkId(unsigned short originalNetworkId);
		unsigned short getOriginalNetworkId(void);
		void setServiceId(unsigned short serviceId);
		unsigned short getServiceId(void);
		void setLinkageType(unsigned char linkageType);
		unsigned char getLinkageType(void);
		void setPrivateData(char* data, unsigned char length);
		unsigned char getPrivateData(char* data);
		int encodeLinkage(char* section);
};
}
}
}
}
}

#endif /*LINKAGEDESCRIPTOR_H_*/
