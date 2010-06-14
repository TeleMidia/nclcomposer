#include "../../../include/model/exceptions/Exception.h"


namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
   	namespace exceptions {
	 Exception::Exception(string message) {
	 	this->message = message;
	 }
	
	 Exception::~Exception() {
	 }
	 
	 string Exception::getMessage(){
	 	return message;
	 }
   	}
   }
  }
 }
}
