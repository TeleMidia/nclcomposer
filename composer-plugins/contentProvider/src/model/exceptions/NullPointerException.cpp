#include "../../../include/model/exceptions/NullPointerException.h"


namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
   	namespace exceptions {
   		
	NullPointerException::NullPointerException() 
	: Exception("") {
	}
	
	NullPointerException::NullPointerException(string message) 
	: Exception(message) {
	}
	
	NullPointerException::~NullPointerException(){
	}
	
   	}
   }
  }
 }
}
