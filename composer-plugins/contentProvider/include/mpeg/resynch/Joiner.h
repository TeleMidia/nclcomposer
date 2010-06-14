#ifndef JOINER_H_
#define JOINER_H_

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "../../gui/ResynchPage.h"
#include "../dsmcc/PMTStreamFactory.h"
#include "../TSPacket.h"
#include "../si/Pat.h"
#include "../si/Pmt.h"

using namespace std;
using namespace org::isdtv::contentProvider::mpeg::si;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace resynch {
	class Joiner {
		private:
			bool keepTsId, keepPId, incrementVersionNumber, original;
			vector<string>* list;
		protected:
			bool getPatPmt(Pat** pat, Pmt** pmt, string filename);
		public:
			Joiner();
			~Joiner();
			static int getReadFD(string fileName);
			static int getWriteFD(string fileName);
			static long getFileSize(string fileName);
			void setKeepTsId(bool value);
			void setKeepPId(bool value);
			void setIncrementVersionNumber(bool value);
			void setOriginal(bool value);
			void addFile(string filename);
			void clearList();
			int joinFiles(string output);
};
}
}
}
}
}

#endif /*JOINER_H_*/
