#include "../../../include/mpeg/si/EventInfo.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
namespace eit {
	EventInfo::EventInfo() {
		descriptorsLoopLength = 0;
	}

	EventInfo::~EventInfo() {

	}

	int EventInfo::bcd(int dec)
	{
		return ((dec / 10) << 4) + (dec % 10);
	}

	int EventInfo::decimal(int bcd)
	{
		return ((bcd >> 4) * 10) + bcd % 16;
	}

	int EventInfo::gmjd(struct tm gregorian) {
		short year, month, day;
		year = gregorian.tm_year;
		month = gregorian.tm_mon;
		day = gregorian.tm_mday;
		return (day - 32075 + 1461 * (year + 4800 + (month - 14) / 12)
			/ 4 + 367 * (month - 2 - (month - 14) / 12 * 12)
			/12 - 3 * ((year + 4900 + (month - 14) / 12) / 100) / 4) - 2400001;
	}

	struct tm EventInfo::mjdg(int mjd) {
		int jd, l, n, i, j, k;
		struct tm r;
		jd = mjd + 2400001;
		l = jd + 68569;
		n = 4 * l / 146097;
		l = l - (146097 * n + 3) / 4;
		i = 4000 * (l + 1) / 1461001;
		l = l - 1461 * i / 4 + 31;
		j = 80 * l / 2447;
		k = l - 2447 * j / 80;
		l = j / 11;
		j = j + 2 - 12 * l;
		i = 100 * (n - 49) + i + l;
		
		r.tm_mday = (short) k; 
		r.tm_mon = (short) j;
		r.tm_year = (short) i;
		
		return r;
	}

	size_t EventInfo::getSize() {
		return (descriptorsLoopLength + 12);
	}

	void EventInfo::setEventId(unsigned short id) {
		eventId = id;
	}

	unsigned short EventInfo::getEventId() {
		return eventId;
	}

	void EventInfo::setStartTime(struct tm st) {
		int date;

		startTime = st;

		date = gmjd(st);

		startTimeEncoded[0] = (date & 0xFF00) >> 8;
		startTimeEncoded[1] = (date & 0xFF);

		startTimeEncoded[2] = bcd(st.tm_hour);
		startTimeEncoded[3] = bcd(st.tm_min);
		startTimeEncoded[4] = bcd(st.tm_sec);
	}

	struct tm EventInfo::getStartTime() {
		return startTime;
	}

	char * EventInfo::getStartTimeEncoded() {
		return startTimeEncoded;
	}

	void EventInfo::setStartTimeEncoded(char* ste) {
		unsigned short myDate;

		memcpy(startTimeEncoded, ste, 5);

		myDate = (((startTimeEncoded[0] << 8) & 0xFF00) |
		(startTimeEncoded[1] & 0xFF));

		startTime = mjdg(myDate);

		startTime.tm_hour = decimal(startTimeEncoded[2]);
		startTime.tm_min = decimal(startTimeEncoded[3]);
		startTime.tm_sec = decimal(startTimeEncoded[4]);
	}

	void EventInfo::setDuration(struct tm duration) {

		this->duration = duration;

		durationEncoded[0] = bcd(duration.tm_hour);
		durationEncoded[1] = bcd(duration.tm_min);
		durationEncoded[2] = bcd(duration.tm_sec);
	}

	void EventInfo::setDurationEncoded(char* duration) {
		memcpy(durationEncoded, duration, 3);

		this->duration.tm_hour = decimal(duration[0]);
		this->duration.tm_min = decimal(duration[1]);
		this->duration.tm_sec = decimal(duration[2]);
	}

	struct tm EventInfo::getDuration() {
		return duration;
	}

	char * EventInfo::getDurationEncoded() {
		return durationEncoded;
	}

	void EventInfo::setRunningStatus(unsigned char status) {
		runningStatus = status;
	}

	unsigned char EventInfo::getRunningStatus() {
		return runningStatus;
	}

	void EventInfo::setFreeCAMode(unsigned char mode) {
		freeCAMode = mode;
	}

	unsigned char EventInfo::getFreeCAMode() {
		return freeCAMode;
	}

	unsigned short EventInfo::getDescriptorsLoopLength() {
		return descriptorsLoopLength;
	}

	void EventInfo::setDescriptorsLoopLength(unsigned short length) {
		descriptorsLoopLength = length;
	}

	void EventInfo::insertDescriptor(Descriptor* info) {
		ShortEventDescriptor* se;
		set<Descriptor*>::iterator it;
		size_t count = 0;

		descriptors.insert(info);

		for (it = descriptors.begin(); it != descriptors.end(); it++) {
			switch ((*it)->getDescriptorTag()) {
			case 0x4D:
				se = (ShortEventDescriptor*) (*it);
				break;
			default:
				break;
			}
			count += (se->getDescriptorLength() + 2);
		}
		descriptorsLoopLength = count;
	}

	set<Descriptor*> * EventInfo::getDescriptors() {
		return &descriptors;
	}

}
}
}
}
}
}
