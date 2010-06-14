#include "../../../include/mpeg/si/Pmt.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
	const short Pmt::STREAM_TYPE_ISO_RESERVED = 0x00;
	const short Pmt::STREAM_TYPE_VIDEO_MPEG1 = 0x01;
	const short Pmt::STREAM_TYPE_VIDEO_MPEG2 = 0x02;
	const short Pmt::STREAM_TYPE_AUDIO_MPEG1 = 0x03;
	const short Pmt::STREAM_TYPE_AUDIO_MPEG2 = 0x04;
	const short Pmt::STREAM_TYPE_PRIVATE_SECTION = 0x05;
	const short Pmt::STREAM_TYPE_PRIVATE_DATA = 0x06;
	const short Pmt::STREAM_TYPE_MHEG = 0x07;
	const short Pmt::STREAM_TYPE_MPEG2_DSMCC = 0x08;
	const short Pmt::STREAM_TYPE_H222_1 = 0x09;
	const short Pmt::STREAM_TYPE_DSMCC_TYPE_A = 0x0A;
	const short Pmt::STREAM_TYPE_DSMCC_TYPE_B = 0x0B;
	const short Pmt::STREAM_TYPE_DSMCC_TYPE_C = 0x0C;
	const short Pmt::STREAM_TYPE_DSMCC_TYPE_D = 0x0D;
	const short Pmt::STREAM_TYPE_MPEG2_AUX = 0x0E;
	const short Pmt::STREAM_TYPE_AUDIO_AAC = 0x0F;
	const short Pmt::STREAM_TYPE_VIDEO_MPEG4 = 0x10;
	const short Pmt::STREAM_TYPE_AUDIO_MPEG4 = 0x11;
	const short Pmt::STREAM_TYPE_SL_PES = 0x12;
	const short Pmt::STREAM_TYPE_SL_SECTION = 0x13;
	const short Pmt::STREAM_TYPE_DSMCC_SYNCH = 0x14;
	const short Pmt::STREAM_TYPE_META_PES = 0x15;
	const short Pmt::STREAM_TYPE_META_SEC = 0x16;
	const short Pmt::STREAM_TYPE_META_DATA_CAROUSEL = 0x17;
	const short Pmt::STREAM_TYPE_META_OBJECT_CAROUSEL = 0x18;
	const short Pmt::STREAM_TYPE_META_DSMCC_SYNCH = 0x19;
	const short Pmt::STREAM_TYPE_MPEG2_IPMP_STREAM = 0x1A;
	const short Pmt::STREAM_TYPE_VIDEO_H264 = 0x1B;
	const short Pmt::STREAM_TYPE_IPMP_STREAM = 0x7F;
	const short Pmt::STREAM_TYPE_AUDIO_AC3 = 0x81;
	const short Pmt::STREAM_TYPE_AUDIO_DTS = 0x8A;

	Pmt::Pmt(unsigned int pid, unsigned int programNumber) : TSSection() {
		streams = new map<unsigned int, short>;
		streamTypeNames = new map<short, string>;
		carousels = new map<unsigned int, CarouselInfo*>;
		ses = new map<unsigned int, SEInfo*>;
		pcrValues = NULL;
		pcrIndex = NULL;
		pcrRates = NULL;
		//aits = new map<unsigned int, AITInfo*>;

		(*streamTypeNames)[STREAM_TYPE_ISO_RESERVED] = "ISO/IEC Reserved";
		(*streamTypeNames)[STREAM_TYPE_VIDEO_MPEG1] = "ISO/IEC 11172-2 Video";
		(*streamTypeNames)[STREAM_TYPE_VIDEO_MPEG2] = "ISO/IEC 13818-2 Video";
		(*streamTypeNames)[STREAM_TYPE_AUDIO_MPEG1] = "ISO/IEC 11172-3 Audio";
		(*streamTypeNames)[STREAM_TYPE_AUDIO_MPEG2] = "ISO/IEC 13818-3 Audio";
		(*streamTypeNames)[STREAM_TYPE_PRIVATE_SECTION] =
			    "ISO/IEC 13818 Private Sections";

		(*streamTypeNames)[STREAM_TYPE_PRIVATE_DATA] =
			    "ISO/IEC 13818 Private Data";

		(*streamTypeNames)[STREAM_TYPE_MHEG] = "ISO/IEC 13522 MHEG";
		(*streamTypeNames)[STREAM_TYPE_MPEG2_DSMCC] = (
				"ISO/IEC 13818-1 Annex A DSM-CC");

		(*streamTypeNames)[STREAM_TYPE_H222_1] = "ITU-T Rec. H.222.1";
		(*streamTypeNames)[STREAM_TYPE_DSMCC_TYPE_A] = "ISO/IEC 13818-6 type A";
		(*streamTypeNames)[STREAM_TYPE_DSMCC_TYPE_B] = "ISO/IEC 13818-6 type B";
		(*streamTypeNames)[STREAM_TYPE_DSMCC_TYPE_C] = "ISO/IEC 13818-6 type C";
		(*streamTypeNames)[STREAM_TYPE_DSMCC_TYPE_D] = "ISO/IEC 13818-6 type D";
		(*streamTypeNames)[STREAM_TYPE_MPEG2_AUX] = "ISO/IEC 13818-1 auxiliary";
		(*streamTypeNames)[STREAM_TYPE_AUDIO_AAC] = (
				"ISO/IEC 13818-7 Audio with ADTS transport syntax AAC");

		(*streamTypeNames)[STREAM_TYPE_VIDEO_MPEG4] = "ISO/IEC 14496-2 Visual";
		(*streamTypeNames)[STREAM_TYPE_AUDIO_MPEG4] = (
				"ISO/IEC 14496-2 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3");

		(*streamTypeNames)[STREAM_TYPE_SL_PES] = (
				"ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets");

		(*streamTypeNames)[STREAM_TYPE_SL_SECTION] = (
				"ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC 14496_sections");

		(*streamTypeNames)[STREAM_TYPE_DSMCC_SYNCH] = "ISO/IEC 13818-6 Synchronized Download Protocol";
		(*streamTypeNames)[STREAM_TYPE_META_PES] = "Metadata carried in PES packets";
		(*streamTypeNames)[STREAM_TYPE_META_SEC] = "Metadata carried in metadata_sections";
		(*streamTypeNames)[STREAM_TYPE_META_DATA_CAROUSEL] = "Metadata carried in ISO/IEC 13818-6 Data Carousel";
		(*streamTypeNames)[STREAM_TYPE_META_OBJECT_CAROUSEL] = "Metadata carried in ISO/IEC 13818-6 Object Carousel";
		(*streamTypeNames)[STREAM_TYPE_META_DSMCC_SYNCH] = "Metadata carried in ISO/IEC 13818-6 Synchronized Download Protocol";
		(*streamTypeNames)[STREAM_TYPE_MPEG2_IPMP_STREAM] = "IPMP stream (defined in ISO/IEC 13818-11, MPEG-2 IPMP)";
		(*streamTypeNames)[STREAM_TYPE_VIDEO_H264] = "AVC video stream as defined in ITU-T Rec. H.264 | ISO/IEC 14496-10 video";
		(*streamTypeNames)[STREAM_TYPE_IPMP_STREAM] = "IPMP stream";
		(*streamTypeNames)[STREAM_TYPE_AUDIO_AC3] = "audio AC3";
		(*streamTypeNames)[STREAM_TYPE_AUDIO_DTS] = "audio DTS";

		this->pid = pid;
		this->programNumber = programNumber;
		this->pcrPid = 0;
	}

	void Pmt::addElementaryStream(unsigned int pid, short esType) {
		cout << "pid = '" << pid << "' esType = '" << esType;
		cout << "' " << endl;
		(*streams)[pid] = esType;
	}

	void Pmt::addCarouselInfo(unsigned int pid, CarouselInfo* info) {
		(*carousels)[pid] = info;
	}

	void Pmt::addSEInfo(unsigned int pid, SEInfo* info) {
		(*ses)[pid] = info;
	}
