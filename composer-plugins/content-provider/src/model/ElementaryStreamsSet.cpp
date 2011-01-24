#include "../../include/model/ElementaryStreamsSet.h"
#include <iostream>

namespace org {
namespace isdtv {
namespace contentProvider {
namespace model {
	ElementaryStreamsSet::ElementaryStreamsSet() : streams() {
		
	}

	ElementaryStreamsSet::~ElementaryStreamsSet() {
		
	}

	void ElementaryStreamsSet::addStream(ElementaryStream* stream) 
			throw (exceptions::PIDAlreadyExistsException) {

		vector<ElementaryStream*>::iterator iter = streams.begin();

		while(iter != streams.end()){
			ElementaryStream* currStream = *iter;
			if(currStream->getPID() == stream->getPID())
				throw exceptions::PIDAlreadyExistsException();
			iter++;
		}
		streams.push_back(stream);
		notifyAdded(stream);
	}

	void ElementaryStreamsSet::removeStream(ElementaryStream* stream) {
		vector<ElementaryStream*>::iterator iter = streams.begin();
		while(iter != streams.end()){
			ElementaryStream* currStream = *iter;
			if (currStream->getPID() == stream->getPID() ||
					currStream == stream ) {

				iter = streams.erase(iter);
				notifyRemoved(stream);
				continue;
			}
			iter++;
		}
	}

	ElementaryStream* ElementaryStreamsSet::getStreamByPID(int pid) {
		/*std::cerr << "tamanho: "<< streams.size();
		std::cerr << "pID: "<< pid;
		std::cerr << "tamanho: "<< streams.size();
		for(unsigned int i=0;i<streams.size();i++) {
			if(streams[i] == NULL)
				continue;
			if(streams[i]->getPID() == pid) {
				return streams[i]; 
			}
		}*/

		vector<ElementaryStream*>::iterator iter = streams.begin();

		while(iter != streams.end()){
			ElementaryStream* currStream = *iter;
			if(currStream->getPID() == pid)
				return *iter;
			iter++;
		}
		return NULL;
	}

	vector<ElementaryStream*> ElementaryStreamsSet::getStreams() {
		return streams;
	}

	void ElementaryStreamsSet::notifyAdded(ElementaryStream* stream) {
		for(unsigned int i=0;i<observers.size();i++) {
			observers[i]->onElementaryStreamAdded(this, stream);
		}
	}

	void ElementaryStreamsSet::notifyRemoved(ElementaryStream* stream) {
		for(unsigned int i=0;i<observers.size();i++) {
			observers[i]->onElementaryStreamRemoved(this, stream);
		}
	}

	void ElementaryStreamsSet::attachStreamsObserver(
			observers::ElementaryStreamsSetObserver* observer) {

		vector<observers::ElementaryStreamsSetObserver*>::iterator iter;
		iter = observers.begin();

		while(iter != observers.end()){
			observers::ElementaryStreamsSetObserver* currObserver = *iter;
			if(currObserver == observer)
				return;
			iter++;
		}
		observers.push_back(observer);
	}

	void ElementaryStreamsSet::detachStreamsObserver(
			observers::ElementaryStreamsSetObserver* observer) {

		vector<observers::ElementaryStreamsSetObserver*>::iterator iter;
		iter = observers.begin();

		while(iter != observers.end()){
			observers::ElementaryStreamsSetObserver* currObserver = *iter;
			if(currObserver == observer){
				iter = observers.erase(iter);
				continue;
			}
			iter++;
		}
	}
}
}
}
}
