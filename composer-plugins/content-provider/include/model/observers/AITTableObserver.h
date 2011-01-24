#ifndef AITTABLEOBSERVER_H_
#define AITTABLEOBSERVER_H_

#include "../AITApplication.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
   	class AITStream;
   	namespace observers {
   	 class AITTableObserver {
	 	 public:
			 virtual ~AITTableObserver(){}
			 
			 virtual void onApplicationRemoved(AITStream* source, AITApplication* application) = 0;
			 virtual void onApplicationAdded(AITStream* source, AITApplication* application) = 0;
	 };
   	}
   }
  }
 }
}

#endif /*AITTABLEOBSERVER_H_*/
