#ifndef ELEMENTARYSTREAM_H_
#define ELEMENTARYSTREAM_H_

#include "exceptions/NullPointerException.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
	class ElementaryStream {
		public:
			enum ElementaryStreamType {
				VIDEO_ELEMENTARY_STREAM,
				AUDIO_ELEMENTARY_STREAM,
				DATA_ELEMENTARY_STREAM,
				AIT_ELEMENTARY_STREAM,
				UNKNOWN_ELEMENTARY_STREAM
			};
			
		private:
			int pid;
			int programNumber;
			ElementaryStreamType streamType;
			//ElementaryStreamContent* content;
		
		protected:
			ElementaryStream(ElementaryStreamType streamType);
			
		public:
			//ElementaryStream(ElementaryStreamContent* content);
			virtual ~ElementaryStream();
			
			virtual int getPID();
			virtual void setPID(int pid);
			virtual int getProgramNumber();
			virtual void setProgramNumber(int programNumber);
			virtual ElementaryStreamType getType();
			
			//virtual void addContent(ElementaryStreamContent* content);
			//virtual ElementaryStreamContent* getContent();
	};
   }
  }
 }
}

#endif /*ELEMENTARYSTREAM_H_*/
