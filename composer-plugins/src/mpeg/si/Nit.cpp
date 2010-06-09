#include "../../../include/mpeg/si/Nit.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
namespace nit {

	NetworkInformationTable::NetworkInformationTable() : TSSection() {
		descriptors = new vector<Descriptor*>;
		encodedStream = new vector<SectionInfo*>;;
		networkDescriptorsLength = 0;
		CRC32 = 0xC1C2C3C4; //example
	}

	NetworkInformationTable::~NetworkInformationTable() {
		if (encodedStream != NULL) {
			encodedStream->clear();
			delete encodedStream;
			encodedStream = NULL;
		}
	}

	void NetworkInformationTable::insertSection(char buf[4096]) {
		SectionInfo* si;
		si = new SectionInfo;
		memcpy(si->data, buf, 4096);
		si->length = (unsigned short) buf[2]; // ateh 255;
		encodedStream->push_back(si);
	}

	bool NetworkInformationTable::insertNetworkNameDescriptor(string name) {
		NetworkNameDescriptor* nnd;
		nnd = (NetworkNameDescriptor*) Descriptor::getDescriptor(descriptors, 0x40);
		if (nnd == NULL) {
			nnd = new NetworkNameDescriptor();
			if (nnd == NULL)
			{
				return false;
			}
			descriptors->push_back((NetworkNameDescriptor*) nnd);
		}
		nnd->setName(name);
		networkDescriptorsLength = 
			Descriptor::getDescriptorsLength(descriptors);
		return true;
	}

	bool NetworkInformationTable::insertServiceListDescriptor(
			unsigned short serviceId, unsigned char serviceType) {
		ServiceListDescriptor* sld;
		sld =
		  (ServiceListDescriptor*) Descriptor::getDescriptor(descriptors, 0x41);
		if (sld == NULL) {
			sld = new ServiceListDescriptor();
			if (sld == NULL)
			{
				return false;
			}
			descriptors->push_back((ServiceListDescriptor*) sld);
		}
		sld->insertService(serviceId, serviceType);
		networkDescriptorsLength = 
					Descriptor::getDescriptorsLength(descriptors);
		return true;
	}

	void NetworkInformationTable::encodeStream(bool append, string filename) {
		size_t pos = 0;
		unsigned int lastSectionNumber;
		bool createNewSection = true;
		vector<Descriptor*>::iterator it;
		ServiceListDescriptor* sld;
		NetworkNameDescriptor* nnd;

		vector<SectionInfo*>::iterator siit;
		vector<SectionInfo*>* sections;
		SectionInfo* si;
		TSPacket* tsp;

		cout << "Encoding NIT..." << endl;
	
		sections = new vector<SectionInfo*>;

		createNewSection = true;
		pos = 0;
		sectionNumber = 0;

		while (createNewSection) {

			createNewSection = false;
			memset(section, 0, MAX_SECTION_SIZE);

			//encoding tableId
			section[pos] = tableId;
			pos++;

			//encoding sectionSyntaxIndicator
			if (sectionSyntaxIndicator)
			{
				section[pos] = 0xF0;
			} else {
				section[pos] = 0x70;
			}
			pos += 2;

			//encoding idExtension (network_id)
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

			//encoding reserved_future_use and
			//DescriptorsLength (parcial)
			section[pos] = (networkDescriptorsLength | 0xF000) >> 8;
			pos++;

			//encoding DescriptorsLength (parcial)
			section[pos] = (networkDescriptorsLength & 0xFF);
			pos++;

			//encoding Descriptors
			it = descriptors->begin();
			while (it != descriptors->end()) {
				switch ((*it)->getDescriptorTag()) {
				case 0x40: //Network Name Descriptor
					nnd = (NetworkNameDescriptor*) *it;
					pos = pos + nnd->encodeNetworkName(section+pos);
					break;
				case 0x41: //Service List Descriptor
					sld = (ServiceListDescriptor*) *it;
					pos = pos + sld->encodeServiceList(section+pos);
					break;
				default:
					break;
				}
				it++;
			}
		}

		//Encoding transport stream loop length
		// TODO: Implement transport_stream_loop
		section[pos] = 0xF0;
		pos++;
		section[pos] = 0x00;
		pos++;

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
		tsp->tsPacketFactory(append, 0x10, 0x01, 0x00, sections, filename);
		delete tsp;

		siit = sections->begin();
		while (siit!=sections->end()) {
			si = *siit;
			delete si;
			siit++;
		}
		sections->clear();
		delete sections;
		cout << "NIT encoded." << endl;
	}

	void NetworkInformationTable::decodeStream() {
		SectionInfo* si;
		vector<SectionInfo*>::iterator i;
		ServiceListDescriptor* sld;
		NetworkNameDescriptor* nnd;
		size_t pos, remainingBytesDescriptor, value;

		cout << "Decoding NIT..." << endl << endl;

		i = encodedStream->begin();
		while (i != encodedStream->end()) {
			si = *i; //getting a section to decode...
			pos = 8; //network_descriptors_length
	
			remainingBytesDescriptor = (((si->data[pos] << 8) & 0x0F00) |
					(si->data[pos+1] & 0xFF));
			pos += 2;

			while (remainingBytesDescriptor) {
				//there's at least one descriptor
				string str;
				value = (si->data[pos+1] + 2);
				remainingBytesDescriptor -= value; //consume this descriptor
				switch (si->data[pos]) {
				case 0x40: //Network Name Descriptor
					nnd = new NetworkNameDescriptor();
					pos++;
					nnd->setDescriptorLength(si->data[pos]);
					pos++;
					str.assign(si->data+pos, nnd->getDescriptorLength());
					pos += nnd->getDescriptorLength();
					nnd->setName(str);
					descriptors->push_back(nnd);
					break;
				case 0x41: //Service List Descriptor
					int nitems, idx;
					unsigned short serviceId;
					unsigned char serviceType;
					sld = new ServiceListDescriptor();
					pos++;
					sld->setDescriptorLength(si->data[pos]);
					pos++;
					nitems = (sld->getDescriptorLength() / 3);
					for (idx = 0; idx < nitems; idx++) {
						serviceId = (((si->data[pos] << 8) & 0xFF00) |
								(si->data[pos+1] & 0xFF));
						pos += 2;
						serviceType = (si->data[pos] & 0xFF); 
						pos++;
						sld->insertService(serviceId, serviceType);
					}
					descriptors->push_back(sld);
					break;
				default: //Unrecognized Descriptor
					pos += value;
					break;
				}
			}
			// TODO: Decode Transport Stream Loop.
			i++;
		}
		cout << "NIT decoded." << endl;
	}

}
}
}
}
}
}
