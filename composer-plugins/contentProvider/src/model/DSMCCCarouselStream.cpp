#include "../../include/model/DSMCCCarouselStream.h"


namespace org {
namespace isdtv {
namespace contentProvider {
namespace model {
	DSMCCCarouselStream::DSMCCCarouselStream(int serviceDomain)
			: ElementaryStream(ElementaryStream::DATA_ELEMENTARY_STREAM)  {

		rootPath = "";
		this->serviceDomain = serviceDomain; 
	}

	DSMCCCarouselStream::~DSMCCCarouselStream() {
		
	}

	int DSMCCCarouselStream::getServiceDomain(){
		return serviceDomain;
	}

	string DSMCCCarouselStream::getRootPath(){
		return rootPath;
	}

	void DSMCCCarouselStream::setRootPath(string newRootPath){
		rootPath = newRootPath;
	}

	void DSMCCCarouselStream::addStreamEventObject(
			StreamEventObject* steObject) {

		vector<StreamEventObject*>::iterator iter = streamEventObjects.begin();
		while(iter != streamEventObjects.end()){
			StreamEventObject* currSteObj = *iter;
			if(currSteObj == steObject)
				return;
			iter++;
		}
		streamEventObjects.push_back(steObject);
	}

	void DSMCCCarouselStream::removeStreamEventObject(
			StreamEventObject* steObject) {

		vector<StreamEventObject*>::iterator iter = streamEventObjects.begin();
		while(iter != streamEventObjects.end()){
			StreamEventObject* currSteObj = *iter;
			if(currSteObj == steObject){
				iter = streamEventObjects.erase(iter);
				continue;
			}
			iter++;
		}
	}
}
}
}
}
