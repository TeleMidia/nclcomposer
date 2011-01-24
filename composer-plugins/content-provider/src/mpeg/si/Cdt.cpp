#include "../../../include/mpeg/si/Cdt.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
namespace cdt {
	CommonDataTable::CommonDataTable() : TSSection() {
		descriptorsLoopLength = 0;
		encodedStream = NULL;
		sectionNumber = 1;
		pngData = NULL;
		pngLength = 0;
		CRC32 = 0xC1C2C3C4; //example
	}

	CommonDataTable::CommonDataTable(char buf[184]) : TSSection(buf) {
		descriptorsLoopLength = 0;
		encodedStream = new vector<SectionInfo*>;
		pngData = NULL;
		pngLength = 0;
	}

	CommonDataTable::~CommonDataTable() {
		if (pngData != NULL) {
			free(pngData);
		}
		if (encodedStream != NULL) {
			encodedStream->clear();
			delete encodedStream;
			encodedStream = NULL;
		}
	}

	void CommonDataTable::setOriginalNetworkId(unsigned short id) {
		originalNetworkId = id;
	}

	unsigned short CommonDataTable::getOriginalNetworkId() {
		return originalNetworkId;
	}
	
	void CommonDataTable::setDataType(unsigned char type) {
		dataType = type;
	}

	unsigned char CommonDataTable::getDataType() {
		return dataType;
	}

	int CommonDataTable::loadPNG(string path) {
		FILE* png;
		size_t rc;
		png = fopen(path.c_str(), "rb");
		if (png == NULL)
		{
			cout << "cannot open PNG file. (" << path << ")" << endl;
			return 1;
		}
		
		if (fseek(png, 0, SEEK_END)) {
			cout << "Seek error." << endl;
			fclose(png);
			return 2;
		}
		else {
			pngLength = ftell(png);
			fseek(png, 0, SEEK_SET);
			if (pngData != NULL) {
				free(pngData);
			}
			pngData = (char*) malloc(pngLength);
			if (pngData == NULL) {
				cout << "Memory error." << endl;
				return 3;
			}
			rc = fread(pngData, 1, pngLength, png);
			if (rc != pngLength) {
				cout << "Reading error." << endl;
				return 4;
			}
		}
		fclose(png);
		return 0;
	}

	int CommonDataTable::savePNG(string path) {
		FILE* png;
		size_t wc;
		
		if (pngData == NULL) {
			cout << "No data to save." << endl;
			return 1;
		}
		
		png = fopen(path.c_str(), "wb");
		if (png == NULL)
		{
			cout << "cannot open PNG file. (" << path << ")" << endl;
			return 2;
		}
		
		wc = fwrite(pngData, 1, pngLength, png);
		if (wc != pngLength) {
			cout << "Writing error." << endl;
			return 3;
		}
		return 0;
	}

	void CommonDataTable::encodeStream(bool append, string filename) {
		size_t pos, pngLeft, remainingTs, pngPos = 0;
		unsigned int lastSectionNumber;
		bool createNewSection = true;
		vector<SectionInfo*>::iterator it;
		vector<SectionInfo*>* sections;
		SectionInfo* si;
		TSPacket* tsp;

		//calculating descriptorsLoopLength
			//considering descriptors always empty (descriptorsLoopLength = 0)
		//finding lastSectionNumber
		lastSectionNumber = (pngLength / (MAX_SECTION_SIZE - (17 /* + descriptorsLoopLength*/)));
		if ((pngLength % (MAX_SECTION_SIZE - (17 /* + descriptorsLoopLength*/))) > 0) {
			lastSectionNumber++;
		}
		
		cout << "Encoding stream..." << endl;
		
		sections = new vector<SectionInfo*>;

		createNewSection = true;
		pos = 0;
		sectionNumber = 0;
		lastSectionNumber--;

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

			//encoding idExtension (download_data_id)
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
			
			//encoding dataType
			section[pos] = dataType;
			pos++;

			//encoding reserved_future_use and DescriptorsLoopLength (parcial)
			section[pos] = (descriptorsLoopLength & 0xFF00) >> 8;
			section[pos] = section[pos] | 0xF0;
			pos++;

			//encoding DescriptorsLoopLength (parcial)
			section[pos] = (descriptorsLoopLength & 0xFF);
			pos++;

			//encoding descriptors
			
			//encoding PNG
			remainingTs = (MAX_SECTION_SIZE - 4) - pos;
			pngLeft = (pngLength - pngPos);
			if (pngLeft > remainingTs) {
				memcpy(section+pos, pngData+pngPos, remainingTs);
				pos += remainingTs;
				pngPos += remainingTs;

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
			}
			else {
				memcpy(section+pos, pngData+pngPos, pngLeft);
				pos += pngLeft;
				pngPos += pngLeft;
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
		tsp->tsPacketFactory(append, 0x29, 0x01, 0x00, sections, filename);
		delete tsp;
		
		it = sections->begin();
		while (it!=sections->end()) {
			si = *it;
			delete si;
			it++;
		}
		sections->clear();
		delete sections;
	}

	void CommonDataTable::decodeStream() {
		SectionInfo* si;
		vector<SectionInfo*>::iterator i;
		int firstTime = 1;
		size_t pos, dll = 0, pngLeft, pngPos = 0, pngLen = 0;

		cout << "Decoding stream..." << endl << endl;
		
		i = encodedStream->begin();
		while (i!=encodedStream->end()) {
			si = *i;
			dll = ((((si->data[11] & 0x0F) << 8) & 0xFF00) | (si->data[12] & 0xFF));
			pngLen += ((si->length) - (dll + 17));
			i++;
		}

		i = encodedStream->begin();
		while (i!=encodedStream->end()) {
			si = *i; //getting a section to decode...

			if (firstTime) {
				firstTime = 0;

				originalNetworkId = (((si->data[8] << 8) & 0xFF00) |
						(si->data[9] & 0xFF));
				descriptorsLoopLength = ((((si->data[11] & 0x0F) << 8) & 0xFF00) |
										(si->data[12] & 0xFF));

				cout << "OriginalNetworkId: " << originalNetworkId << endl;
					cout << endl;
				cout << "descriptorLoopLength: " << descriptorsLoopLength << endl;
					cout << endl;
				
				if (pngData != NULL) {
					free(pngData);
				}
				pngData = (char*) malloc(pngLen);
				if (pngData == NULL) {
					cout << "Memory error." << endl;
					return;
				}
			}
			pos = 13; //it points to first descriptor if exists
			pos += descriptorsLoopLength;

			while ((pos+4) < si->length) {
				//there's at least 1 byte (logoData)
				pngLeft = ((si->length) - (descriptorsLoopLength + 17));
				memcpy(pngData+pngPos, si->data+pos, pngLeft);
				pos += pngLeft;
				pngPos += pngLeft;
			}
			i++;
		}
		pngLength = pngPos;
		savePNG("h:\\nbc_saved.png");
		cout << "Stream decoded successfully." << endl;
	}

	void CommonDataTable::storeSection() {
		//create a new EventInfo data;
		SectionInfo* si;
		si = new SectionInfo();

		memcpy(si->data, this->section, (sectionLength + 3));
		si->length = (sectionLength + 3);

		encodedStream->push_back(si);
	}

	void CommonDataTable::print() {

	}
}
}
}
}
}
}
