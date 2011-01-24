#include "../../../include/mpeg/si/Pat.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {
	Pat::Pat() : TSSection() {
		programs = new map<unsigned int, Pmt*>;
		pat = new map<unsigned int, unsigned int>;
		unprocessedPmts = new vector<unsigned int>;
		currentPid = 150;
		currentCarouselId = 15;
		carouselComponentTag = 0x21;
	}

	unsigned int Pat::getNextPid() {
		currentPid++;
		while (hasPid(currentPid)) {
			currentPid++;
		}

		return currentPid;
	}

	unsigned int Pat::getNextCarouselId() {
		currentCarouselId++;
		return currentCarouselId;
	}

	unsigned int Pat::getCarouselComponentTag() {
		return carouselComponentTag;
	}

	bool Pat::hasProgramNumber(unsigned int programNumber) {
		map<unsigned int, unsigned int>::iterator i;
		i = pat->begin();
		while (i != pat->end()) {
			if (i->second == programNumber) {
				return true;
			}
			++i;
		}
		return false;
	}

	bool Pat::hasPid(unsigned int pid) {
		map<unsigned int, Pmt*>::iterator i;
		Pmt* pmt;

		i = programs->begin();
		while (i != programs->end()) {
			pmt = i->second;
			if (pmt->hasPid(pid)) {
				return true;
			}
			++i;
		}
		return false;
	}

	Pmt* Pat::getPmtByProgramNumber(unsigned int programNumber) {
		map<unsigned int, unsigned int>::iterator i;

		i = pat->begin();
		while (i != pat->end()) {
			if (i->second == programNumber) {
				if (programs->count(i->first) != 0) {
					return (*programs)[i->first];
				}
			}
			++i;
		}
		return NULL;
	}

	unsigned int Pat::getTSId() {
		return this->idExtention;
	}

	void Pat::setTSId(unsigned int newTSId) {
		this->idExtention = newTSId;
	}

	unsigned int Pat::getNumOfPrograms() {
		return programs->size();
	}

	void Pat::addProgram(unsigned int pid, unsigned int programNumber) {
		if (pat->count(pid) != 0) {
			cout << "Pat::addProgram Warning! Trying to override an existent";
			cout << " program. Pid = '" << pid << "'" << endl;

		} else {
			(*pat)[pid] = programNumber;
		}
	}

	void Pat::addPmt(Pmt* program) {
		if (program == NULL) {
			return;
		}

		vector<unsigned int>::iterator i;
		for (i = unprocessedPmts->begin(); i != unprocessedPmts->end(); ++i) {
			if (*i == program->getPid()) {
				unprocessedPmts->erase(i);
				break;
			}
		}

		if (programs->count(program->getPid() != 0)) {

			cout << "Pat::addPmt Warning! Trying to override an existent";
			cout << " program. Pid = '" << program->getPid() << "'";
			cout << endl;

		} else {
			(*programs)[program->getPid()] = program;
		}
	}

	unsigned int Pat::getFirstProgramNumber() {
		if (pat->empty()) {
			return 0;
		}
		return pat->begin()->second;
	}

	unsigned int Pat::getProgramNumberByPid(unsigned int pid) {
		if (pat->count(pid) != 0) {
			return (*pat)[pid];
		}
		return 0;
	}

	string Pat::getStreamType(unsigned int pid) {
		if (programs->empty()) {
			return "";
		}

		map<unsigned int, Pmt*>::iterator i;

		Pmt* program = NULL;
		i = programs->begin();
		while (i != programs->end()) {
			program = i->second;
			if (program->hasPid(pid)) {
				break;
			}

			program = NULL;
			++i;
		}

		if (program == NULL) {
			return "";
		}

		return program->getStreamType(pid);
	}

	bool Pat::isSectionType(unsigned int pid) {
		unsigned int unpPid;

		if (pid == 0x00 || pid == 0x01 || pid == 0x03 ||
			    programs->count(pid) != 0) {

			return true;
		}

		vector<unsigned int>::iterator it;
		if (unprocessedPmts != NULL) {
			it = unprocessedPmts->begin();
			while (it != unprocessedPmts->end()) {
				unpPid = *it;
				if (unpPid == pid) {
					return true;
				}
				++it;
			}
		}

		map<unsigned int, Pmt*>::iterator i;

		Pmt* program = NULL;
		i = programs->begin();
		while (i != programs->end()) {
			program = i->second;
			if (program->hasPid(pid)) {
				break;
			}

			program = NULL;
			++i;
		}

		if (program == NULL) {
			return false;
		}

		return program->isSectionType(pid);
	}

	bool Pat::processSectionPayload() {
		if (tableId != 0 || !isConsolidated()) {
			return false;
		}

		char sectionPayload[sectionLength - 9];
		memcpy((void*)(&sectionPayload[0]), getPayload(), sectionLength - 9);

		unsigned int n;
		unsigned int i;
		unsigned int programNumber;
		unsigned int pid;
		n = (unsigned int)((sectionLength - 9)/4);

		for (i = 0; i < n; i++) {
			programNumber = ((sectionPayload[i*4] & 0xFF) << 8) |
				    (sectionPayload[((i*4) + 1)] & 0xFF);

			pid = ((sectionPayload[((i*4) + 2)] & 0x1F) << 8) |
				    (sectionPayload[((i*4) + 3)] & 0xFF);

			(*pat)[pid] = programNumber;
			unprocessedPmts->push_back(pid);
		}
		return true;
	}

	bool Pat::hasUnprocessedPmt() {
		if (unprocessedPmts->empty()) {
			return false;
		}
		return true;
	}

	vector<unsigned int>* Pat::getUnprocessedPmtPids() {
		return unprocessedPmts;
	}

	void Pat::checkConsistency() {
		map<unsigned int, unsigned int>::iterator i;

		i = pat->begin();
		while (i != pat->end()) {
			if (programs->count(i->first) == 0) {
				pat->erase(i);
				if (pat->empty()) {
					return;
				}
				i = pat->begin();
			} else {
				++i;
			}
		}
	}

	map<unsigned int, Pmt*>* Pat::getProgramsInfo() {
		return this->programs;
	}

	bool Pat::hasData() {
		map<unsigned int, Pmt*>::iterator i;

		i = programs->begin();
		while (i != programs->end()) {
			if (i->second->hasData()) {
				return true;
			}
			++i;
		}

		return false;
	}

	int Pat::createPATStream(string streamUri) {
		unsigned int bw, pos = 0;
		char section[1024];
		char ts[188];
		map<unsigned int, unsigned int>::iterator i;
		int fd;

#ifdef WIN32
		fd = open(streamUri.c_str(), O_CREAT | O_WRONLY | O_BINARY, 0644);
#else
		fd = open(streamUri.c_str(), O_WRONLY, 0644);
#endif

		if (fd <= 0) {
			cout << "PAT::createPATStream can't open output file '";
			cout << streamUri << "' ";
#ifdef WIN32
			cout << "err = '" << strerror(errno) << "'" << endl;
#else
			perror(strerror(errno));
#endif
			return 1;
		}

		memset(ts, 0, 188);

		ts[0] = 0x47;
		ts[1] = 0x40;
		ts[2] = 0x00;
		ts[3] = 0x10;
		ts[4] = 0x00;

		//encoding tableId
		section[pos] = 0x00;
		pos++;

		//encoding sectionSyntaxIndicator
		section[pos] = 0xB0;
		pos += 2;

		//encoding idExtension (trsnsport_stream_id)
		section[pos] = (tsPid & 0xFF00) >> 8;
		pos++;
		section[pos] = (tsPid & 0xFF);
		pos++;

		//encoding versionNumber and currentNextIndicator
		section[pos] = (0xC0 | (versionNumber << 1));
		section[pos] = (section[pos] | 0x01);
		pos++;

		//encoding sectionNumber
		section[pos] = 0;
		pos++;

		//encoding lastSectionNumber
		section[pos] = 0;
		pos++;

		for (i = pat->begin(); i != pat->end(); ++i) {
			section[pos] = (i->second & 0xFF00) >> 8;
			pos++;
			section[pos] = (i->second & 0xFF);
			pos++;
			section[pos] = 0xE0;
			section[pos] = section[pos] | ((i->first & 0xFF00) >> 8);
			pos++;
			section[pos] = (i->first & 0xFF);
			pos++;
		}

		section[pos++] = 0xC1;
		section[pos++] = 0xC2;
		section[pos++] = 0xC3;
		section[pos++] = 0xC4;

		//encoding sectionLength
		section[1] = section[1] | (((pos-3) & 0x3F00) >> 8);
		section[2] = (pos-3) & 0xFF;

		memcpy(ts+5, section, pos);
		bw = write(fd, ts, 188);
		if (bw != 188) {
			cout << "PAT::createPATStream - Write error." << endl;
			return 2;
		}

		close(fd);

		cout << "Successfully encoded pat into " << streamUri << endl;

		return 0;
	}

	void Pat::print() {
		cout << "Pat::print" << endl;
		cout << "TS id = " << idExtention << endl;
		cout << "programs:" << endl;
		map<unsigned int, unsigned int>::iterator i;
		for (i = pat->begin(); i != pat->end(); ++i) {
			cout << "programNumber '" << hex << i->second << "' ";
			cout << " has pid = '" << hex << i->first << "'" << endl;
		}
	}
}
}
}
}
}
