#include "../../../include/mpeg/si/Eit.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
namespace eit {
	EventInfoTable::EventInfoTable() : TSSection() {
		eventInfo = new vector<EventInfo*>;
		encodedStream = NULL;
		sectionNumber = 1;
		nextId = 0; //the first id will be 0x01.
		CRC32 = 0xC1C2C3C4; //example
	}

	EventInfoTable::EventInfoTable(char buf[184]) : TSSection(buf) {
		eventInfo = new vector<EventInfo*>;
		encodedStream = new vector<SectionInfo*>;
	}

	EventInfoTable::~EventInfoTable() {
		if (eventInfo != NULL) {
			eventInfo->clear();
			delete eventInfo;
			eventInfo = NULL;
		}

		if (encodedStream != NULL) {
			encodedStream->clear();
			delete encodedStream;
			encodedStream = NULL;
		}
	}

	unsigned short EventInfoTable::getNextId() {
		nextId++;
		return nextId;
	}

	void EventInfoTable::setTransportStreamId(unsigned short id) {
		transportStreamId = id;
	}

	unsigned short EventInfoTable::getTransportStreamId() {
		return transportStreamId;
	}

	void EventInfoTable::setOriginalNetworkId(unsigned short id) {
		originalNetworkId = id;
	}

	unsigned short EventInfoTable::getOriginalNetworkId() {
		return originalNetworkId;
	}

	void EventInfoTable::setSegmentLastSectionNumber(unsigned char number) {
		segmentLastSectionNumber = number;
	}

	unsigned char EventInfoTable::getSegmentLastSectionNumber() {
		return segmentLastSectionNumber;
	}

	void EventInfoTable::setLastTableId(unsigned char id) {
		lastTableId = id;
	}

	unsigned char EventInfoTable::getLastTableId() {
		return lastTableId;
	}

	unsigned short EventInfoTable::insertEvent(
			struct tm dateTime, struct tm duration) {
		//create a new EventInfo data;
		EventInfo* ei;
		ei = new EventInfo();

		//get a new event id;
		ei->setEventId(getNextId());

		//fill all information in EventInfo;
		ei->setStartTime(dateTime);
		ei->setDuration(duration);
		ei->setRunningStatus(0);
		ei->setFreeCAMode(0);

		//push back into vector;
		eventInfo->push_back((EventInfo*) ei);

		//TODO
		//all events should be chronological!!!

		return ei->getEventId();
	}

	int EventInfoTable::removeEvent(unsigned short id) {
		EventInfo* ei;
		vector<EventInfo*>::iterator i;

		i = eventInfo->begin();
		while (i!=eventInfo->end()) {
			ei = *i;
			if (ei->getEventId() == id) {
				eventInfo->erase(i);
				return 1;
			}
			i++;
		}

		return 0;
	}

