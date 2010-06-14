#include "../../include/mpeg/TSPacket.h"
#include "../../include/mpeg/Muxer.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace mpeg {
	TSPacket::TSPacket(char streamData[TS_PACKET_SIZE]) {
		memset(payload, 0, sizeof(payload));
		payloadSize = 184;
		pcr = 0;
		memcpy((void*)(&packData[0]), (void*)(&streamData[0]), TS_PACKET_SIZE);
		process(streamData);
	}

	TSPacket::TSPacket() {
		pcr = 0;
	}

	TSPacket::~TSPacket() {

	}

	void TSPacket::process(char streamData[TS_PACKET_SIZE]) {
		syncByte = (streamData[0] & 0xFF);
		if (syncByte != TS_PACKET_SYNC_BYTE) {
			cout << "TSPacket::process warning! syncByte != 0x47" << endl;
			payloadSize = 0;
			return;
		}

		transportErrorIndication = (streamData[1] & 0x80) >> 7;
		payloadUnitStartIndicator = (streamData[1] & 0x40) >> 6;
		transportPriority = (streamData[1] & 0x20) >> 5;
		pid = (((streamData[1] & 0x1F) << 8) | (streamData[2] & 0xFF));

		transportScramblingControl = (streamData[3] & 0xC0) >> 6;
		adaptationFieldControl = (streamData[3] & 0x30) >> 4;
		continuityCounter = (streamData[3] & 0x0F);
		if (getAdaptationFieldControl() == PAYLOAD_ONLY) {
			/*
			 * Check if the ts packet have a section (true) or a PES (false)
			 * if (true) we have a pointer field if has start indicator
			 */
			if (Muxer::getInstance()->isSectionStream(pid)) {
				if (getStartIndicator()) {
					unsigned int pointerField;
					pointerField = (streamData[4] & 0xFF);
					if (pointerField == 0) {
						payloadSize = 183;
						memcpy((void*)(&payload[0]),
							    (void*)(&streamData[5]), 183);

					} else if (pointerField < 183) {
						payloadSize = 183 - pointerField;
						memcpy((void*)(&payload[0]),
							    (void*)(&streamData[5 + pointerField]),
							    183 - pointerField);
					}

				} else {
					memcpy((void*)(&payload[0]),
						    (void*)(&streamData[4]), 184);
				}

			} else {
				// TS pack is not to filter
				payloadSize = 0;
			}

		} else if (getAdaptationFieldControl() == ADAPT_PAYLOAD) {
			unsigned int adaptationFieldLength;
			bool pcrFlag, oPcrFlag;
			uint64_t progClockReferenceBase, progClockReferenceExt;

			adaptationFieldLength = (streamData[4] & 0xFF);
			if (adaptationFieldLength >= 1) {
				pcrFlag = (bool)(streamData[5] & 0x10);
				oPcrFlag = (bool)(streamData[5] & 0x08);
				if (pcrFlag || oPcrFlag) {
					progClockReferenceBase = (((streamData[6] & 0xFF) << 25) |
							((streamData[7] & 0xFF) << 17) |
							((streamData[8] & 0xFF) << 9) |
							((streamData[9] & 0xFF) << 1) |
							((streamData[10] & 0x80) >> 7));

					progClockReferenceExt = (((streamData[10] & 0x01) << 8) |
							(streamData[11] & 0xFF));

					pcr = ((progClockReferenceBase * 300) +
							progClockReferenceExt);
				}
			}

			// mais um porque o adaptationLength conta os bytes de adaptacao,
			// como queremos o proximo que nao eh de adaptacao...
			adaptationFieldLength++;
			if (syncByte == 0x47 && adaptationFieldLength <= 183) {
				unsigned int pointerField;
				if (payloadUnitStartIndicator) {
					if (adaptationFieldLength < 183) {
						pointerField = (streamData[
							    4 + adaptationFieldLength] & 0xFF);

					} else {
						pointerField = 188;
					}

				} else {
					pointerField = 0;
				}

				if (pointerField == 0 && adaptationFieldLength < 183) {
					payloadSize = 184 - adaptationFieldLength - 1;
					memcpy((void*)(&payload[0]), (void*)(
						    &streamData[4 + adaptationFieldLength + 1]),
						    payloadSize);

				} else if (adaptationFieldLength + pointerField < 183) {
					payloadSize = 184 - adaptationFieldLength - pointerField;
					memcpy((void*)(&payload[0]), (void*)(&streamData[4 +
						    adaptationFieldLength + 1 + pointerField]),
						    payloadSize);

				} else {
					payloadSize = 0;
				}

			} else {
				payloadSize = 0;
			}

		} else if (getAdaptationFieldControl() == NO_PAYLOAD) {
			// no payload
			// TODO: do we use adaptation_field?
			payloadSize = 0;
			unsigned int adaptationFieldLength;
			bool pcrFlag, oPcrFlag;
			uint64_t progClockReferenceBase, progClockReferenceExt;

			adaptationFieldLength = (streamData[4] & 0xFF);
			if (adaptationFieldLength >= 1) {
				pcrFlag = (bool)(streamData[5] & 0x10);
				oPcrFlag = (bool)(streamData[5] & 0x08);
				if (pcrFlag || oPcrFlag) {
					progClockReferenceBase = (((streamData[6] & 0xFF) << 25) |
							((streamData[7] & 0xFF) << 17) |
							((streamData[8] & 0xFF) << 9) |
							((streamData[9] & 0xFF) << 1) |
							((streamData[10] & 0x80) >> 7));

					progClockReferenceExt = (((streamData[10] & 0x01) << 8) |
							(streamData[11] & 0xFF));

					pcr = ((progClockReferenceBase * 300)
							+ progClockReferenceExt);
				}
			}
		}
	}

	unsigned int TSPacket::getPid() {
		return pid;
	}

	void TSPacket::getPacketData(char streamData[TS_PACKET_SIZE]) {
		streamData[0] = TS_PACKET_SYNC_BYTE;
		streamData[1] = ((transportErrorIndication << 7) & 0x80) |
				((payloadUnitStartIndicator << 6) & 0x40) |
				((transportPriority << 5) & 0x20) |
				(((pid & 0xFF00) >> 8) & 0x1F);

		streamData[2] = (pid & 0x00FF);
		streamData[3] = ((transportScramblingControl << 6) & 0xC0) |
				((adaptationFieldControl << 4) & 0x30) |
				(continuityCounter & 0x0F);

		memcpy((void*)(&streamData[4]), (void*)(&packData[4]), 184);
	}

	void TSPacket::getPayload(char streamData[184]) {
		memcpy((void*)(&streamData[0]), (void*)(&payload[0]), 184);
	}

	unsigned int TSPacket::getPayloadSize() {
		return payloadSize;
	}

	bool TSPacket::getStartIndicator() {
		return payloadUnitStartIndicator;
	}

	unsigned int TSPacket::getAdaptationFieldControl() {
		return adaptationFieldControl;
	}

	unsigned int TSPacket::getContinuityCounter() {
		return continuityCounter;
	}

	void TSPacket::setContinuityCounter(unsigned int counter) {
		continuityCounter = counter;
	}

	uint64_t TSPacket::getPCRValue() {
		return pcr;
	}

	void TSPacket::print() {
		unsigned int i;
		cout << "TS PACK" << endl;
		cout << "pid = " << hex << pid << endl;

		cout << "payloadUnitStartIndicator = ";
		cout << hex << payloadUnitStartIndicator << endl;

		cout << "payloadSize = " << hex << payloadSize << endl;

		cout << "transportErrorIndication = ";
		cout << hex << transportErrorIndication << endl;

		cout << "payloadUnitStartIndicator = ";
		cout << hex << payloadUnitStartIndicator << endl;

		cout << "transportPriority = " << hex << transportPriority;
		cout << endl;

		cout << "transportScramblingControl = ";
		cout << hex << transportScramblingControl << endl;

		cout << "adaptationFieldControl = " << hex << adaptationFieldControl;
		cout << endl;

		cout << "continuityCounter = " << hex << continuityCounter;
		cout << endl;

		char pl[184];
		getPayload(pl);
		cout << "payload: " << endl;

		for (i=0; i < 184; i++) {
			cout << hex << (pl[i] & 0xFF) << " ";
		}
		cout << endl << endl;
	}

	void TSPacket::tsPacketFactory(
			bool append, unsigned short pId,
			unsigned char adaptation,
			unsigned char unitStart,
			vector<SectionInfo*>* sections,
			string filename) {

		SectionInfo* si;
		vector<SectionInfo*>::iterator it;
		FILE* fp;
		unsigned short sectionPos, tsPos, remainingBytes, jmp;
		unsigned char continuity = 0;
		bool firstPacket = true;
		char buffer[TS_PACKET_SIZE];

		if (unitStart > 183) {
			cout << "payload unit start too long: " << unitStart << endl;
			return;
		}

		if (append) {
			fp = fopen(filename.c_str(), "a+b");

		} else {
			fp = fopen(filename.c_str(), "wb");
		}

		if (fp == NULL) {
			cout << "cannot open output file. (" << filename << ")" << endl;
			return;
		}

		it = sections->begin();
		while (it!=sections->end()) {
			si = *it;
			sectionPos = 0;
			firstPacket = true;
			while (sectionPos < (si->length)) {
				tsPos = 0;
				memset(buffer, 0, TS_PACKET_SIZE);

				// sync_byte
				buffer[tsPos] = TS_PACKET_SYNC_BYTE;
				tsPos++;

				// transport_error_indicator
				if (transportErrorIndication) {
					buffer[tsPos] = 0x80;
				}

				// payload_unit_start_indicator
				if (firstPacket) {
					buffer[tsPos] = buffer[tsPos] | 0x40;
					firstPacket = false;
				}

				// PID
				buffer[tsPos] = buffer[tsPos] | ((pId & 0xFF00) >> 8);
				tsPos++;
				buffer[tsPos] = buffer[tsPos] | (pId & 0xFF);
				tsPos++;

				// transport_scrambling_control
				buffer[tsPos] = 0x00;
				// adaptation_field_control
				buffer[tsPos] = buffer[tsPos] | ((adaptation & 0xFF) << 4);
				// continuity_counter
				buffer[tsPos] = buffer[tsPos] | (continuity & 0xFF);
				tsPos++;
				continuity++;
				if (continuity == 0x10) {
					continuity = 0;
				}

				//TODO: WRONG WAY! FIX IT ASAP!!!
				if ((adaptation == 0x02) || (adaptation == 0x03)) {
					// Audio || Video

				} else if (adaptation == 0x01) {
					// Sections
					if (((buffer[1] & 0x40) >> 6) == 0x01) {
						buffer[tsPos] = unitStart;
						jmp = unitStart + 1;
						tsPos += jmp;

					} else {
						jmp = 0;
					}
					remainingBytes = si->length - sectionPos;
					if (remainingBytes > (184 - jmp)) {
						remainingBytes = 184 - jmp;
					}
					memcpy(buffer+tsPos, (si->data)+sectionPos, remainingBytes);
					sectionPos += remainingBytes;
					// write file
					if ((unsigned int)TS_PACKET_SIZE !=
							fwrite(buffer, 1, TS_PACKET_SIZE, fp)) {

						cout << "cannot write output file. (";
						cout << filename << ")" << endl;
						return;
					}

				} else {
					cout << "unrecognized adaptation field control: "
						<< adaptation << endl;
				}
			}
			it++;
		}
		fclose(fp);

		cout << "TS Packets encoded successfully." << endl;
	}
   }
  }
 }
}
