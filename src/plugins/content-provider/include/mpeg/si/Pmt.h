#ifndef PMT_H_
#define PMT_H_

#include "../TSSection.h"
using namespace org::isdtv::contentProvider::mpeg;

#include "../dsmcc/OCStreamFactory.h"
#include "../dsmcc/SEDStreamFactory.h"

#include "../../functions.h"
using namespace org::isdtv::util;

#include <map>
#include <string>
using namespace std;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
	 class Pmt : public TSSection {
	 	public:
	 		static const short STREAM_TYPE_ISO_RESERVED;
	 		static const short STREAM_TYPE_VIDEO_MPEG1;
			static const short STREAM_TYPE_VIDEO_MPEG2;
			static const short STREAM_TYPE_AUDIO_MPEG1;
			static const short STREAM_TYPE_AUDIO_MPEG2;
			static const short STREAM_TYPE_PRIVATE_SECTION;
			static const short STREAM_TYPE_PRIVATE_DATA;
			static const short STREAM_TYPE_MHEG;
			static const short STREAM_TYPE_MPEG2_DSMCC;
			static const short STREAM_TYPE_H222_1;
			static const short STREAM_TYPE_DSMCC_TYPE_A;
			static const short STREAM_TYPE_DSMCC_TYPE_B;
			static const short STREAM_TYPE_DSMCC_TYPE_C;
			static const short STREAM_TYPE_DSMCC_TYPE_D;
			static const short STREAM_TYPE_MPEG2_AUX;
			static const short STREAM_TYPE_AUDIO_AAC;
			static const short STREAM_TYPE_VIDEO_MPEG4;
			static const short STREAM_TYPE_AUDIO_MPEG4;
			static const short STREAM_TYPE_SL_PES;
			static const short STREAM_TYPE_SL_SECTION;
			static const short STREAM_TYPE_DSMCC_SYNCH;
			static const short STREAM_TYPE_META_PES;
			static const short STREAM_TYPE_META_SEC;
			static const short STREAM_TYPE_META_DATA_CAROUSEL;
			static const short STREAM_TYPE_META_OBJECT_CAROUSEL;
			static const short STREAM_TYPE_META_DSMCC_SYNCH;
			static const short STREAM_TYPE_MPEG2_IPMP_STREAM;
			static const short STREAM_TYPE_VIDEO_H264;
			static const short STREAM_TYPE_IPMP_STREAM;
			static const short STREAM_TYPE_AUDIO_AC3;
			static const short STREAM_TYPE_AUDIO_DTS;

			static const uint64_t SYSTEM_CLOCK_FREQUENCY = 27000000;

	 	protected:
	 		map<unsigned int, short>* streams;
	 		map<short, string>* streamTypeNames;
	 		map<unsigned int, CarouselInfo*>* carousels;
	 		map<unsigned int, SEInfo*>* ses;
	 		//map<unsigned int, AITInfo*>* aits;
	 		unsigned int pid;
	 		unsigned int programNumber;
	 		unsigned int pcrPid;
			vector<uint64_t>* pcrValues;
			vector<unsigned int>* pcrIndex;
			vector<uint64_t>* pcrRates;

		public:
			Pmt(unsigned int pid, unsigned int programNumber);
			void addElementaryStream(unsigned int pid, short esType);
			void addCarouselInfo(unsigned int pid, CarouselInfo* info);
			void addSEInfo(unsigned int pid, SEInfo* info);
			//void addAITInfo(unsigned int pid, AITInfo* info);
			unsigned int getPid();
			void setPid(unsigned int pid);
			unsigned int getProgramNumber();
			void setProgramNumber(unsigned int programNumber);
			bool hasPid(unsigned int somePid);
			string getStreamType(unsigned int somePid);
			bool isSectionType(unsigned int pid);
			short getStreamTypeValue(unsigned int somePid);
			string getStreamTypeName(short streamType);
			bool processSectionPayload();
			map<unsigned int, short>* getStreamsInformation();
	 		map<unsigned int, CarouselInfo*>* getCarousels();
	 		map<unsigned int, SEInfo*>* getStreamEvents();
			unsigned int getPCRPid();
			void setPCRPid(unsigned int pcrPid);
			bool hasData();
			double getTransportRate(unsigned int rateIndex);
			void setPcrIndex(vector<unsigned int>* index);
			void setPcrValues(vector<uint64_t>* readedValues);
			void refreshPcrRates();
			void print();
	 };
}
}
}
}
}

#endif /*PMT_H_*/
