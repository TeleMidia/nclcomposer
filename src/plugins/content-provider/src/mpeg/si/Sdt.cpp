#include "../../../include/mpeg/si/Sdt.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
namespace sdt {
	ServiceDescriptionTable::ServiceDescriptionTable() : TSSection() {
		serviceInfo = new vector<ServiceInfo*>;
		encodedStream = NULL;
		sectionNumber = 1;
		CRC32 = 0xC1C2C3C4; //example
	}

	ServiceDescriptionTable::ServiceDescriptionTable(char buf[184]) : TSSection(buf) {
		serviceInfo = new vector<ServiceInfo*>;
		encodedStream = new vector<SectionInfo*>;
	}

	ServiceDescriptionTable::~ServiceDescriptionTable() {
		if (serviceInfo != NULL) {
			serviceInfo->clear();
			delete serviceInfo;
			serviceInfo = NULL;
		}

		if (encodedStream != NULL) {
			encodedStream->clear();
			delete encodedStream;
			encodedStream = NULL;
		}
	}

	void ServiceDescriptionTable::setOriginalNetworkId(unsigned short id) {
		originalNetworkId = id;
	}
	
	unsigned short ServiceDescriptionTable::getOriginalNetworkId() {
		return originalNetworkId;
	}
	
	unsigned short ServiceDescriptionTable::insertEvent(
			unsigned short serviceId,
			bool eitSchedule,
			bool eitPresentFollowing,
			unsigned char runningStatus,
			bool freeCAMode) {
		//create a new ServiceInfo data;
		ServiceInfo* srvi;
		srvi = new ServiceInfo();
	
		//get a new service id;
		srvi->setServiceId(serviceId);
	
		//fill all information in ServiceInfo;
		if (eitSchedule) {
			srvi->setEitScheduleFlag(1);
		} else {
			srvi->setEitScheduleFlag(0);
		}
		if (eitPresentFollowing) {
			srvi->setEitPresentFollowingFlag(1);
		} else {
			srvi->setEitPresentFollowingFlag(0);
		}
		if (freeCAMode) {
			srvi->setFreeCAMode(1);
		} else {
			srvi->setFreeCAMode(0);
		}
		srvi->setRunningStatus(runningStatus);
	
		//push back into vector;
		serviceInfo->push_back((ServiceInfo*) srvi);
	
		return srvi->getServiceId();
	}
	
	int ServiceDescriptionTable::removeEvent(unsigned short id) {
		ServiceInfo* srvi;
		vector<ServiceInfo*>::iterator i;
	
		i = serviceInfo->begin();
		while (i!=serviceInfo->end()) {
			srvi = *i;
			if (srvi->getServiceId() == id) {
				serviceInfo->erase(i);
				return 1;
			}
			i++;
		}
	
		return 0;
	}
	
