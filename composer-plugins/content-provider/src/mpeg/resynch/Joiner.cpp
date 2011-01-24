#include "../../../include/mpeg/resynch/Joiner.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace resynch {

	Joiner::Joiner() {
		list = new vector<string>;
		keepTsId = false;
		keepPId = false;
		incrementVersionNumber = false;
	}

	Joiner::~Joiner() {
		if (list != NULL) {
			list->clear();
			delete list;
			list = NULL;
		}
	}

	int Joiner::getReadFD(string fileName) {
		int fd;
		#ifdef WIN32
			fd = open(fileName.c_str(), O_RDONLY | O_BINARY, 0644);
		#else
			fd = open(fileName.c_str(), O_RDONLY, 0644);
		#endif
		return fd;
	}

	int Joiner::getWriteFD(string fileName) {
		int fd;
		#ifdef WIN32
			fd = open(fileName.c_str(), O_CREAT | O_WRONLY | O_BINARY, 0644);
		#else
			fd = open(fileName.c_str(), O_WRONLY, 0644);
		#endif
		return fd;
	}

	long Joiner::getFileSize(string fileName) {
		FILE * stream = fopen( fileName.c_str(), "r" );
		if (stream == NULL) {
			return -1;
		}
		fseek( stream, 0L, SEEK_END );
		long endPos = ftell( stream );
		fclose( stream );
		return endPos;
	}

	void Joiner::setKeepTsId(bool value) {
		keepTsId = value;
	}

	void Joiner::setKeepPId(bool value) {
		keepPId = value;
	}

	void Joiner::setIncrementVersionNumber(bool value) {
		incrementVersionNumber = value;
	}

	void Joiner::setOriginal(bool value) {
		original = value;
	}

	void Joiner::addFile(string filename) {
		list->push_back(filename);
		cout << filename << " file added." << endl;
	}

	void Joiner::clearList() {
		list->clear();
	}

	bool Joiner::getPatPmt(Pat** pat, Pmt** pmt, string filename) {
		int fd;
		int br = 1;
		vector<unsigned int>* unprocessedPmts;
		vector<unsigned int>::iterator it;
		map<unsigned int, Pmt*>* pmts = NULL;
		TSPacket* tsp;
		char buffer[188], payload[184];
		unsigned int programNumber;

		fd = getReadFD(filename);

		if (fd <= 0) {
			cout << "Joiner::getPatPmt can't open file " << filename << endl;
			cout << "err = '" << strerror(errno) << "'" << endl;
			return false;
		}

		*pat = new Pat();
		while (!(*pat)->isConsolidated()) {
			br = read(fd, buffer, 188);
			if (br > 0) {
				tsp = new TSPacket(buffer);
				if (tsp->getPid() == 0x00) {
					tsp->getPayload(payload);
					(*pat)->process(payload);
				}
				delete tsp;
				tsp = NULL;
			}
			else {
				break;
			}
		}

		if ((*pat)->processSectionPayload()) {
			pmts = new map<unsigned int, Pmt*>;
			unprocessedPmts = (*pat)->getUnprocessedPmtPids();
			it = unprocessedPmts->begin();
			while (it != unprocessedPmts->end()) {
				programNumber = (*pat)->getProgramNumberByPid(*it);
				(*pmt) = new Pmt(*it, programNumber);
				(*pmts)[*it] = (*pmt);
				++it;
			}
		}
		while (true) {
			br = read(fd, buffer, 188);
			if (br <= 0) break;
			if (br != 188) continue;
			tsp = new TSPacket(buffer);
			if (tsp->getAdaptationFieldControl() == 1 ||
					tsp->getAdaptationFieldControl() == 3) {
				if (pmts->count(tsp->getPid())) {
					(*pmt) = (*pmts)[tsp->getPid()];
					tsp->getPayload(payload);
					(*pmt)->process(payload);
					if ((*pmt)->processSectionPayload()) {
						(*pat)->addPmt(*pmt);
						pmts->erase(pmts->find(tsp->getPid()));
						if (pmts->empty()) {
							delete tsp;
							tsp = NULL;
							break;
						}
					}
				}
			}
			delete tsp;
			tsp = NULL;
		}
		close(fd);
		if (pmts != NULL) {
			delete pmts;
			pmts = NULL;
		} else {
			if ((*pat) == NULL) {
				delete (*pat);
				(*pat) = NULL;
			}
			return false;
		}
		return true;
	}

	int Joiner::joinFiles(string output) {
		int fdin = 0, fdout = 0, patFd = 0, pmtFd = 0;
		char buffer[1128];
		char buf[188];
		int br = 1, bw = 0, packetbr;
		unsigned int i, versionNumber;
		unsigned char patCnt = 0, pmtCnt = 0;
		vector<unsigned int>::iterator it;
		Pat* pat = NULL, *pat1 = NULL;
		Pmt* pmt = NULL, *pmt1 = NULL;
		map<unsigned int, short>* table = NULL, *originalTable = NULL;
		map<unsigned int, short>::iterator ito, itm;
		map<unsigned int, unsigned char> continuity;
		string pmtPath, patPath;
		/*long bytesRead = 0, totalBytes = 0;*/

		if (list->size() == 0) return 0;

		fdout = getWriteFD(output);

		if (fdout <= 0) {
			cout << "joinFiles can't open output file " << output << endl;
			cout << "err = '" << strerror(errno) << "'" << endl;
			return 1;
		}

		/*
		for (i = 0; i < list->size(); i++) {
			string str = list->at(i);
			totalBytes += getFileSize(str);
		}*/

		for (i = 0; i < list->size(); i++) {
			string str = list->at(i);
			TSPacket* tsp;

			if (i == 0) {
				if (!(getPatPmt(&pat, &pmt, str))) {
					return 2;
				}

				versionNumber = pmt->getVersionNumber();
				pmtPath = PMT_ES_PATH+intToStrHexa(pmt->getPid()) + ".ts";
				patPath = PAT_ES_PATH+intToStrHexa(0x00) + ".ts";
				pat->print();
				pmt->print();
			}
			else {
				versionNumber++;
				if (original) {
					if (!(getPatPmt(&pat1, &pmt1, str))) {
						return 2;
					}
					originalTable = pmt1->getStreamsInformation();
					PMTStreamFactory::createPMTStream(
									pmtPath, pmt1->getPid(),
									pmt1->getProgramNumber(), pmt1->getPCRPid(),
									pmt1->getVersionNumber(), originalTable,
									NULL, NULL);

				} else {
					originalTable = pmt->getStreamsInformation();
					if (incrementVersionNumber) {
						pmt->setVersionNumber(versionNumber);
					}
					if (keepTsId || keepPId) {
						getPatPmt(&pat1, &pmt1, str);
						if (keepTsId) {
							pat->setExtensionId(pat1->getExtensionId());
						}
						if (keepPId) {
							originalTable = pmt1->getStreamsInformation();
						}
					}
					PMTStreamFactory::createPMTStream(
									pmtPath, pmt->getPid(),
									pmt->getProgramNumber(), pmt->getPCRPid(),
									pmt->getVersionNumber(), originalTable,
									NULL, NULL);
				}
				pat->createPATStream(patPath);
				if (pat1 != NULL){
					delete pat1;
					pat1 = NULL;
				}
				if (pmt1 != NULL){
					delete pmt1;
					pmt1 = NULL;
				}
				patFd = getReadFD(patPath);
				pmtFd = getReadFD(pmtPath);
			}

			fdin = getReadFD(str);
			br = 1;
			/* Joining */
			while (br > 0) {
				int numPackets, idx;
				unsigned short PID;
				br = read(fdin, buffer, 1128); /* 6 TS Packets */

				/*bytesRead += br;
				if ((bytesRead ???) {
					int value = 100*bytesRead/totalBytes;
					cout << "Progress: " << value << "% done." << endl;
				}*/

				if (br != 0) {
					numPackets = br / 188;
					for (idx = 0; idx < numPackets; idx++) {
						tsp = new TSPacket(buffer+(idx*188));
						PID = tsp->getPid();
						if (i == 0) {
							tsp->getPacketData(buf);
							if (PID == 0x00) {
								patCnt = tsp->getContinuityCounter();
							} else if (PID == pmt->getPid()) {
								pmtCnt = tsp->getContinuityCounter();
							} else {
								continuity[PID] = tsp->getContinuityCounter();
							}

						} else {
							packetbr = 0;
							if (PID == 0x00) {
								/* PAT packet */
								while (packetbr == 0) {
									packetbr = read(patFd, buf, 188);
									if (packetbr > 0) {
										patCnt++;
										buf[3] = (buf[3] | (patCnt & 0x0F));
									} else if (packetbr == 0) {
										close(patFd);
										patFd = getReadFD(patPath);
									} else {
										memset(buf, 0, 188);
										break;
									}
								}
							}
							else if (PID == pmt->getPid()) {
								/* expecting just one */
								/* PMT packet */
								while (packetbr == 0) {
									packetbr = read(pmtFd, buf, 188);
									if (packetbr > 0) {
										pmtCnt++;
										buf[3] = (buf[3] | (pmtCnt & 0x0F));
									} else if (packetbr == 0) {
										close(pmtFd);
										pmtFd = getReadFD(pmtPath);
									} else {
										memset(buf, 0, 188);
										break;
									}
								}
							}
							else {
								/* Video, Audio, etc packets */
								memcpy(buf, buffer+(idx*188), 188);
								table = pmt->getStreamsInformation();
								originalTable =
									pmt->getStreamsInformation();
								for (	ito = originalTable->begin();
										ito != originalTable->end();
										ito++) {
									if (ito->second == (*table)[PID]) {
										continuity[ito->first]++;
										buf[3] = ((buf[3] & 0xF0) |
											(continuity[ito->first] & 0x0F));
										break;
									}
								}
							}
						}
						bw = write(fdout, buf, 188);
						if (bw != 188) {
							cout << "Write error." << endl;
						}
						delete tsp;
					}
				}
			}
			br = 1;
			close(fdin);
			if (patFd > 0) close(patFd);
			if (pmtFd > 0) close(pmtFd);
		}
		if (pat != NULL) {
			delete pat;
		}
		if (pmt != NULL) {
			delete pmt;
		}
		close(fdout);
		cout << " done." << endl;
		return 0;
	}
}
}
}
}
}
