#ifndef PAT_H_
#define PAT_H_

#include "Pmt.h"
#include "../TSSection.h"

#include <map>
#include <vector>
#include <iostream>
#include <string>
using namespace std;

#ifdef WIN32
  #define PAT_ES_PATH "C:\\temp\\cousins\\pat_"
#else
  #include "errno.h"
  #define PAT_ES_PATH "/root/Desktop/pat_"
#endif

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
	class Pat : public TSSection {
		protected:
			unsigned int currentPid;
			unsigned int currentCarouselId;
			unsigned int carouselComponentTag;
			unsigned int tsPid;

			//Program association table (pmt pid x program number)
			// if i->first == 0 i->second = networkPid
			map<unsigned int, unsigned int>* pat;

			vector<unsigned int>* unprocessedPmts;

			//PMT Pid TO PMT
			map<unsigned int, Pmt*>* programs;

		public:
			Pat();
			virtual ~Pat(){};
			unsigned int getNextPid();
			unsigned int getNextCarouselId();
			unsigned int getCarouselComponentTag();
			bool hasProgramNumber(unsigned int programNumber);
			bool hasPid(unsigned int pid);
			Pmt* getPmtByProgramNumber(unsigned int programNumber);
			unsigned int getTSId();
			void setTSId(unsigned int newTSId);
			unsigned int getNumOfPrograms();
			void addProgram(unsigned int pid, unsigned int programNumber);
			void addPmt(Pmt* program);
			unsigned int getFirstProgramNumber();
			unsigned int getProgramNumberByPid(unsigned int pid);
			string getStreamType(unsigned int pid);
			bool isSectionType(unsigned int pid);
			bool processSectionPayload();
			bool hasUnprocessedPmt();
			vector<unsigned int>* getUnprocessedPmtPids();
			map<unsigned int, Pmt*>* getProgramsInfo();
			void checkConsistency();
			bool hasData();
			int createPATStream(string streamUri);

			void print();
	 };
}
}
}
}
}

#endif /*PAT_H_*/
