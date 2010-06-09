#ifndef OCSTREAMFACTORY_H_
#define OCSTREAMFACTORY_H_

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <map>
#include <string>

#include "../../../include/mpeg/dsmcc/conf.h"
#include "../../../include/mpeg/dsmcc/dccs.h"
#include "../../../include/mpeg/dsmcc/dii.h"
#include "../../../include/mpeg/dsmcc/diis.h"
#include "../../../include/mpeg/dsmcc/dsi.h"
#include "../../../include/mpeg/dsmcc/ents.h"
#include "../../../include/mpeg/dsmcc/exps.h"
#include "../../../include/mpeg/dsmcc/mods.h"
#include "../../../include/mpeg/dsmcc/msec.h"
#include "../../../include/mpeg/dsmcc/pes.h"
#include "../../../include/mpeg/dsmcc/secs.h"
#include "../../../include/mpeg/dsmcc/size.h"
#include "../../../include/mpeg/dsmcc/util.h"

#ifndef DMSG
#define DMSG(x...) printf("dsmcc-oc: "x); fflush(0)
#endif

#ifndef DLMSG
#define DLMSG(x...) printf(x); fflush(0)
#endif

#ifdef WIN32
  #define OC_ES_PATH "C:\\temp\\cousins\\oc_"
#else
  #define OC_ES_PATH "/root/Desktop/oc_"
#endif

typedef struct CarouselInfo {
	unsigned int pid; //es pid
	unsigned int programNumber;
	unsigned int myId; //sd
	unsigned int componentTag;
};

typedef struct ObjectInfo {
	std::string ior;
	unsigned int pid; //es pid
	unsigned int size;
	unsigned int sd;
	unsigned int componentTag;
};

class OCStreamFactory {
	private:
		static unsigned int  dsmcc_oc_dump;
		static DsmccList*    dsmcc_ext_pids;
		static unsigned int  dsmcc_srg_obj;
		static void dsmcc_oc_conf();

	public:
		static std::map<std::string, ObjectInfo*>* objectsIor;
		static std::map<std::string, ObjectInfo*>* getObjectsIor();
		static void createOCStream(
				std::string directoryUri,
				std::string streamUri,
				unsigned int pid,
				unsigned int serviceDomainId,
				unsigned int componentTag);
};

#endif /*OCSTREAMFACTORY_H_*/