	void EventInfoTable::encodeStream(bool append, string filename) {
		EventInfo* ei;
		vector<EventInfo*>::iterator i;
		size_t pos = 0;
		unsigned int lastSectionNumber;
		bool createNewSection = true;
		set<Descriptor*> * descriptors;
		set<Descriptor*>::iterator it;
		ShortEventDescriptor * sed;

		vector<SectionInfo*>::iterator siit;
		vector<SectionInfo*>* sections;
		SectionInfo* si;
		TSPacket* tsp;

		//TODO
		//The descriptorsLoopLength and sectionLength have 12 bits!

		//finding lastSectionNumber
		lastSectionNumber = 0;
		i = eventInfo->begin();
		while (createNewSection) {
			createNewSection = false;
			pos = 14;
			while (i!=eventInfo->end()) {
				ei = *i;
				if ((pos + ei->getSize()) <= (MAX_SECTION_SIZE)) {
					pos += ei->getSize();
					i++;
				} else {
					createNewSection = true;
					lastSectionNumber++;
					pos = 0;
					break;
				}
			}
		}

		//TODO: check how segmentLastSectionNumber works.
		segmentLastSectionNumber = lastSectionNumber;

		cout << "Encoding stream..." << endl;
		
		sections = new vector<SectionInfo*>;

		createNewSection = true;
		pos = 0;
		sectionNumber = 0;

		i = eventInfo->begin();

		while (createNewSection) {

			createNewSection = false;
			memset(section, 0, MAX_SECTION_SIZE);

			//encoding tableId
			section[pos] = tableId;
			pos++;

			//encoding sectionSyntaxIndicator
			if (sectionSyntaxIndicator)
			{
				section[pos] = (0x0F << 4);
			} else {
				section[pos] = (0x07 << 5);
			}
			pos += 2;

			//encoding idExtension
			section[pos] = (idExtention & 0xFF00) >> 8;
			pos++;
			section[pos] = (idExtention & 0xFF);
			pos++;

			//encoding versionNumber and currentNextIndicator
			section[pos] = (0xC0 | (versionNumber << 1));
			section[pos] = (section[pos] | 0x01);
			pos++;

			//encoding sectionNumber
			section[pos] = sectionNumber;
			sectionNumber++;
			pos++;

			//encoding lastSectionNumber
			section[pos] = lastSectionNumber;
			pos++;

			//encoding transportStreamId
			section[pos] = (transportStreamId & 0xFF00) >> 8;
			pos++;
			section[pos] = (transportStreamId & 0xFF);
			pos++;

			//encoding originalNetworkId
			section[pos] = (originalNetworkId & 0xFF00) >> 8;
			pos++;
			section[pos] = (originalNetworkId & 0xFF);
			pos++;

			//TODO what is it?
			//encoding segmentLastSectionNumber
			section[pos] = segmentLastSectionNumber;
			pos++;

			//TODO: what is it?
			//encoding lastTableId
			section[pos] = lastTableId;
			pos++;

			while (i!=eventInfo->end()) {
				ei = *i;

				if ((pos + ei->getSize()) <= (MAX_SECTION_SIZE - 1)) {

					//encoding EventId
					section[pos] = (ei->getEventId() & 0xFF00) >> 8;
					pos++;
					section[pos] = (ei->getEventId() & 0xFF);
					pos++;

					//encoding StartTime
					memcpy(section+pos, ei->getStartTimeEncoded(), 5);
					pos += 5;

					//encoding Duration
					memcpy(section+pos, ei->getDurationEncoded(), 3);
					pos += 3;

					//encoding RunningStatus, FreeCAMode and
					//DescriptorsLoopLength (parcial)
					section[pos] =
						(ei->getDescriptorsLoopLength() & 0xFF00) >> 8;
					section[pos] = section[pos] | (ei->getRunningStatus() << 5);
					section[pos] = section[pos] | (ei->getFreeCAMode() << 4);
					pos++;

					//encoding DescriptorsLoopLength (parcial)
					section[pos] = (ei->getDescriptorsLoopLength() & 0xFF);
					pos++;

					//encoding Descriptors
					descriptors = ei->getDescriptors();
					it = descriptors->begin();
					while (it != descriptors->end()) {
						switch ((*it)->getDescriptorTag()) {
						case 0x4D: //Short Event Descriptor
							sed = (ShortEventDescriptor*) *it;
							//encoding Descriptor Tag
							section[pos] = sed->getDescriptorTag();
							pos++;
							//encoding Descriptor Length
							section[pos] = sed->getDescriptorLength();
							pos++;
							//encoding Language Code
							memcpy(section+pos, sed->getLanguageCode(), 3);
							pos += 3;
							//encoding Event Name Length
							section[pos] = sed->getEventNameLength();
							pos++;
							//encoding Event Name
							memcpy(section+pos,
									sed->getEventName(),
									sed->getEventNameLength());
							pos += sed->getEventNameLength();
							//encoding Description Length
							section[pos] = sed->getDescriptionLength();
							pos++;
							//encoding Description
							memcpy(section+pos,
									sed->getDescription(),
									sed->getDescriptionLength());
							pos += sed->getDescriptionLength();
							break;
						default:
							break;
						}
						it++;
					}
					i++;
				} else {
					// new section required
					createNewSection = true;

					//Encoding CRC_32
					section[pos] = (CRC32 & 0xFF000000) >> 24;
					pos++;
					section[pos] = (CRC32 & 0xFF0000) >> 16;
					pos++;
					section[pos] = (CRC32 & 0xFF00) >> 8;
					pos++;
					section[pos] = (CRC32 & 0xFF);

					//Section length
					section[1] = (section[1] | (((pos-2) >> 8) & 0xFF));
					section[2] = (section[2] | (pos-2));

					si = new SectionInfo();
					memcpy(si->data, this->section, (pos+1));
					si->length = (pos+1);
					sections->push_back(si);

					pos = 0;
					break;
				}
			}
		}

		//Encoding CRC_32
		section[pos] = (CRC32 & 0xFF000000) >> 24;
		pos++;
		section[pos] = (CRC32 & 0xFF0000) >> 16;
		pos++;
		section[pos] = (CRC32 & 0xFF00) >> 8;
		pos++;
		section[pos] = (CRC32 & 0xFF);

		//Section length
		section[1] = (section[1] | (((pos-2) >> 8) & 0xFF));
		section[2] = (section[2] | (pos-2));

		//Current next indicator is false
		section[5] = (section[5] & 0xFE);

		si = new SectionInfo();
		memcpy(si->data, this->section, (pos+1));
		si->length = (pos+1);
		sections->push_back(si);

		tsp = new TSPacket();
		tsp->tsPacketFactory(append, 0x12, 0x01, 0x00, sections, filename);
		delete tsp;

		siit = sections->begin();
		while (siit!=sections->end()) {
			si = *siit;
			delete si;
			siit++;
		}
		sections->clear();
		delete sections;

	}

