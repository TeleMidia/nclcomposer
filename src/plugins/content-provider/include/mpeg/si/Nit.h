#ifndef NIT_H_
#define NIT_H_

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <set>

#include "../TSPacket.h"
#include "../TSSection.h"
#include "../Descriptor.h"
#include "descriptors/ServiceListDescriptor.h"
#include "descriptors/NetworkNameDescriptor.h"

#ifdef WIN32
  #define NIT_ES_PATH "C:\\temp\\cousins\\nit_"
#else
  #define NIT_ES_PATH "/root/Desktop/nit_"
#endif

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
namespace nit {

	class NetworkInformationTable : public mpeg::TSSection {
	private:
		vector<Descriptor*>* descriptors;
	protected:
		unsigned int CRC32;
		unsigned short networkDescriptorsLength;
		vector <SectionInfo*>* encodedStream;
	public:
		NetworkInformationTable();
		~NetworkInformationTable();

		void insertSection(char buf[4096]);
		bool insertNetworkNameDescriptor(string name);
		bool insertServiceListDescriptor(
				unsigned short serviceId, unsigned char serviceType);
		void encodeStream(bool append, string filename);
		void decodeStream();

		virtual bool processSectionPayload(){return false;};
};
}
}
}
}
}
}


#endif /*NIT_H_*/
