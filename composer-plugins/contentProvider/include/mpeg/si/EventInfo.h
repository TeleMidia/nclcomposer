#ifndef EVENTINFO_H_
#define EVENTINFO_H_

#include <iostream>
#include <time.h>
#include <set>

#include "../Descriptor.h"
#include "descriptors/ShortEventDescriptor.h"

using namespace std;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
namespace eit {
    class EventInfo {
	private:
			
	protected:
		unsigned short eventId; 
		char startTimeEncoded[5];
		char durationEncoded[3];
		unsigned char runningStatus; 
		unsigned char freeCAMode;
		unsigned short descriptorsLoopLength;

		struct tm startTime;
		struct tm duration;
	public:
		set<Descriptor*> descriptors;
		EventInfo();
		~EventInfo();
		static int bcd(int dec);
		static int decimal(int bcd);
		static int gmjd(struct tm gregorian);
		static struct tm mjdg(int mjd);
		size_t getSize();
		void setEventId(unsigned short id);
		unsigned short getEventId();
		void setStartTime(struct tm st);
		struct tm getStartTime();
		char * getStartTimeEncoded();
		void setStartTimeEncoded(char* ste);
		void setDuration(struct tm duration);
		struct tm getDuration();
		void setDurationEncoded(char* duration);
		char * getDurationEncoded();
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

#endif /*EVENTINFO_H_*/
