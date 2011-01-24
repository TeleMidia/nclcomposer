#ifndef ELEMENTARYSTREAMSGROUP_H_
#define ELEMENTARYSTREAMSGROUP_H_

#include <vector>
#include "ElementaryStream.h"
#include "observers/ElementaryStreamsSetObserver.h"
#include "exceptions/PIDAlreadyExistsException.h"

using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
	class ElementaryStreamsSet {
		private:
			vector<ElementaryStream*> streams;
		public:
			ElementaryStreamsSet();
			virtual ~ElementaryStreamsSet();
			
			void addStream(ElementaryStream* stream)
					throw (exceptions::PIDAlreadyExistsException);
			void removeStream(ElementaryStream* stream);
			vector<ElementaryStream*> getStreams();
			ElementaryStream* getStreamByPID(int pid);
		///observers
		private:
			vector<observers::ElementaryStreamsSetObserver*> observers;
			void notifyAdded(ElementaryStream* stream);
			void notifyRemoved(ElementaryStream* stream);
		public:
			void attachStreamsObserver(observers::ElementaryStreamsSetObserver* observer);
			void detachStreamsObserver(observers::ElementaryStreamsSetObserver* observer);
	};
   }
  }
 }
}
#endif /*ELEMENTARYSTREAMSGROUP_H_*/
