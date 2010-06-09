#ifndef NULLPOINTEREXCEPTION_H_
#define NULLPOINTEREXCEPTION_H_

#include "Exception.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
   	namespace exceptions {
	class NullPointerException : public Exception {
		public:
			NullPointerException();
			NullPointerException(string message);
			virtual ~NullPointerException();
	};
   	}
   }
  }
 }
}

#endif /*NULLPOINTEREXCEPTION_H_*/
