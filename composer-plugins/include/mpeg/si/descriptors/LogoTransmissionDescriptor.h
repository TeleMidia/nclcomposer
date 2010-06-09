#ifndef LOGOTRANSMISSIONDESCRIPTOR_H_
#define LOGOTRANSMISSIONDESCRIPTOR_H_

#include <string>
#include <cstring>
#include "../../Descriptor.h"

using namespace std;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {

    class LogoTransmissionDescriptor : public Descriptor {
	private:
		
	protected:
		unsigned char logoType;
		unsigned short logoId;
		unsigned short logoVersion;
		unsigned short downloadDataId;
		string logoName;
	public:
		LogoTransmissionDescriptor();
		~LogoTransmissionDescriptor();
		void setType(unsigned char type);
		unsigned char getType();
		void setLogoId(unsigned short id);
		unsigned short getLogoId();
		void setLogoVersion(unsigned short version);
		unsigned short getLogoVersion();
		void setDownloadDataId(unsigned short id);
		unsigned short getDownloadDataId();
		void setName(string name);
		string getName();
		unsigned char getNameLength();
};
}
}
}
}
}

#endif /*LOGOTRANSMISSIONDESCRIPTOR_H_*/
