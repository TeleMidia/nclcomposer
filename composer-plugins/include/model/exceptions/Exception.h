#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>

using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
   	namespace exceptions {
   	 class Exception {
   	 	private:
   	 		string message;
   	 	public:
			Exception(string message);
			virtual ~Exception();
			
			virtual string getMessage();
	 };
   	}
   }
  }
 }
}

#endif /*EXCEPTION_H_*/
