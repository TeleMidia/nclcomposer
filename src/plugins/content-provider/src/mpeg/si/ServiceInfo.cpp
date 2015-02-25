#include "../../../include/mpeg/si/ServiceInfo.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
namespace sdt {
	ServiceInfo::ServiceInfo() {
		descriptorsLoopLength = 0;
	}

	ServiceInfo::~ServiceInfo() {

	}

	size_t ServiceInfo::getSize() {
		return (descriptorsLoopLength + 5);
	}

	void ServiceInfo::setServiceId(unsigned short id) {
		serviceId = id;
	}

	unsigned short ServiceInfo::getServiceId() {
		return serviceId;
	}
	
	void ServiceInfo::setEitScheduleFlag(unsigned char flag) {
		eitScheduleFlag = flag;
	}

	unsigned char ServiceInfo::getEitScheduleFlag() {
		return eitScheduleFlag;
	}

	void ServiceInfo::setEitPresentFollowingFlag(unsigned char flag) {
		eitPresentFollowingFlag = flag;
	}

	unsigned char ServiceInfo::getEitPresentFollowingFlag() {
		return eitPresentFollowingFlag;
	}

	void ServiceInfo::setRunningStatus(unsigned char status) {
		runningStatus = status;
	}

	unsigned char ServiceInfo::getRunningStatus() {
		return runningStatus;
	}

	void ServiceInfo::setFreeCAMode(unsigned char mode) {
		freeCAMode = mode;
	}

	unsigned char ServiceInfo::getFreeCAMode() {
		return freeCAMode;
	}

	unsigned short ServiceInfo::getDescriptorsLoopLength() {
		return descriptorsLoopLength;
	}

	void ServiceInfo::setDescriptorsLoopLength(unsigned short length) {
		descriptorsLoopLength = length;
	}

	void ServiceInfo::insertDescriptor(Descriptor* info) {
		LogoTransmissionDescriptor* ltd;
		set<Descriptor*>::iterator it;
		size_t count = 0;

		descriptors.insert(info);

		for (it = descriptors.begin(); it != descriptors.end(); it++) {
			switch ((*it)->getDescriptorTag()) {
			case 0xCF:
				ltd = (LogoTransmissionDescriptor*) (*it);
				break;
			default:
				break;
			}
			count += (ltd->getDescriptorLength() + 2);
		}
		descriptorsLoopLength = count;
	}

	set<Descriptor*> * ServiceInfo::getDescriptors() {
		return &descriptors;
	}

}
}
}
}
}
}
