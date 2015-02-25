#ifndef DSMCCCAROUSEL_H_
#define DSMCCCAROUSEL_H_

#include <vector>
#include <string>
#include "ElementaryStream.h"
#include "StreamEventObject.h"

using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
	class DSMCCCarouselStream : public ElementaryStream  {
		private:
			string rootPath;
			int serviceDomain;
			
			vector<StreamEventObject*> streamEventObjects;
		public:
			DSMCCCarouselStream(int serviceDomain);
			virtual ~DSMCCCarouselStream();
			
			int 	getServiceDomain();
			string 	getRootPath();
			void 	setRootPath(string newRootPath);
			
			void 	addStreamEventObject(StreamEventObject* steObject);
			void 	removeStreamEventObject(StreamEventObject* steObject);
	};
   }
  }
 }
}

#endif /*DSMCCCAROUSEL_H_*/
