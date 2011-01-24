#ifndef EVENTINFOTABLE_H_
#define EVENTINFOTABLE_H_

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <set>

#include "../TSPacket.h"
#include "../TSSection.h"
#include "../Descriptor.h"
#include "EventInfo.h"
#include "descriptors/ShortEventDescriptor.h"

using namespace std;

#ifdef WIN32
  #define EIT_ES_PATH "C:\\temp\\cousins\\eit_"
#else
  #define EIT_ES_PATH "/root/Desktop/eit_"
#endif

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
namespace eit {
	class EventInfoTable : public mpeg::TSSection {
	private:

	protected:
		unsigned short nextId;

		unsigned short transportStreamId;
		unsigned short originalNetworkId;
		unsigned char segmentLastSectionNumber;
		unsigned char lastTableId;
		vector <EventInfo*>* eventInfo;
		unsigned int CRC32;

		vector <SectionInfo*>* encodedStream;
	public:
		EventInfoTable(char buf[184]);
		EventInfoTable();
		~EventInfoTable();
		unsigned short getNextId();
		void setTransportStreamId(unsigned short id);
		unsigned short getTransportStreamId();
		void setOriginalNetworkId(unsigned short id);
		unsigned short getOriginalNetworkId();
		void setSegmentLastSectionNumber(unsigned char number);
		unsigned char getSegmentLastSectionNumber();
		void setLastTableId(unsigned char id);
		unsigned char getLastTableId();
		unsigned short insertEvent(struct tm dateTime, struct tm duration);
		int removeEvent(unsigned short id);
		size_t calculateLastSection();
		size_t calculateSectionLength(vector<EventInfo*>::iterator it);
		void encodeStream(bool append, string filename);
		void decodeStream();
		void storeSection();

		virtual bool processSectionPayload(){return false;};
		void print();

		int insertShortEventDescriptor(
				unsigned short id,
				string language,
				string name,
				string description);
};
}
}
}
}
}
}

#endif /*EVENTINFOTABLE_H_*/
