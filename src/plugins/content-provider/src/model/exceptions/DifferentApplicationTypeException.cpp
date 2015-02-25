#include "../../../include/model/exceptions/DifferentApplicationTypeException.h"


namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
   	namespace exceptions {
	DifferentApplicationTypeException::DifferentApplicationTypeException()
	: Exception("The type of applications in a AIT must be the same!") {
	}
	
	DifferentApplicationTypeException::~DifferentApplicationTypeException() {
	}
   	}
   }
  }
 }
}
