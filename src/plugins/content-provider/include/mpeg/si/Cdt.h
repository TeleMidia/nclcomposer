#ifndef CDT_H_
#define CDT_H_

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <set>

#include "../TSPacket.h"
#include "../TSSection.h"
#include "../Descriptor.h"

#ifdef WIN32
  #define CDT_ES_PATH "C:\\temp\\cousins\\cdt_"
#else
  #define CDT_ES_PATH "/root/Desktop/cdt_"
#endif

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
namespace cdt {
	class CommonDataTable : public mpeg::TSSection {
	private:

	protected:
		unsigned short originalNetworkId;
		unsigned char dataType;
		unsigned short descriptorsLoopLength;
		set<Descriptor*> descriptors;
		unsigned int CRC32;

		char * pngData;
		size_t pngLength;

		vector <SectionInfo*>* encodedStream;
	public:
		CommonDataTable(char buf[184]);
		CommonDataTable();
		~CommonDataTable();
		void setOriginalNetworkId(unsigned short id);
		unsigned short getOriginalNetworkId();
		void setDataType(unsigned char type);
		unsigned char getDataType();
		int loadPNG(string path);
		int savePNG(string path);
		size_t calculateLastSection();
		void encodeStream(bool append, string filename);
		void decodeStream();
		void storeSection();

		virtual bool processSectionPayload(){return false;};
		void print();

		//virtual bool processSectionPayload(){return false;};
};
}
}
}
}
}
}

#endif /*CDT_H_*/
