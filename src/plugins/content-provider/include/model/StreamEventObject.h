#ifndef STREAMEVENTOBJECT_H_
#define STREAMEVENTOBJECT_H_

#include <string>

using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
	class StreamEventObject {
		private:
			int id;
			string name;
		public:
			StreamEventObject();
			virtual ~StreamEventObject();
			
			int getID();
			void setID(int newID);
			string getName();
			void setName(string newName);
	};
   }
  }
 }
}

#endif /*STREAMEVENTOBJECT_H_*/
