#include "../../include/mpeg/TSSection.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
	TSSection::TSSection(char streamData[184]) {
		currentSize = 0;
		memset(section, 0, sizeof(section));
		process(streamData);
	}

	TSSection::TSSection() {
		currentSize = 0;
	}

	TSSection::~TSSection() {
		
	}

	void TSSection::process(char streamData[184]) {
		if (currentSize == 0) {
			processHeader(streamData);

		} else {
			addPayloadData(streamData);
		}
	}

	void TSSection::processHeader(char streamData[184]) {
		tableId = (streamData[0] & 0xFF);
		sectionSyntaxIndicator = (streamData[1] & 0x80) >> 7;

		if (tableId == 0x00 || tableId == 0x02) {
			sectionLength = (((streamData[1] & 0x03) << 8) |
			(streamData[2] & 0xFF));

		} else {
			sectionLength = (((streamData[1] & 0x0F) << 8) |
			(streamData[2] & 0xFF));
		}

		idExtention = (((streamData[3] & 0xFF) << 8) | (streamData[4] & 0xFF));
		versionNumber = (streamData[5] & 0x3E) >> 1;
		currentNextIndicator = (streamData[5] & 0x01);
		sectionNumber = (streamData[6] & 0xFF);
		lastSectionNumber = (streamData[7] & 0xFF);

		//TODO: CRC32 and checksum

		/*// PAT
		if (tableId == 0x00) {
		// CAT
		} else if (tableId == 0x01) {
			//TODO: do we need CAT?

		// PMT
		} else if (tableId == 0x02) {
			//TODO

		// DSM-CC
		} else if ((tableId >= 0x38) && (tableId <= 0x3F)) {

		// EIT
		} else if ((tableId >= 0x4E) && (tableId <= 0x6F)) {

		Required tables: PAT, CAT, PMT, NIT, SDT, EIT and TOT.
		*/

		if (184 >= (sectionLength + 3)) {
			memcpy((void*)&(section[0]),
			(void*)(&streamData[0]), sectionLength + 3);

			currentSize = sectionLength + 3;

		} else {
			memcpy((void*)&(section[0]), (void*)(&streamData[0]), 184);
			currentSize = 184;
		}
	}

	void TSSection::addPayloadData(char streamData[184]) {
		if (currentSize + 184 >= (sectionLength + 3)) {
			memcpy((void*)&(section[currentSize]),
			(void*)(&streamData[0]),
			(sectionLength + 3 - currentSize));

			currentSize = sectionLength + 3;

		} else {
			memcpy((void*)&(section[currentSize]),
			(void*)(&streamData[0]), 184);

			currentSize += 184;
		}
	}

	unsigned int TSSection::getTableId() {
		return tableId;
	}

	void TSSection::setTableId(unsigned int id) {
		tableId = id;
	}

	bool TSSection::getSectionSyntaxIndicator() {
		return sectionSyntaxIndicator;
	}

	void TSSection::setSectionSyntaxIndicator(bool indication) {
		sectionSyntaxIndicator = indication;
	}

	unsigned int TSSection::getSectionLength() {
		return sectionLength;
	}

	unsigned int TSSection::getExtensionId() {
		return idExtention;
	}
	
	void TSSection::setExtensionId(unsigned int id) {
		idExtention = id;
	}

	unsigned int TSSection::getVersionNumber() {
		return versionNumber;
	}

	bool TSSection::getCurrentNextIndicator() {
		return currentNextIndicator;
	}

	unsigned int TSSection::getSectionNumber() {
		return sectionNumber;
	}

	unsigned int TSSection::getLastSectionNumber() {
		return lastSectionNumber;
	}

	void* TSSection::getPayload() {
		return (void*)&section[8];
	}

	unsigned int TSSection::getPayloadSize() {
		if ((sectionLength - 9) != (currentSize - 12)) {
			cout << "Warning! Strange section size. CurrentSize = "
			<< currentSize << ", sectionLength = " << sectionLength
			<< endl;

			return 0;
		}

		return (currentSize - 12);
	}

	void TSSection::setVersionNumber(unsigned int versionNumber) {
		this->versionNumber = versionNumber;
	}

	bool TSSection::isConsolidated() {
		if (currentSize == 0) {
			return false;
		}
		return ((sectionLength + 3) <= currentSize);
	}

	void TSSection::print() {
		unsigned int i;
		cout << "TS SECTION " << endl;
		cout << "tableid = " << getTableId() << endl;
		cout << "syntax indicator = " << getSectionSyntaxIndicator() << endl;
		cout << "section length = " << getSectionLength() << endl;
		cout << "extendion Id = " << getExtensionId() << endl;
		cout << "section version = " << getVersionNumber() << endl;
		cout << "current next indicator = " << getCurrentNextIndicator() << endl;
		cout << "section number = " << getSectionNumber() << endl;
		cout << "last secion number = " << getLastSectionNumber() << endl;

		char payload[currentSize - 12];
		memcpy(&payload[0], getPayload(), currentSize - 12);

		for (i=0; i < (currentSize - 12); i++) {
			cout << hex << (payload[i] & 0xFF) << " ";
		}
		cout << endl << endl;
	}
	
	char * TSSection::getSection() {
		return section;
	}
}
}
}
}