	void EventInfoTable::decodeStream() {
		EventInfo* ei;
		SectionInfo* si;
		vector<SectionInfo*>::iterator i;
		ShortEventDescriptor* se;
		int firstTime = 1, ids;
		struct tm time;
		size_t pos, remainingBytesDescriptor, value;
		char str[256];

		cout << "Decoding stream..." << endl << endl;

		i = encodedStream->begin();
		while (i!=encodedStream->end()) {
			si = *i; //getting a section to decode...

			if (firstTime) {
				firstTime = 0;

				transportStreamId = (((si->data[8] << 8) & 0xFF00) |
						(si->data[9] & 0xFF));
				originalNetworkId = (((si->data[10] << 8) & 0xFF00) |
						(si->data[11] & 0xFF));
				segmentLastSectionNumber = si->data[12];
				lastTableId = si->data[13];

				cout << "TransportStreamId: " << transportStreamId << endl;
				cout << "OriginalNetworkId: " << originalNetworkId << endl;
				ids = segmentLastSectionNumber;
				cout << "SegmentLastSectionNumber: " << dec << ids << endl;
				ids = lastTableId;
				cout << "LastTableId: " << dec << ids << endl;
				cout << endl;

				pos = 14;
			}

			while ((pos+4) < si->length) {
				//there's at least one eventinfo
				ei = new EventInfo();

				ei->setEventId((((si->data[pos] << 8) & 0xFF00) |
						(si->data[pos+1] & 0xFF)));
				pos += 2;
				ei->setStartTimeEncoded(si->data+pos);
				pos += 5;
				ei->setDurationEncoded(si->data+pos);
				pos += 3;
				ei->setRunningStatus((si->data[pos] & 0xE0) >> 5);
				ei->setFreeCAMode((si->data[pos] & 0x10) >> 4);
				ei->setDescriptorsLoopLength(
						(((si->data[pos] & 0x0F) << 8) & 0xFF00) |
						(si->data[pos+1] & 0xFF));
				pos += 2;

				cout << "EventId: " << ei->getEventId() << endl;
				time = ei->getStartTime();
				cout << "StartTime: " << endl;
				time = ei->getDuration();
				cout << "Duration: " << endl;
				cout << "RunningStatus: ";
				if (ei->getRunningStatus()) {
					cout << "Yes" << endl;
				} else {
					cout << "No" << endl;
				}
				cout << "FreeCAMode: ";
				if (ei->getRunningStatus()) {
					cout << "Yes" << endl;
				} else {
					cout << "No" << endl;
				}
				cout << "DescriptorsLoopLength: "
					<< ei->getDescriptorsLoopLength() << endl;
				cout << endl;

				remainingBytesDescriptor = ei->getDescriptorsLoopLength();
				
				while (remainingBytesDescriptor) {
					//there's at least one descriptor
					value = ((si->data[pos+1] & 0xFF) + 2);
					remainingBytesDescriptor -= value;
					switch (si->data[pos]) {
					case 0x4D: //Short Event Descriptor
						se = new ShortEventDescriptor();
						se->setDescriptorLength(si->data[pos+1]);
						pos += 2;
						se->setLanguageCode(si->data+pos);
						pos += 3;
						se->setEventName(si->data+pos+1, si->data[pos]);
						pos += (se->getEventNameLength()+1);
						se->setDescription(si->data+pos+1, si->data[pos]);
						pos += (se->getDescriptionLength() + 1);

						cout << "Descriptor: Short Event Descriptor" << endl;
						memcpy(str, se->getLanguageCode(), 3); str[3] = 0;
						cout << "LanguageCode: " << str << endl;
						memcpy(str, se->getEventName(),
								se->getEventNameLength());
						str[se->getEventNameLength()] = 0;
						cout << "EventName: " << str << endl;
						memcpy(str, se->getDescription(),
								se->getDescriptionLength());
						str[se->getDescriptionLength()] = 0;
						cout << "Description: " << str << endl;
						cout << endl;

						ei->insertDescriptor(se);
						break;
					default: //Unrecognized Descriptor
						pos += value;
						break;
					}
				}

				eventInfo->push_back((EventInfo*) ei);
			}
			i++;
		}
		cout << "Stream decoded successfully." << endl;
	}

