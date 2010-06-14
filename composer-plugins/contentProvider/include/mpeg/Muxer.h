#ifndef MUXER_H_
#define MUXER_H_

#include "../functions.h"
using namespace org::isdtv::util;

#include "../server/SocketServer.h"
using namespace org::isdtv::network::server;

#include "dsmcc/PMTStreamFactory.h"

#include "si/Sdt.h"
#include "si/Eit.h"
#include "si/Cdt.h"
#include "si/Pmt.h"
#include "si/Pat.h"
using namespace org::isdtv::contentProvider::mpeg::si;

#include "TSPacket.h"

#ifdef WIN32
  #include <winsock2.h>
#else
  #include <sys/socket.h>
#endif

#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>

#include <map>
#include <vector>
#include <string>
using namespace std;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
	class Muxer {
		private:
			Pat* pat;
			static Muxer* _instance;
			unsigned int numOfPackets;
			unsigned int dataPacks;
			unsigned int eitPacks;
			unsigned int sdtPacks;
			unsigned int cdtPacks;
			short errCondition;
			string tsFileName;
			vector<string>* fileNames;
			map<string, unsigned int>* packetHandler;
			bool hasEit;
			bool hasSdt;
			bool hasCdt;

			static bool initThreadFlags;
			static pthread_mutex_t threadFlagMutex;
			static pthread_cond_t threadFlagConditionVariable;

		public:
			//defs
			static const short NB_PID_MAX = 0x1FFF; //8191

			// specific packet ids
			static const short PAT_PID    = 0x0000;
			static const short CAT_PID    = 0x0001;
			static const short TSDT_PID   = 0x0001; //TS Description Table
			static const short SDT_PID    = 0x0011;
			static const short EIT_PID    = 0x0012;
			static const short CDT_PID    = 0x0029;

			// specific section ids
			static const short PAT_TID    = 0x00;
			static const short CAT_TID    = 0x01;
			static const short PMT_TID    = 0x02;
			static const short TST_TID    = 0x02; //TS Description Section
			/*colocar as do DSM-CC (0x38 a 0x3F)*/
			static const short SDT_TID    = 0x42;
			static const short EIT_TID    = 0x4E;
			static const short CDT_TID    = 0xC8;

			static const short MUX_TO_FILE   = 0;
			static const short MUX_TO_SOCKET = 1;

			static const short ERR_CONDITION_SATISFIED = 5;

		private:
			Muxer();
			int getReadFD(const char* fileName);

		public:
			static Muxer* getInstance();
			string getStreamType(int pid);
			bool isSectionStream(int pid);
			bool openTSFile(string fileName);
			void saveTSFile(string fileName);
			void muxToSock(int fdin, SocketServer* sock);
			void muxToFile(int fdin, int fdout);

		private:
			bool dataPacketHandler(
					short type, int fdout=0, SocketServer* server=NULL);

			TSPacket* getNextPmtPacket(unsigned int pmtPid);
			TSPacket* getNextDataPacket();
			TSPacket* getNextTSPacketByPid(int fd, vector<unsigned int>* pids);

		public:
			map<unsigned int, Pmt*>* getProgramsInfo();
			unsigned int getTSId();
			string getTSFileName();
			Pat* getPat();
			void muxCarousels(vector<CarouselInfo*>* infos);
			void muxStreamEvents(vector<SEInfo*>* infos);
			/*void muxAppInformationTables(vector<AITInfo*>* infos);*/
			void checkConsistency();
			void setHasSdt(bool hasSdt);
			void setHasEit(bool hasEit);
			void setHasCdt(bool hasCdt);
			static void uSleep(double sleepValue);
	 };
   }
  }
 }
}

#endif /*MUXER_H_*/
