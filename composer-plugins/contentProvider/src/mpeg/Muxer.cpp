#include "../../include/mpeg/Muxer.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {

bool Muxer::initThreadFlags = false;
pthread_mutex_t Muxer::threadFlagMutex;
pthread_cond_t Muxer::threadFlagConditionVariable;

	Muxer::Muxer() {
		this->pat = new Pat();
		this->numOfPackets = 0;
		this->errCondition = 0;
		this->tsFileName = "";
		this->fileNames = NULL;
		this->packetHandler = NULL;
		this->hasSdt = false;
		this->hasEit = false;
		this->hasCdt = false;
		this->dataPacks = 0;
	}

	int Muxer::getReadFD(const char* fileName) {
		int fd;

#ifdef WIN32
		fd = open(fileName, O_RDONLY | O_BINARY, 0644);
#else
		fd = open(fileName, O_RDONLY, 0644);
#endif
		return fd;
	}

	Muxer* Muxer::_instance = 0;

	Muxer* Muxer::getInstance() {
		if (Muxer::_instance == NULL) {
			Muxer::_instance = new Muxer();
		}

		return Muxer::_instance;
	}

	string Muxer::getStreamType(int pid) {
		return pat->getStreamType(pid);
	}

	bool Muxer::isSectionStream(int pid) {
		return pat->isSectionType(pid);
	}

	bool Muxer::openTSFile(string fileName) {
		//TODO: how to manipulate a TS file with more than one PAT

		this->tsFileName = fileName;
		if (fileName == "") {
			return false;
		}

		int fd;
		TSPacket* packet = NULL;
		Pmt* pmt = NULL;
		vector<uint64_t>* pcrValues;
		vector<unsigned int>* pcrIndex;
		vector<unsigned int>* pids;
		vector<unsigned int>* unprocessedPmts;
		vector<unsigned int>::iterator i;
		map<unsigned int, Pmt*>* pmts;
		map<unsigned int, Pmt*>::iterator j;
		unsigned int programNumber;
		char tsPacketPayload[184];

		fd = getReadFD(fileName.c_str());
		if (fd <= 0) {
			cout << "Muxer::openTSFile cant open file" << endl;
			cout << "mux1 err = '" << strerror(errno) << "'" << endl;
			return false;
		}

		memset(tsPacketPayload, 0, sizeof(tsPacketPayload));

		pids = new vector<unsigned int>;
		pids->push_back(0x00);
		while (!(pat->isConsolidated()) &&
			    errCondition < ERR_CONDITION_SATISFIED) {

			packet = getNextTSPacketByPid(fd, pids);
			if (packet != NULL &&
				    (packet->getAdaptationFieldControl() == 1 ||
				    packet->getAdaptationFieldControl() == 3)) {

				packet->getPayload(tsPacketPayload);
				//patPacket->print();

				pat->process(tsPacketPayload);
				//pat->print();

			} else {
				errCondition++;
			}
			delete packet;
			packet = NULL;
		}

		delete pids;
		pids = NULL;

		if (pat->processSectionPayload()) {
			pmts = new map<unsigned int, Pmt*>;

			unprocessedPmts = pat->getUnprocessedPmtPids();
			i = unprocessedPmts->begin();
			while (i != unprocessedPmts->end()) {
				programNumber = pat->getProgramNumberByPid(*i);
				pmt = new Pmt(*i, programNumber);
				(*pmts)[*i] = pmt;
				++i;
			}

			while (errCondition < ERR_CONDITION_SATISFIED) {
				packet = getNextTSPacketByPid(fd, pids);
				/*if (pmtPacket != NULL) {
					cout << "pmtPacket: " << endl;
					pmtPacket->print();
				}*/
				if (packet != NULL &&
					    (packet->getAdaptationFieldControl() == 1 ||
					    packet->getAdaptationFieldControl() == 3)) {

					if (pmts->count(packet->getPid()) != 0) {
						pmt = (*pmts)[packet->getPid()];
						packet->getPayload(tsPacketPayload);
						pmt->process(tsPacketPayload);
						if (pmt->processSectionPayload()) {
							//pmt->print();
							pat->addPmt(pmt);
							pmts->erase(pmts->find(packet->getPid()));
							if (pmts->empty()) {
								delete packet;
								packet = NULL;
								break;
							}
						}
					}

				} else {
					errCondition++;
				}
				delete packet;
				packet = NULL;
			}
			cout << "DONE! " << numOfPackets << " packets processed. err = '";
			cout << strerror(errno) << "' calculating rates... ";

			delete pmts;
			pmts = NULL;
			close(fd);

			pcrValues = new vector<uint64_t>;
			pcrIndex = new vector<unsigned int>;
			pids = new vector<unsigned int>;
			pmts = pat->getProgramsInfo();
			if (!pmts->empty()) {
				j = pmts->begin();
				while (j != pmts->end()) {
					pmt = j->second;
					numOfPackets = 0;
					fd = getReadFD(fileName.c_str());
					pids->push_back(pmt->getPCRPid());
					packet = getNextTSPacketByPid(fd, pids);
					while (true) {
						if (packet != NULL && packet->getPCRValue() != 0) {
							pcrValues->push_back(packet->getPCRValue());
							pcrIndex->push_back(numOfPackets * 188);
						}
						packet = getNextTSPacketByPid(fd, pids);
						if (packet == NULL) {
							break;
						}
					}
					pmt->setPcrIndex(pcrIndex);
					pmt->setPcrValues(pcrValues);
					pmt->refreshPcrRates();

					close(fd);
					++j;
				}
			}
			delete pids;
			pids = NULL;
			cout << " all done for the opened TS!" << endl;
		}
		close(fd);
		numOfPackets = 0;
		dataPacks = 0;
		return true;
	}

	void Muxer::saveTSFile(string fileName) {
		int fdin, fdout;

		fdin = getReadFD(tsFileName.c_str());
		if (fdin <= 0) {
			cout << "Muxer::saveTSFile cant open input file" << endl;
			cout << "mux2 err = '" << strerror(errno) << "'" << endl;
			return;
		}

#ifdef WIN32
		fdout = open(
				fileName.c_str(),
				O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);

#else
		fdout = open(fileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
#endif

		if (fdout <= 0) {
			cout << "Muxer::saveTSFile cant create output file" << endl;
			cout << "mux3 err = '" << strerror(errno) << "'" << endl;
			return;
		}

		muxToFile(fdin, fdout);

		close(fdin);
		close(fdout);
	}

	void Muxer::muxToSock(int fdin, SocketServer* server) {
		unsigned int pid, pcrPid, bytesSent, rateIndex;
		int ret;
		TSPacket* packet = NULL;
		TSPacket* newPacket = NULL;
		char tsPacketData[TSPacket::TS_PACKET_SIZE];
		Pmt* pmt;
		map<unsigned int, Pmt*>* programs;
		double timer, nthTRate, tRate, elapsedTime, dataTime;
		uint64_t nthDenominator;
		long timeUnit;
		bool isFirstPcrPacket = true;

		bytesSent = 0;
		timer = 0;
		rateIndex = 0;

		//TODO: handle TS with two or more programs
		programs = pat->getProgramsInfo();
		pmt = programs->begin()->second;
		pcrPid = pmt->getPCRPid();

		//TODO: URGENT! FIX RATE / CLOCK STUFFS
		if (tsFileName.length() > 9 && tsFileName.substr(
				tsFileName.length() - 9, 9) == "matrix.ts") {

			cout << "MATRIX!!!" << endl;
			nthDenominator = 1000;
			//timeUnit = (long)(1100000 / nthDenominator);

		} else if (tsFileName.length() > 13 && tsFileName.substr(
				tsFileName.length() - 13, 13) == "f1_low_res.ts") {

			cout << "F1!!!" << endl;
			nthDenominator = 100;
			//timeUnit = (long)(1300000 / nthDenominator);

		} else {
			nthDenominator = 100;
			//timeUnit = (long)(1000000 / nthDenominator);
		}

		timeUnit = (long)(1000000 / nthDenominator);

		tRate = pmt->getTransportRate(rateIndex);
		nthTRate = tRate/nthDenominator;

		cout << "Muxing '" << tsFileName << "' with first rate '";
		cout << (tRate / 1000) << "' KBps." << endl;

		packet = getNextTSPacketByPid(fdin, NULL);
		while (packet != NULL) {
			ret = 0;
			if (timer == 0) {
				 // getCurrentTimeMillis returns micro, not millis :(
				//timer = getCurrentTimeMillis() * 1000;
				timer = getCurrentTimeMillis();
			}

			pid = packet->getPid();

			if (pid == pcrPid && packet->getPCRValue() != 0) {
				if (!isFirstPcrPacket) {
					rateIndex++;
					tRate = pmt->getTransportRate(rateIndex);
					nthTRate = tRate/nthDenominator;
				}
				isFirstPcrPacket = false;
			}

			//replace old pmt
			if (programs->count(pid) != 0) {
				delete packet;
				packet = NULL;

				newPacket = getNextPmtPacket(pid);
				if (newPacket != NULL) {
					memset(tsPacketData, 0, sizeof(tsPacketData));
					newPacket->getPacketData(tsPacketData);
					ret = server->send(
							tsPacketData,
							TSPacket::TS_PACKET_SIZE);

					if (ret < 0) {
#ifdef WIN32
						cout << "pmt muxWinSock error: ";
						cout << WSAGetLastError() << endl;
#else
						herror("pmt muxWinSock");
#endif
					} else if (ret != TSPacket::TS_PACKET_SIZE) {
						cout << "pmt muxWinSock warning!";
						cout << " cant send 188 bytes.";
						cout << endl;
					}

					if (packet != newPacket) {
						delete newPacket;
						newPacket = NULL;
					}
				}

			} else if (pid == 0x1FFF) {
				if (!dataPacketHandler(MUX_TO_SOCKET, 0, server)) {
					packet->getPacketData(tsPacketData);
					ret = server->send(
							tsPacketData,
							TSPacket::TS_PACKET_SIZE);

					if (ret < 0) {
#ifdef WIN32
						cout << "muxWinSock error: ";
						cout << WSAGetLastError() << endl;
#else
						herror("AV muxSock");
#endif
					} else if (ret != TSPacket::TS_PACKET_SIZE) {
						cout << "muxWinSock warning! cant send NULL PACKET.";
						cout << endl;
					}
				}

			} else if (!pat->hasPid(pid) && pid != 0) {
				cout << "replace non mapped" << endl;
				dataPacketHandler(MUX_TO_SOCKET, 0, server);

			} else {
				packet->getPacketData(tsPacketData);
				ret = server->send(
						tsPacketData,
						TSPacket::TS_PACKET_SIZE);

				if (ret < 0) {
#ifdef WIN32
					cout << "AV muxWinSock error: ";
					cout << WSAGetLastError() << endl;
#else
					herror("AV muxSock");
#endif
				} else if (ret != TSPacket::TS_PACKET_SIZE) {
					cout << "AV muxWinSock warning! cant send 188 bytes.";
					cout << endl;
				}
			}

			/*if (pcrPid == pid) {
				bytesSent = bytesSent + ret;
			}*/
			bytesSent = bytesSent + ret;

			//elapsedTime = (getCurrentTimeMillis() * 1000) - timer;
			elapsedTime = getCurrentTimeMillis() - timer;
			if (bytesSent >= nthTRate) {
				//cout << "Sent '" << bytesSent << "' bytes in '" << elapsedTime;
				//cout << "' microseconds. Checking data packets... ";
				if (elapsedTime < timeUnit) {
					dataTime = elapsedTime + (0.9 * elapsedTime);
					if ((dataTime) >= timeUnit) {
						dataTime = timeUnit - elapsedTime;
					}

					while (elapsedTime < dataTime) {
						//elapsedTime = (getCurrentTimeMillis() * 1000) - timer;
						elapsedTime = getCurrentTimeMillis() - timer;
						if (!dataPacketHandler(MUX_TO_SOCKET, 0, server)) {
							break;
						}
					}

					//cout << "Elapsed '" << elapsedTime << "', ";
					//cout << "sleeping '" << (1000000 - elapsedTime)/1000;
					//cout << "'" << endl;
					//elapsedTime = (getCurrentTimeMillis() * 1000) - timer;
					elapsedTime = getCurrentTimeMillis() - timer;
					if (elapsedTime < timeUnit) {
						Muxer::uSleep(timeUnit - elapsedTime);
					}

				} else {
					cout << "Server warning! To send '" << nthTRate << "' ";
					cout << "bytes and data packet takes '" << elapsedTime;
					cout << ", which is more than '" << timeUnit << "'";
					cout << endl;

					if (nthDenominator > 100) {
						nthDenominator = nthDenominator / 10;
						nthTRate = tRate/nthDenominator;
						timeUnit = (long)(1000000 / nthDenominator);

						cout << "Trying to correct nthDenominator to '";
						cout << nthDenominator;
						cout << "', nthTrate to '" << nthTRate;
						cout << "' and timeUnit to '" << timeUnit;
						cout << "'" << endl;
					}
				}

				timer = 0;
				bytesSent = 0;

			} else if (elapsedTime >= timeUnit) {
				cout << "Server warning! To send '" << nthTRate;
				cout << "' bytes takes more than " << nthDenominator;
				cout << "th of a second. elapsedTime = " << elapsedTime;
				cout << " and timeUnit = " << timeUnit << endl;

				if (nthDenominator > 100) {
					nthDenominator = nthDenominator / 10;
					nthTRate = tRate/nthDenominator;
					timeUnit = (long)(1000000 / nthDenominator);

					cout << "Trying to correct nthDenominator to '";
					cout << nthDenominator << "', nthTrate to '" << nthTRate;
					cout << "' and timeUnit to '" << timeUnit << "'" << endl;
				}

				timer = 0;
				bytesSent = 0;
			}

			if (packet != NULL) {
				delete packet;
			}
			packet = getNextTSPacketByPid(fdin, NULL);
		}

		cout << "DONE! " << numOfPackets << " ts packets writed (";
		cout << dataPacks << " data packets)" << endl;
		numOfPackets = 0;
		dataPacks = 0;
	}

	//Mux to File (fdout)
	void Muxer::muxToFile(int fdin, int fdout) {
		unsigned int pid, count, avCount;
		int ret;
		TSPacket* packet = NULL;
		TSPacket* newPacket = NULL;
		char tsPacketData[TSPacket::TS_PACKET_SIZE];

		avCount = 10;
		count = 0;
		cout << "Muxer::muxToFile" << endl;

		packet = getNextTSPacketByPid(fdin, NULL);
		while (packet != NULL) {
			pid = packet->getPid();
			if (count == avCount) {
				count = 0;
				avCount = 6;
				dataPacketHandler(fdout, MUX_TO_FILE);

			} else {
				//replace old pmt
				if (pat->getProgramsInfo()->count(pid) != 0) {
					newPacket = getNextPmtPacket(pid);
					if (newPacket != NULL) {
						memset(tsPacketData, 0, sizeof(tsPacketData));
						newPacket->getPacketData(tsPacketData);
						ret = write(fdout,
								(void*)tsPacketData, TSPacket::TS_PACKET_SIZE);

						if (ret != TSPacket::TS_PACKET_SIZE) {
							cout << "pmt mux warning! cant save 188 bytes.";
							cout << endl;
						}

						delete newPacket;
						newPacket = NULL;

					} else {
						cout << "new pmt packet is null" << endl;
					}

				} else if (pid == 0x1FFF) {
					cout << "replace 0x1FFF" << endl;
					count = 0;
					dataPacketHandler(fdout, MUX_TO_FILE);

				} else if (!pat->hasPid(pid) && pid != 0) {
					cout << "replace non mapped" << endl;
					count = 0;

				} else {
					count++;
					packet->getPacketData(tsPacketData);
					ret = write(
							fdout,
							(void*)tsPacketData, TSPacket::TS_PACKET_SIZE);

					if (ret != TSPacket::TS_PACKET_SIZE) {
						cout << "AV mux warning! cant save 188 bytes.";
						cout << endl;
					}
				}

				delete packet;
				packet = NULL;
				packet = getNextTSPacketByPid(fdin, NULL);
			}
		}

		cout << "DONE! " << numOfPackets << " ts packets writed (";
		cout << dataPacks << " dsmcc packets)" << endl;
		numOfPackets = 0;
		dataPacks = 0;
	}

	bool Muxer::dataPacketHandler(short type, int fdout, SocketServer* server) {
		TSPacket* newPacket = NULL;
		char tsPacketData[TSPacket::TS_PACKET_SIZE];
		int ret;

		if (pat->hasData() || hasSdt || hasEit || hasCdt) {
			newPacket = getNextDataPacket();
			if (newPacket != NULL) {
				dataPacks++;
				memset(tsPacketData, 0, sizeof(tsPacketData));
				newPacket->getPacketData(tsPacketData);
				switch (type) {
				case MUX_TO_FILE:
					ret = write(
							fdout,
							(void*)tsPacketData,
							TSPacket::TS_PACKET_SIZE);

					if (ret != TSPacket::TS_PACKET_SIZE) {
						cout << "nonmap dataPacketHandler muxtofile warning!";
						cout << "cant save 188 bytes." << endl;
					}
					delete newPacket;
					newPacket = NULL;
					return true;

				case MUX_TO_SOCKET:
					ret = server->send(
							tsPacketData,
							TSPacket::TS_PACKET_SIZE);

					if (ret != TSPacket::TS_PACKET_SIZE) {
						cout << "dataPacketHandler muxtosock warning! cant";
						cout << " send 188 bytes." << endl;
					}

					delete newPacket;
					newPacket = NULL;
					return true;
				}

				delete newPacket;
				newPacket = NULL;
			}
		}

		return false;
	}

	TSPacket* Muxer::getNextPmtPacket(unsigned int pmtPid) {
		int fd;
		TSPacket* packet;
		string pmtFileName;

		pmtFileName = PMT_ES_PATH + intToStrHexa(pmtPid) + ".ts";
		fd = getReadFD(pmtFileName.c_str());
		//pmt file was not created
		//(in this case it still not necessary, since TS was not modified)
		if (fd <= 0) {
			close(fd);
			return NULL;
		}

		packet = getNextTSPacketByPid(fd, NULL);
		if (packet == NULL) {
			cout << "cant find pmt packets for pmt pid = '";
			cout << pmtPid << "'" << endl;
			close(fd);
			return NULL;
		}

		close(fd);
		return packet;
	}

	TSPacket* Muxer::getNextDataPacket() {
		int fd;
		TSPacket* packet;
		string fileName, file;
		unsigned int currentPacket, l;

		Pmt* pmt;
		map<unsigned int, Pmt*>* programs;
		map<unsigned int, Pmt*>::iterator i;

		map<unsigned int, CarouselInfo*>* carousels;
		map<unsigned int, CarouselInfo*>::iterator j;

		map<unsigned int, SEInfo*>* events;
		map<unsigned int, SEInfo*>::iterator k;

		char payload[184];

		if (!pat->hasData() && !hasSdt && !hasEit && !hasCdt) {
			cout << "Muxer::getNextDataPacket() there is no data packet";
			cout << " to send." << endl;
			return NULL;
		}

		if (fileNames == NULL) {
			fileNames = new vector<string>;

			if (packetHandler == NULL) {
				packetHandler = new map<string, unsigned int>;
			}

			programs = pat->getProgramsInfo();
			i = programs->begin();
			while (i != programs->end()) {
				pmt = i->second;
		 		carousels = pmt->getCarousels();
		 		if (carousels != NULL && !carousels->empty()) {
		 			j = carousels->begin();
		 			while (j != carousels->end()) {
		 				fileName = OC_ES_PATH + intToStrHexa(j->first) + ".ts";
		 				fileNames->push_back(fileName);
		 				(*packetHandler)[fileName] = 0;
		 				++j;
		 			}
		 		}

		 		events = pmt->getStreamEvents();
		 		if (events != NULL && !events->empty()) {
		 			k = events->begin();
		 			while (k != events->end()) {
		 				fileName = SE_ES_PATH + intToStrHexa(k->first) + ".ts";
		 				fileNames->push_back(fileName);
		 				++k;
		 			}
		 		}
				++i;
			}

			if (hasSdt) {
				fileName = SDT_ES_PATH + intToStrHexa(17) + ".ts";
				fileNames->push_back(fileName);
				(*packetHandler)[fileName] = 0;
			}

			if (hasEit) {
				fileName = EIT_ES_PATH + intToStrHexa(18) + ".ts";
				fileNames->push_back(fileName);
				(*packetHandler)[fileName] = 0;
			}

			if (hasCdt) {
				fileName  = CDT_ES_PATH + intToStrHexa(41) + ".ts";
				fileNames->push_back(fileName);
				(*packetHandler)[fileName] = 0;
			}
		}

		if (fileNames->empty()) {
			delete fileNames;
			fileNames = NULL;
			if (packetHandler != NULL) {
				delete packetHandler;
				packetHandler = NULL;
			}
			return NULL;
		}

		fileName = *(fileNames->begin());
		fd = getReadFD(fileName.c_str());
		if (fd <= 0) {
			cout << "invalid file descriptor filename = '" << fileName;
			cout << "' fd = " << fd << "'" << endl;
			cout << "Muxer::getNextDataPacket() err = '";
			cout << strerror(errno) << "'" << endl;
			close(fd);
			return NULL;
		}

		packet = getNextTSPacketByPid(fd, NULL);
		if (packet == NULL) {
			close(fd);

			fileNames->erase(fileNames->begin());
			if (fileNames->empty()) {
				delete fileNames;
				fileNames = NULL;
			}

			if (packetHandler != NULL && packetHandler->count(fileName) != 0) {
				packetHandler->erase(packetHandler->find(fileName));
				if (packetHandler->empty()) {
					delete packetHandler;
					packetHandler = NULL;
				}
			}

			return getNextDataPacket();
		}

#ifdef WIN32
		if (fileName.find_last_of("\\") != std::string::npos) {
			file = fileName.substr(
					fileName.find_last_of("\\") + 1, fileName.length());
		}
#else
		if (fileName.find_last_of("/") != std::string::npos) {
			file = fileName.substr(
					fileName.find_last_of("/") + 1, fileName.length());
		}
#endif
		if (file.substr(0, 3) == "se_") {
			fileNames->erase(fileNames->begin());
			if (fileNames->empty()) {
				delete fileNames;
				fileNames = NULL;
			}

			close(fd);
			cout << "Muxer::getNextPacketData sending StreamEvent with pid = '";
			cout << packet->getPid() << "'" << endl;
			return packet;
		}

		if (packetHandler != NULL && packetHandler->count(fileName) != 0) {
			currentPacket = (*packetHandler)[fileName];
			currentPacket++;

			(*packetHandler)[fileName] = currentPacket;
			if (currentPacket == 1) {
				close(fd);
				/*
				cout << "Muxer::getNextDataPacket() sending packet(" << packet;
				cout << ") with pid = '" << packet->getPid() << "'" << endl;*/
				return packet;
			}

			for (l = 1; l < currentPacket; l++) {
				delete packet;
				packet = NULL;
				packet = getNextTSPacketByPid(fd, NULL);
				if (packet == NULL) {
					close(fd);
					break;
				}
			}

			if (packet != NULL) {
				packet->getPayload(payload);
				if ((payload[0] & 0xFF) == 0xFF &&
						(payload[1] & 0xFF) == 0xFF &&
						(payload[2] & 0xFF) == 0xFF &&
						(payload[3] & 0xFF) == 0xFF) {

					delete packet;
					packet = NULL;
				}
			}

			if (packet == NULL) {
				fileNames->erase(fileNames->begin());
				if (fileNames->empty()) {
					delete fileNames;
					fileNames = NULL;
				}

				if (packetHandler != NULL &&
						packetHandler->count(fileName) != 0) {

					packetHandler->erase(packetHandler->find(fileName));
					if (packetHandler->empty()) {
						delete packetHandler;
						packetHandler = NULL;
					}
				}

				close(fd);
				return getNextDataPacket();
			}

			close(fd);
			/*cout << "Muxer::getNextDataPacket() sending packet with";
			cout << " pid = '" << packet->getPid() << "'" << endl;*/
			return packet;
		}

		close(fd);
		fileNames->erase(fileNames->begin());
		return NULL;
	}

	TSPacket* Muxer::getNextTSPacketByPid(int fd, vector<unsigned int>* pids) {
		if (fd <= 0) {
			cout << "Muxer::getNextTSPacketByPid Warning! Invalid file ";
			cout << "descriptor received" << endl;
			return NULL;
		}

		int rval;
		vector<unsigned int>::iterator i;
		char packetData[TSPacket::TS_PACKET_SIZE];
		TSPacket* packet = NULL;
		unsigned int packPid;

		while (packet == NULL) {
			memset(packetData, 0, sizeof(packetData));
			rval = read(fd, (void*)(&packetData[0]), TSPacket::TS_PACKET_SIZE);
			if (rval != TSPacket::TS_PACKET_SIZE) {
				//TODO: make a better protection
				cout << "Muxer::getNextTSPacketByPid Warning! Can't ";
				cout << "read " << TSPacket::TS_PACKET_SIZE << " bytes" << endl;
				close(fd);
				return NULL;
			}

			numOfPackets++;
			packet = new TSPacket(packetData);
			packPid = packet->getPid();
			//cout << "current packet id = '";
			//cout << packet->getPid() << "'" << endl;

			if (pids == NULL) {
				return packet;
			}

			i = pids->begin();
			while (i != pids->end()) {
				if (packPid == *i) {
					return packet;
				}
				++i;
			}

			delete packet;
			packet = NULL;
		}
		return packet;
	}

	map<unsigned int, Pmt*>* Muxer::getProgramsInfo() {
		return pat->getProgramsInfo();
	}

	unsigned int Muxer::getTSId() {
		return pat->getTSId();
	}

	string Muxer::getTSFileName() {
		return this->tsFileName;
	}

	Pat* Muxer::getPat() {
		return pat;
	}

	void Muxer::muxCarousels(vector<CarouselInfo*>* infos) {
		vector<CarouselInfo*>::iterator i;
		Pmt* pmt;
		CarouselInfo* info;
		unsigned int pid, programNumber;

		cout << "muxing carousels" << endl;
		i = infos->begin();
		while (i != infos->end()) {
			info = *i;
			programNumber = info->programNumber;
			pid = info->pid;
			cout << "adding pid '" << pid << "' to pmt" << endl;
			pmt = pat->getPmtByProgramNumber(programNumber);
			if (pmt != NULL) {
				pmt->addElementaryStream(pid, Pmt::STREAM_TYPE_DSMCC_TYPE_B);
				pmt->addCarouselInfo(pid, info);
			}

			++i;
		}
		delete infos;
		infos = NULL;
	}

	void Muxer::muxStreamEvents(vector<SEInfo*>* infos) {
		vector<SEInfo*>::iterator i;
		Pmt* pmt;
		SEInfo* info;
		unsigned int pid, programNumber;

		i = infos->begin();
		while (i != infos->end()) {
			info = *i;
			programNumber = info->programNumber;
			pid = info->pid;

			pmt = pat->getPmtByProgramNumber(programNumber);
			if (pmt != NULL) {
				pmt->addElementaryStream(pid, Pmt::STREAM_TYPE_DSMCC_TYPE_C);
				pmt->addSEInfo(pid, info);
			}

			++i;
		}
		delete infos;
		infos = NULL;
	}
/*
	void Muxer::muxAppInformationTables(vector<AITInfo*>* infos) {

	}*/

	void Muxer::checkConsistency() {
		pat->checkConsistency();
	}

	void Muxer::setHasSdt(bool hasSdt) {
		this->hasSdt = hasSdt;
	}

	void Muxer::setHasEit(bool hasEit) {
		this->hasEit = hasEit;
	}

	void Muxer::setHasCdt(bool hasCdt) {
		this->hasCdt = hasCdt;
	}

	void Muxer::uSleep(double sleepValue) {
		double rest = 0;

		if (!initThreadFlags) {
			initThreadFlags = true;
			pthread_mutex_init(&threadFlagMutex, NULL);
			pthread_cond_init(&threadFlagConditionVariable, NULL);
		}

#ifdef WIN32
		if (sleepValue >= 1000) {
			rest = ((int)sleepValue) % 1000;
			sleepValue = (sleepValue - rest) / 1000;
			Sleep((long)sleepValue);

		} else {
			rest = sleepValue;
		}

		if (rest > 0) {
			int res;
			long int micro;
			struct timeval explicitTime;
			struct timespec timerOut;

			gettimeofday(&explicitTime, NULL);
			micro = explicitTime.tv_usec;

			/*cout << "Muxer::uSleep(" << sleepValue << ") tv_usec = '";
			cout << explicitTime.tv_usec << "' and rest = '" << rest;
			cout << "'" << endl;*/

			while (micro > 1000000) {
				explicitTime.tv_sec++;
				micro = micro - 1000000;
			}

			//cout << "Muxer::uSleep(" << sleepValue << ") beginning '";
			//cout << getCurrentTimeMillis() << "'" << endl;

			pthread_mutex_lock(&threadFlagMutex);

			timerOut.tv_sec = explicitTime.tv_sec;
			timerOut.tv_nsec = (micro * 1000) + (long(rest * 1000));

			res = pthread_cond_timedwait(
					&threadFlagConditionVariable,
					&threadFlagMutex,
					(const struct timespec*)(&timerOut));

			pthread_mutex_unlock(&threadFlagMutex);

			//cout << "Muxer::uSleep(" << sleepValue << ") ending '";
			//cout << getCurrentTimeMillis() << "'" << endl;

			//cout << "Muxer::uSleep(" << sleepValue << ") Sleep beginning '";
			//cout << getCurrentTimeMillis() << "'" << endl;
			//Sleep(1);
			//cout << "Muxer::uSleep(" << sleepValue << ") Sleep ending '";
			//cout << getCurrentTimeMillis() << "'" << endl;
		}
#else
		//in linux usleep, 1000000 is a second
		usleep(sleepValue);
#endif
	}
}
}
}
}
