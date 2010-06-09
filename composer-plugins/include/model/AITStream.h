#ifndef AITTABLE_H_
#define AITTABLE_H_

#include <vector>
#include "ElementaryStream.h"
#include "observers/AITTableObserver.h"
#include "exceptions/DifferentApplicationTypeException.h"

using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
	class AITStream : public ElementaryStream {
		public:
			enum ApplicationType { DVB_J_APPLICATION = 0x0001,
								   DVB_HTML_APPLICATION = 0x0002 };
	//////// MODEL
		private:
			unsigned short tableID; //MAX 8 bits
			unsigned short version;
			ApplicationType applicationType; //MAX 15bits
			vector<AITApplication*> applications;
			
			vector<observers::AITTableObserver*> observers;
		public:
			AITStream(ApplicationType applicationType);
			virtual ~AITStream();
			
			virtual void 								setTableID(unsigned short id);
			virtual unsigned short 						getTableID();
			virtual void 								setTableVersion(unsigned short verison);
			virtual unsigned short 						getTableVersion();
			
			virtual bool 								addApplication(AITApplication* application)
															throw (exceptions::DifferentApplicationTypeException);
			virtual bool 								removeApplication(AITApplication* application);
			
			virtual ApplicationType 					getApplicationType();
			
			virtual vector<AITApplication*> getApplications();
			
	/////// Observers
		private:
			virtual void notifyRemoved(AITApplication* application);
			virtual void notifyAdded(AITApplication* application);
		public:
			virtual void attachTableObserver(observers::AITTableObserver* observer);
			virtual void detachTableObserver(observers::AITTableObserver* observer);
	};
   }
  }
 }
}

#endif /*AITTABLE_H_*/
