#ifndef PMTSTREAMFACTORY_H_
#define PMTSTREAMFACTORY_H_

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <map>
#include <string>

#include "conf.h"
#include "esis.h"
#include "pes.h"
#include "pmt.h"
#include "util.h"

#include "OCStreamFactory.h"
#include "SEDStreamFactory.h"
#include "../../functions.h"
using namespace org::isdtv::util;

#ifndef DMSG
#define DMSG(x...) printf("dsmcc-pmt: "x); fflush(0)
#endif

#ifndef DLMSG
#define DLMSG(x...) printf(x); fflush(0)
#endif

#ifdef WIN32
  #define PMT_ES_PATH "C:\\temp\\cousins\\pmt_"
#else
  #define PMT_ES_PATH "/root/Desktop/pmt_"
#endif

class PMTStreamFactory {
	private:
		static unsigned char pmt_valid;
		static unsigned int pmt_pcr;
		static unsigned int pmt_prog;
		static unsigned short pmt_ver;
		static DsmccList* pmt_esis;

		static void dsmcc_pmt_conf(void);
		static unsigned int dsmcc_pmt_check(void);

	public:
		static void createPMTStream(
				std::string streamUri,
				unsigned int pid,
				unsigned int programNumber,
				unsigned int pcrPid,
				unsigned int version,
				std::map<unsigned int, short>* streamsInfo,
				std::map<unsigned int, CarouselInfo*>* ocs,
				std::map<unsigned int, SEInfo*>* ses);
};

#endif /*PMTSTREAMFACTORY_H_*/
