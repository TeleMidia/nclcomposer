#ifndef TSSECTION_H_
#define TSSECTION_H_

#include "../functions.h"

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace mpeg {
   class TSSection {
   		public:
   			static const unsigned short MAX_SECTION_SIZE = 4096;

   		protected:
   			// ISO/IEC 13818-1 TS SECTION HEADER
   			unsigned int tableId;
   			bool sectionSyntaxIndicator;

   			/*
   			 * number of bytes of the section, starting immediately following
   			 * the sectionLength field, and including CRC32. Thus, 9 bytes
   			 * of overhead.
   			 */
   			unsigned int sectionLength;

   			/*
   			 * transport_stream_id for PAT
   			 * program_number for PMT
   			 * table_id_extension for DSM_CC Section
   			 * service_id for EIT
   			 */
   			unsigned int idExtention;
   			unsigned int versionNumber;
   			bool currentNextIndicator;
   			unsigned int sectionNumber;
   			unsigned int lastSectionNumber;
   			char section[MAX_SECTION_SIZE];

   			unsigned int currentSize;

   		public:
   			TSSection();
   			TSSection(char streamData[184]);
   			virtual ~TSSection();

   			void process(char streamData[184]);

   		protected:
   			void processHeader(char streamData[184]);
   			void addPayloadData(char streamData[184]);

   		public:
   			unsigned int getTableId();
   			void setTableId(unsigned int id);
   			bool getSectionSyntaxIndicator();
   			void setSectionSyntaxIndicator(bool indication);
   			unsigned int getSectionLength();
   			unsigned int getExtensionId();
   			void setExtensionId(unsigned int id);
   			unsigned int getVersionNumber();
   			bool getCurrentNextIndicator();
   			unsigned int getSectionNumber();
   			unsigned int getLastSectionNumber();
   			void* getPayload();
   			unsigned int getPayloadSize();
   			void setVersionNumber(unsigned int versionNumber);
   			bool isConsolidated();
   			virtual bool processSectionPayload(){return false;};
   			virtual void print();
   			char * getSection();
   	};
   }
  }
 }
}

#endif /*TSSECTION_H_*/
