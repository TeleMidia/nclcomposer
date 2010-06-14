#ifndef TSPACKET_
#define TSPACKET_

#include "si/Pat.h"
using namespace org::isdtv::contentProvider::mpeg::si;

#include <iostream>
using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace mpeg {

   typedef struct {
   		unsigned short length;
   		char data[4096];
   	} SectionInfo;

	class TSPacket {
		public:
			static const short TS_PACKET_SIZE = 188;
			static const short TS_PACKET_SYNC_BYTE = 0x47;

		protected:
			// ISO/IEC 13818-1 TS PACKET HEADER
			char syncByte;
			bool transportErrorIndication;
			bool payloadUnitStartIndicator;
			bool transportPriority;
			unsigned int pid;

			/* 00 = Not scrambled
			 * 01 = User Defined
			 * 10 = User Defined
			 * 11 = User Defined
			 */
			unsigned int transportScramblingControl;

			/* 00 = ISO future use (0)
			 * 01 = no adaptation field (payload only) (1)
			 * 10 = adaptation field only (no payload) (2)
			 * 11 = adaptation field followed by payload (3)
			 */
			static const unsigned int FUTURE_USE = 0;
			static const unsigned int PAYLOAD_ONLY = 1;
			static const unsigned int NO_PAYLOAD = 2;
			static const unsigned int ADAPT_PAYLOAD = 3;
			unsigned int adaptationFieldControl;
			unsigned int continuityCounter;
			uint64_t pcr;
			char payload[184];
			char packData[TS_PACKET_SIZE];

			unsigned int payloadSize;

		public:
			TSPacket();
			TSPacket(char streamData[TS_PACKET_SIZE]);
			~TSPacket();

		protected:
			void process(char streamData[TS_PACKET_SIZE]);

		public:
			unsigned int getPid();
			void getPacketData(char streamData[TS_PACKET_SIZE]);
			void getPayload(char streamData[184]);
			unsigned int getPayloadSize();
			bool getStartIndicator();
			unsigned int getAdaptationFieldControl();
			unsigned int getContinuityCounter();
			void setContinuityCounter(unsigned int counter);
			uint64_t getPCRValue();
			void print();
			void tsPacketFactory(
					bool append, unsigned short pId,
					unsigned char unitStart,
					unsigned char adaptation,
					vector<SectionInfo*>* sections,
					string filename);
	};
   }
  }
 }
}

#endif /*TSPACKET_*/
