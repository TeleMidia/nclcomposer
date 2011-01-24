#include "../../include/model/AITStream.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace model {
	AITStream::AITStream(ApplicationType applicationType)
			: ElementaryStream(ElementaryStream::AIT_ELEMENTARY_STREAM) {

		tableID = 0;
		this->applicationType = applicationType;
	}

	AITStream::~AITStream() {
		
	}

	void AITStream::setTableID(unsigned short id) {
		if(id > 0xFF) {
			id = 0xFF;
		} else {
			tableID = id;
		}
	}

	unsigned short AITStream::getTableID() {
		return tableID;
	}

	void AITStream::setTableVersion(unsigned short version) {
		this->version = version;
	}

	unsigned short AITStream::getTableVersion() {
		return version;
	}

	AITStream::ApplicationType AITStream::getApplicationType() {
		return applicationType;
	}

	bool AITStream::addApplication(AITApplication* application) 
			throw (exceptions::DifferentApplicationTypeException) {

		vector<AITApplication*>::iterator iter = applications.begin();
		while (iter != applications.end()){
			AITApplication* currentApplication = *iter;
			if(currentApplication == application){
				throw exceptions::DifferentApplicationTypeException();
				//return false;
			}
			iter++;
		}
		applications.push_back(application);
		notifyAdded(application);
		return true;
	}

	bool AITStream::removeApplication(AITApplication* application) {
		bool removed = false;
		vector<AITApplication*>::iterator iter = applications.begin();
		while(iter != applications.end()){
			AITApplication* currentApplication = *iter;
			if(currentApplication == application){
				iter = applications.erase(iter);
				removed = true;
				continue;
			}
			iter++;
		}
		return removed;
	}

	vector<AITApplication*> AITStream::getApplications() {
		return applications;
	}

	void AITStream::notifyRemoved(AITApplication* application) {
		for(unsigned int i=0; i< observers.size() ; i++) {
			observers[i]->onApplicationRemoved(this,application);
		}
	}

	void AITStream::notifyAdded(AITApplication* application) {
		for(unsigned int i=0; i< observers.size() ; i++) {
			observers[i]->onApplicationAdded(this,application);
		}
	}

	void AITStream::attachTableObserver(
			observers::AITTableObserver* observer) {

		////Check if the observer has already been added 
		vector<observers::AITTableObserver*>::iterator iter = observers.begin();
		while(iter != observers.end()){
			observers::AITTableObserver* currentObserver = *iter;
			if(currentObserver == observer)
				return; //Observer already exits!
			iter++;
		}
		observers.push_back(observer);
	}

	void AITStream::detachTableObserver(
			observers::AITTableObserver* observer) {

		vector<observers::AITTableObserver*>::iterator iter = observers.begin();
		while(iter != observers.end()){
			observers::AITTableObserver* currentObserver = *iter;
			if(currentObserver == observer){
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
