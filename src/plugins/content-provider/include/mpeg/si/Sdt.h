#ifndef SDT_H_
#define SDT_H_

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <set>

#include "../TSPacket.h"
#include "../TSSection.h"
#include "ServiceInfo.h"
#include "../Descriptor.h"
#include "descriptors/LogoTransmissionDescriptor.h"

#ifdef WIN32
  #define SDT_ES_PATH "C:\\temp\\cousins\\sdt_"
#else
  #define SDT_ES_PATH "/root/Desktop/sdt_"
#endif

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
namespace sdt {

	class ServiceDescriptionTable : public mpeg::TSSection {
	private:

	protected:
		unsigned short originalNetworkId;
		vector <ServiceInfo*>* serviceInfo;
		unsigned int CRC32;

		vector <SectionInfo*>* encodedStream;
	public:
		ServiceDescriptionTable(char buf[184]);
		ServiceDescriptionTable();
		~ServiceDescriptionTable();
		void setOriginalNetworkId(unsigned short id);
		unsigned short getOriginalNetworkId();
		unsigned short insertEvent(
				unsigned short serviceId,
				bool eitSchedule,
				bool eitPresentFollowing,
				unsigned char runningStatus,
				bool freeCAMode);
		int removeEvent(unsigned short id);
		size_t calculateLastSection();
		size_t calculateSectionLength(vector<ServiceInfo*>::iterator it);
		void encodeStream(bool append, string filename);
		void decodeStream();
		void storeSection(char buffer[4096], int slen);

		virtual bool processSectionPayload(){return false;};
		void print();

		//virtual bool processSectionPayload(){return false;};
		int insertLogoTransmissionDescriptor(
						unsigned short serviceId,
						unsigned short type,
						unsigned short logoId,
						unsigned short logoVersion,
						unsigned short downloadDataId,
						string name);
};
}
}
}
}
}
}

#endif /*SDT_H_*/
