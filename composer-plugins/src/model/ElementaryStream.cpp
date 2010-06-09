#include <stdlib.h>
#include "../../include/model/ElementaryStream.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace model {
   	ElementaryStream::ElementaryStream(ElementaryStreamType streamType) {
   		this->streamType = streamType;
   		pid = 0;
		programNumber = 0;
   	}

	/*ElementaryStream::ElementaryStream(ElementaryStreamContent* content) {
		this->content = content;
		pid = 0;
		programNumber = 0;
		if(content == NULL)
			throw exceptions::NullPointerException();
			
		switch(content->getContentType()){
			case ElementaryStreamContent::VIDEO_CONTENT:
				streamType = VIDEO_ELEMENTARY_STREAM;
				break;
			case ElementaryStreamContent::AUDIO_CONTENT:
				streamType = AUDIO_ELEMENTARY_STREAM;
				break;
			case ElementaryStreamContent::DATA_CONTENT:
				streamType = DATA_ELEMENTARY_STREAM;
				break;
			case ElementaryStreamContent::AIT_CONTENT:
				streamType = AIT_ELEMENTARY_STREAM;
				break;
			default:
				streamType = UNKNOWN_ELEMENTARY_STREAM;  
		}
	}*/

	ElementaryStream::~ElementaryStream() {
		
	}

	int ElementaryStream::getPID() {
		return pid;
	}

	void ElementaryStream::setPID(int pid) {
		this->pid = pid;
	}

	int ElementaryStream::getProgramNumber() {
		return programNumber;
	}

	void ElementaryStream::setProgramNumber(int programNumber) {
		this->programNumber = programNumber;
	}

	ElementaryStream::ElementaryStreamType ElementaryStream::getType() {
		return streamType;
	}

	/*ElementaryStreamContent* ElementaryStream::getContent(){
		return content;
	}*/
}
}
}
}
