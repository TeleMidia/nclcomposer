#ifndef ELEMENTARYSTREAMSSETOBSERVER_H_
#define ELEMENTARYSTREAMSSETOBSERVER_H_

#include "../ElementaryStream.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
   	class ElementaryStreamsSet;
   	namespace observers {
   	 class ElementaryStreamsSetObserver {
	 	 public:
			 virtual ~ElementaryStreamsSetObserver(){}
			 
			 virtual void onElementaryStreamRemoved(ElementaryStreamsSet* source, ElementaryStream* stream) = 0;
			 virtual void onElementaryStreamAdded(ElementaryStreamsSet* source, ElementaryStream* stream) = 0;
	 };
   	}
   }
  }
 }
}

#endif /*ELEMENTARYSTREAMSSETOBSERVER_H_*/
