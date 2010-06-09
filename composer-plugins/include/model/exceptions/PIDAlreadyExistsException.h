#ifndef PIDALREADYEXISTSEXCEPTION_H_
#define PIDALREADYEXISTSEXCEPTION_H_

#include "Exception.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
   	namespace exceptions {
		class PIDAlreadyExistsException : public Exception{
			
			public:
				PIDAlreadyExistsException();
				PIDAlreadyExistsException(string message);
				virtual ~PIDAlreadyExistsException();
		};		
   	}
   }
  }
 }
}

#endif /*PIDALREADYEXISTSEXCEPTION_H_*/