/*
	void Pmt::addAITInfo(unsigned int pid, AITInfo* info) {
		(*aits)[pid] = info;
	}
*/
	unsigned int Pmt::getPid() {
		return this->pid;
	}

	void Pmt::setPid(unsigned int pid) {
		this->pid = pid;
	}

	unsigned int Pmt::getProgramNumber() {
		return this->programNumber;
	}

	void Pmt::setProgramNumber(unsigned int programNumber) {
		this->programNumber = programNumber;
	}

	bool Pmt::hasPid(unsigned int somePid) {
		if (somePid == this->pid) {
			return true;
		}

		return (streams->count(somePid) != 0);
	}

	short Pmt::getStreamTypeValue(unsigned int somePid) {
		if (streams->count(somePid) == 0) {
			return -1;
		}

		return (*streams)[somePid];
	}

	string Pmt::getStreamType(unsigned int somePid) {
		if (streams->count(somePid) == 0) {
			return "";
		}

		return getStreamTypeName((*streams)[somePid]);
	}

	bool Pmt::isSectionType(unsigned int pid) {
		short value;
		value = getStreamTypeValue(pid);

		if (value == STREAM_TYPE_PRIVATE_SECTION ||
			    value == STREAM_TYPE_PRIVATE_DATA ||
			    (value >= STREAM_TYPE_DSMCC_TYPE_A &&
			    value <= STREAM_TYPE_DSMCC_TYPE_D)) {

			return true;
		}
		return false;
	}

	string Pmt::getStreamTypeName(short streamType) {
		if (streamTypeNames == NULL ||
			    streamTypeNames->count(streamType) == 0) {

			return "unknown type '" + intToStrHexa(streamType) + "'";
		}
		return (*streamTypeNames)[streamType];
	}

	bool Pmt::processSectionPayload() {
		if (tableId != 0x02 || !isConsolidated()) {
			cout << "Pmt::processSectionPayload Warning! inconsistency found";
			cout << " tableId = " << tableId << " and isConsolidated = ";
			cout << isConsolidated() << endl;
			return false;
		}

		if (programNumber != idExtention) {
			cout << "Pmt::processSectionPayload Warning! programNumber !=";
			cout << " idExtension." << endl;
		}

		char sectionPayload[sectionLength - 9];
		memcpy((void*)(&sectionPayload[0]), getPayload(), sectionLength - 9);

		pcrPid = ((sectionPayload[0] & 0x1F) << 8) |
			    (sectionPayload[1] & 0xFF);

		unsigned int programInfoLength;
		programInfoLength = ((sectionPayload[2] & 0x0F) << 8) |
			    (sectionPayload[3] & 0xFF);

		unsigned int i;
		short streamType;
		unsigned int elementaryPid;
		unsigned int esInfoLength;

		i = 4 + programInfoLength;//jumping descriptors
		while (i < (sectionLength - 9)) {
			streamType = (short)(sectionPayload[i] & 0xFF);
			i++;
			elementaryPid = ((sectionPayload[i] & 0x1F) << 8) |
			    (sectionPayload[i + 1] & 0xFF);

			(*streams)[elementaryPid] = streamType;

			i += 2;
			esInfoLength = ((sectionPayload[i] & 0x0F) << 8) |
			    (sectionPayload[i + 1] & 0xFF);

			i = i + 2 + esInfoLength;
		}

		return true;
	}

	map<unsigned int, short>* Pmt::getStreamsInformation() {
		return streams;
	}

	map<unsigned int, CarouselInfo*>* Pmt::getCarousels() {
		return carousels;
	}

	map<unsigned int, SEInfo*>* Pmt::getStreamEvents() {
		return ses;
	}

	unsigned int Pmt::getPCRPid() {
		return pcrPid;
	}

	void Pmt::setPCRPid(unsigned int pcrPid) {
		this->pcrPid = pcrPid;
	}

	bool Pmt::hasData() {
		if (!carousels->empty() || !ses->empty()/* || !aits->empty()*/) {
			return true;
		}

		return false;
	}

	double Pmt::getTransportRate(unsigned int rateIndex) {
		if (pcrRates != NULL && rateIndex < pcrRates->size()) {
			return (*pcrRates)[rateIndex];
		}

		return 0;
	}

	void Pmt::setPcrIndex(vector<unsigned int>* index) {
		if (pcrIndex != NULL) {
			delete pcrIndex;
			pcrIndex = NULL;
		}

		pcrIndex = index;
	}

	void Pmt::setPcrValues(vector<uint64_t>* readedValues) {
		if (pcrValues != NULL) {
			delete pcrValues;
			pcrValues = NULL;
		}

		pcrValues = readedValues;
	}

	void Pmt::refreshPcrRates() {
		double rate, a;
		vector<uint64_t>::iterator i;
		vector<unsigned int>::iterator j;

		if (pcrValues != NULL && pcrIndex != NULL) {
			if (pcrValues->size() != pcrIndex->size()) {
				cout << "Pmt::refreshPcrRates Warning DIFF PCR sizes" << endl;
				return;
			}

			if (pcrRates != NULL) {
				delete pcrRates;
				pcrRates = NULL;
			}
			pcrRates = new vector<uint64_t>;

			a = 0;
			i = pcrValues->begin();
			j = pcrIndex->begin();
			while ((i + 1) != pcrValues->end()) {
				rate = ((((*(j + 1)) - (*j)) * SYSTEM_CLOCK_FREQUENCY) /
						((*(i + 1)) - (*i)));

				pcrRates->push_back((uint64_t) rate);
				a++;

				uint64_t diff;
				double seconds;
				std::_Ios_Fmtflags originalFlags = cout.flags();
				int originalPrecision = cout.precision();

				//cout << "idx: " << (*j) << " <-> " << (*i) << endl;
				cout << "Pmt::refreshPcrRates Rate" << a;
				cout.setf( ios_base::fixed, ios_base::floatfield );
				cout.precision(2);
				cout << " = " << (rate/1000) << " KBps";

				diff = (*(i + 1)) - (*i);
				seconds = diff;
				seconds = seconds / SYSTEM_CLOCK_FREQUENCY;
				cout.setf( ios_base::fixed, ios_base::floatfield );
				cout.precision(3);
				cout << " (" << seconds << " s)" << endl;

				cout.flags(originalFlags);
				cout.precision(originalPrecision);

				++i;
				++j;
			}

			if (pcrValues->size() != (pcrRates->size() + 1)) {
				cout << "Pmt::refreshPcrRates Warning PCRRates sizes" << endl;
			}
		}
	}

	void Pmt::print() {
		cout << "Pmt::print" << endl;
		cout << "Program number = " << idExtention << endl;
		cout << "streams:" << endl;
		map<unsigned int, short>::iterator i;
		for (i = streams->begin(); i != streams->end(); ++i) {
			cout << "streamType '" << getStreamTypeName(i->second) << "' ";
			cout << " has pid = '" << hex << i->first << "'" << endl;
		}
	}
}
}
}
}
}
