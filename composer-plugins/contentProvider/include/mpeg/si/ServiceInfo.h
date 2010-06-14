#ifndef SERVICEINFO_H_
#define SERVICEINFO_H_

#include <iostream>
#include <time.h>
#include <set>

#include "../Descriptor.h"
#include "descriptors/LogoTransmissionDescriptor.h"

using namespace std;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
namespace sdt {
    class ServiceInfo {
	private:
			
	protected:
		unsigned short serviceId; 
		bool eitScheduleFlag;
		bool eitPresentFollowingFlag;
		unsigned char runningStatus; 
		unsigned char freeCAMode;
		unsigned short descriptorsLoopLength;

	public:
		set<Descriptor*> descriptors;
		ServiceInfo();
		~ServiceInfo();
		size_t getSize();
		void setServiceId(unsigned short id);
		unsigned short getServiceId();
		void setEitScheduleFlag(unsigned char flag);
		unsigned char getEitScheduleFlag();
		void setEitPresentFollowingFlag(unsigned char flag);
		unsigned char getEitPresentFollowingFlag();
		void setRunningStatus(unsigned char status);
		unsigned char getRunningStatus();
		void setFreeCAMode(unsigned char mode);
		unsigned char getFreeCAMode();
		unsigned short getDescriptorsLoopLength();
		void setDescriptorsLoopLength(unsigned short length);
		void insertDescriptor(Descriptor* info);
		set<Descriptor*> * getDescriptors();
};
}
}
}
}
}
}

#endif /*SERVICEINFO_H_*/
