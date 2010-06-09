#include "../../../include/model/exceptions/PIDAlreadyExistsException.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
   	namespace exceptions {
   	PIDAlreadyExistsException::PIDAlreadyExistsException() 
	: Exception("PID already exists!"){
	}
   	
	PIDAlreadyExistsException::PIDAlreadyExistsException(string message) 
	: Exception(message){
	}
	
	PIDAlreadyExistsException::~PIDAlreadyExistsException() {
	}
   	}
   }
  }
 }
}
