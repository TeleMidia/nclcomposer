#ifndef SEDSTREAMFACTORY_H_
#define SEDSTREAMFACTORY_H_

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <string>
#include <iostream>
using namespace std;

#include "conf.h"
#include "pes.h"
#include "sec.h"
#include "sted.h"
#include "util.h"

#ifndef DMSG
#define DMSG(x...) printf("dsmcc-ste: "x); fflush(0)
#endif

#ifndef DLMSG
#define DLMSG(x...) printf(x); fflush(0)
#endif

#ifdef WIN32
  #define SE_ES_PATH "C:\\temp\\cousins\\se_"
#else
  #define SE_ES_PATH "/root/Desktop/se_"
#endif
typedef struct SEInfo {
	unsigned int pid; //es pid
	unsigned int programNumber;
	unsigned int myId; //event_id
	unsigned int componentTag;
};

class SEDStreamFactory {
	private:
		static unsigned short event_id;
		static unsigned int event_npt;
		static char* event_desc;
		static unsigned int event_dlen;
		static unsigned short event_ver;

		static void dsmcc_ste_conf(void);
		static unsigned int dsmcc_ste_check(void);

	public:
		static void createSEDStream(
				std::string streamUri,
				unsigned int pid,
				unsigned int cTag,
				unsigned int eventId,
				unsigned int npt,
				std::string privateData);
};

#endif /*SEDSTREAMFACTORY_H_*/
