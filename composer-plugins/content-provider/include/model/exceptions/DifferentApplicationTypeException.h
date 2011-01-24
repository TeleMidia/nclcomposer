#ifndef DIFFERENTAPPLICATIONTYPEEXCEPTION_H_
#define DIFFERENTAPPLICATIONTYPEEXCEPTION_H_

#include "Exception.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
   	namespace exceptions {
	class DifferentApplicationTypeException : public Exception {
		public:
			DifferentApplicationTypeException();
			virtual ~DifferentApplicationTypeException();
	};
   	}
   }
  }
 }
}

#endif /*DIFFERENTAPPLICATIONTYPEEXCEPTION_H_*/