	void ServiceDescriptionTable::encodeStream(bool append, string filename) {
		ServiceInfo* srvi;
		vector<ServiceInfo*>::iterator i;
		size_t pos = 0;
		unsigned int lastSectionNumber;
		bool createNewSection = true;
		set<Descriptor*> * descriptors;
		set<Descriptor*>::iterator it;
		LogoTransmissionDescriptor * lgd;
	
		vector<SectionInfo*>::iterator siit;
		vector<SectionInfo*>* sections;
		SectionInfo* si;
		TSPacket* tsp;
	
		//finding lastSectionNumber
		lastSectionNumber = 0;
		i = serviceInfo->begin();
		while (createNewSection) {
			createNewSection = false;
			pos = 11;
			while (i!=serviceInfo->end()) {
				srvi = *i;
				if ((pos + srvi->getSize()) <= (MAX_SECTION_SIZE)) {
					pos += srvi->getSize();
					i++;
				} else {
					createNewSection = true;
					lastSectionNumber++;
					pos = 0;
					break;
				}
			}
		}
	
		cout << "Encoding stream..." << endl;
	
		sections = new vector<SectionInfo*>;
	
		createNewSection = true;
		pos = 0;
		sectionNumber = 0;
	
		i = serviceInfo->begin();
	
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
	
			//encoding originalNetworkId
			section[pos] = (originalNetworkId & 0xFF00) >> 8;
			pos++;
			section[pos] = (originalNetworkId & 0xFF);
			pos++;
	
			//encoding reserved_future_use
			section[pos] = 0xFF;
			pos++;
	
			while (i!=serviceInfo->end()) {
				srvi = *i;
	
				if ((pos + srvi->getSize()) <= (MAX_SECTION_SIZE - 1)) {
	
					//encoding ServiceId
					section[pos] = (srvi->getServiceId() & 0xFF00) >> 8;
					pos++;
					section[pos] = (srvi->getServiceId() & 0xFF);
					pos++;
	
					//encoding EIT_schedule_flag and EIT_present_following_flag
					if (srvi->getEitScheduleFlag())
					{
						section[pos] = 0xFE;
					} else {
						section[pos] = 0xFC;
					}
					if (srvi->getEitPresentFollowingFlag())
					{
						section[pos] = section[pos] | 0x01;
					}
					pos += 1;
	
					//encoding RunningStatus, FreeCAMode and
					//DescriptorsLoopLength (parcial)
					section[pos] =
						(srvi->getDescriptorsLoopLength() & 0xFF00) >> 8;
					section[pos] = section[pos] | (srvi->getRunningStatus() << 5);
					section[pos] = section[pos] | (srvi->getFreeCAMode() << 4);
					pos++;
	
					//encoding DescriptorsLoopLength (parcial)
					section[pos] = (srvi->getDescriptorsLoopLength() & 0xFF);
					pos++;
	
					//encoding Descriptors
					descriptors = srvi->getDescriptors();
					it = descriptors->begin();
					while (it != descriptors->end()) {
						switch ((*it)->getDescriptorTag()) {
						case 0xCF: //Logo Transmission Descriptor
							lgd = (LogoTransmissionDescriptor*) *it;
							//encoding Descriptor Tag
							section[pos] = lgd->getDescriptorTag();
							pos++;
							//encoding Descriptor Length
							section[pos] = lgd->getDescriptorLength();
							pos++;
							//encoding logo transmission type
							section[pos] = lgd->getType();
							pos++;
							if (lgd->getType() == 0x01) { // scheme 1
								section[pos] = 0xFE | ((lgd->getLogoId() & 0xFF00) >> 8);
								pos++;
								section[pos] = (lgd->getLogoId() & 0xFF);
								pos++;
								section[pos] = 0xF0 | ((lgd->getLogoVersion() & 0xFF00) >> 8);
								pos++;
								section[pos] = (lgd->getLogoVersion() & 0xFF);
								pos++;
								section[pos] = ((lgd->getDownloadDataId() & 0xFF00) >> 8);
								pos++;
								section[pos] = (lgd->getDownloadDataId() & 0xFF);
								pos++;
							}
							else if (lgd->getType() == 0x02) { // scheme 2
								section[pos] = 0xFE | ((lgd->getLogoId() & 0xFF00) >> 8);
								pos++;
								section[pos] = (lgd->getLogoId() & 0xFF);
								pos++;
							}
							else if (lgd->getType() == 0x03) { // simple logo system
								string str;
								str = lgd->getName();
								memcpy(section+pos, str.c_str(), lgd->getNameLength());
								pos += lgd->getNameLength();
							}
							else {
								cout << "Unrecognized Logo Transmission Type: " << lgd->getType() << endl;
							}
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
		tsp->tsPacketFactory(append, 0x11, 0x01, 0x00, sections, filename);
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
	
	void ServiceDescriptionTable::decodeStream() {
		ServiceInfo* srvi;
		SectionInfo* si;
		vector<SectionInfo*>::iterator i;
		LogoTransmissionDescriptor* ltd;
		int firstTime = 1;
		size_t pos, remainingBytesDescriptor, value;
	
		cout << "Decoding stream..." << endl << endl;
	
		i = encodedStream->begin();
		while (i!=encodedStream->end()) {
			si = *i; //getting a section to decode...
	
			if (firstTime) {
				firstTime = 0;
	
				originalNetworkId = (((si->data[8] << 8) & 0xFF00) |
						(si->data[9] & 0xFF));
	
				cout << "OriginalNetworkId: " << originalNetworkId << endl;
				cout << endl;
	
				pos = 11; //it points to service_id
			}
	
			while ((pos+4) < si->length) {
				//there's at least one serviceinfo
				srvi = new ServiceInfo();
	
				srvi->setServiceId((((si->data[pos] << 8) & 0xFF00) |
						(si->data[pos+1] & 0xFF)));
				pos += 2;
				srvi->setEitScheduleFlag((si->data[pos] & 0x02) >> 1);
				srvi->setEitPresentFollowingFlag((si->data[pos] & 0x01));
				pos++;
				srvi->setRunningStatus((si->data[pos] & 0xE0) >> 5);
				srvi->setFreeCAMode((si->data[pos] & 0x10) >> 4);
				srvi->setDescriptorsLoopLength(
						(((si->data[pos] & 0x0F) << 8) & 0xFF00) |
						(si->data[pos+1] & 0xFF));
				pos += 2;
	
				cout << "ServiceId: " << srvi->getServiceId() << endl;
				cout << "EitSchecudeFlag: ";
				if (srvi->getEitScheduleFlag()) {
					cout << "Yes" << endl;
				} else {
					cout << "No" << endl;
				}
				cout << "EitPresentFollowingFlag: ";
				if (srvi->getEitPresentFollowingFlag()) {
					cout << "Yes" << endl;
				} else {
					cout << "No" << endl;
				}
				cout << "RunningStatus: ";
				if (srvi->getRunningStatus()) {
					cout << "Yes" << endl;
				} else {
					cout << "No" << endl;
				}
				cout << "FreeCAMode: ";
				if (srvi->getRunningStatus()) {
					cout << "Yes" << endl;
				} else {
					cout << "No" << endl;
				}
				cout << "DescriptorsLoopLength: "
					<< srvi->getDescriptorsLoopLength() << endl;
				cout << endl;
	
				remainingBytesDescriptor = srvi->getDescriptorsLoopLength();
				
				while (remainingBytesDescriptor) {
					//there's at least one descriptor
					value = (si->data[pos+1] + 2);
					remainingBytesDescriptor -= value;
					switch (si->data[pos]) {
					case 0xCF: //Logo Transmission Descriptor
						ltd = new LogoTransmissionDescriptor();
						pos++;
						ltd->setDescriptorLength(si->data[pos]);
						cout << "Descriptor length: " << (ltd->getDescriptorLength() & 0xFF) << endl;
						pos++;
						ltd->setType(si->data[pos]);
						cout << "Type: " << (ltd->getType() & 0xFF) << endl;
						pos++;
						if (si->data[pos-1] == 0x01) { // scheme 1
							ltd->setLogoId(((si->data[pos] << 8) & 0xFF00) |
												(si->data[pos+1] & 0xFF));
							pos += 2;
							ltd->setLogoVersion(((si->data[pos] << 8) &
										0x0100) | (si->data[pos+1] & 0xFF));
							pos += 2;
							ltd->setDownloadDataId(((si->data[pos] << 8) &
										0xFF00) | (si->data[pos+1] & 0xFF));
							pos += 2;
						}
						else if (si->data[pos-1] == 0x02) { // scheme 2
							ltd->setLogoId(((si->data[pos] << 8) & 0xFF00) |
												(si->data[pos+1] & 0xFF));
							pos += 2;
						}
						else if (si->data[pos-1] == 0x03) { // simple logo system
							char str[ltd->getDescriptorLength()];
							memcpy(str, si->data+pos, ltd->getDescriptorLength()-1);
							str[ltd->getDescriptorLength()-1] = 0;
							ltd->setName((string) str);
							pos += (ltd->getDescriptorLength()-1);
							cout << "Simple logo system: " << ltd->getName() << endl;
						}
						else {
							pos += (ltd->getDescriptorLength()-1);
							cout << "Unrecognized Logo Transmission Type: "
										<< ltd->getType() << endl;
						}
	
						srvi->insertDescriptor(ltd);
						break;
					default: //Unrecognized Descriptor
						pos += value;
						break;
					}
				}
	
				serviceInfo->push_back((ServiceInfo*) srvi);
			}
			i++;
		}
		cout << "Stream decoded successfully." << endl;
	}
	
	void ServiceDescriptionTable::storeSection(char buffer[4096], int slen) {
		//create a new EventInfo data;
		SectionInfo* si;
		si = new SectionInfo();
	
		memcpy(si->data, this->section, 4096);
		si->length = slen;
	
		encodedStream->push_back(si);
	}
	
	void ServiceDescriptionTable::print() {
	
	}
	
	int ServiceDescriptionTable::insertLogoTransmissionDescriptor(
			unsigned short serviceId,
			unsigned short type,
			unsigned short logoId,
			unsigned short logoVersion,
			unsigned short downloadDataId,
			string name) {
		LogoTransmissionDescriptor* ltd;
		ServiceInfo* srvi;
		vector<ServiceInfo*>::iterator i;
	
		i = serviceInfo->begin();
		while (i!=serviceInfo->end()) {
			srvi = *i;
			if (srvi->getServiceId() == serviceId) {
				//check if logo descriptor already exists...
				//
				//not exists...
				ltd = new LogoTransmissionDescriptor();
				ltd->setType(type);
				switch (type) {
				case 0x01: // scheme 1
					ltd->setLogoId(logoId);
					ltd->setLogoVersion(logoVersion);
					ltd->setDownloadDataId(downloadDataId);
					break;
				case 0x02: // scheme 2
					ltd->setLogoId(logoId);
					break;
				case 0x03: // simple logo system
					ltd->setName(name);
					break;
				default:
					cout << "Unrecognized Logo Transmission Type: "
								<< ltd->getType() << endl;
					break;
				}
				srvi->insertDescriptor(ltd);
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