	void EventInfoTable::storeSection() {
		//create a new EventInfo data;
		SectionInfo* si;
		si = new SectionInfo();

		memcpy(si->data, this->section, (sectionLength + 3));
		si->length = (sectionLength + 3);

		encodedStream->push_back(si);
	}

	void EventInfoTable::print() {
		EventInfo* ei;
		vector<EventInfo*>::iterator i;

		cout << "EIT TABLE " << endl;
		cout << "transport stream id = " << getTransportStreamId() << endl;
		cout << "original network id = " << getOriginalNetworkId() << endl;
		cout << "segment last section number = "
			<< getSegmentLastSectionNumber() << endl;
		cout << "last table id = " << getLastTableId() << endl;

		i = eventInfo->begin();
		while (i!=eventInfo->end()) {
			ei = *i;
			//cout << "EIT TABLE " <<  << endl;
			//TODO
			i++;
		}
	}

	int EventInfoTable::insertShortEventDescriptor(
			unsigned short id,
			string language,
			string name,
			string description) {
		ShortEventDescriptor* se;
		EventInfo* ei;
		vector<EventInfo*>::iterator i;

		i = eventInfo->begin();
		while (i!=eventInfo->end()) {
			ei = *i;
			if (ei->getEventId() == id) {
				//check if it already exists...
				//
				//not exists...
				se = new ShortEventDescriptor();
				se->setLanguageCode(language);
				se->setEventName(name, name.length());
				se->setDescription(description, description.length());
				ei->insertDescriptor(se);
				return 1;
			}
			i++;
		}
		return 0;
	}
}
}
}
}
}
}
