#ifndef NETWORKNAMEDESCRIPTOR_H_
#define NETWORKNAMEDESCRIPTOR_H_

#include <string.h>
using namespace std;

#include "../../Descriptor.h"
using namespace ::org::isdtv::contentProvider::mpeg;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
namespace si {

	class NetworkNameDescriptor : public Descriptor {
	private:

	protected:
		string text;
	public:
		NetworkNameDescriptor();
		~NetworkNameDescriptor();
		void setName(string networkName);
		string getName(void);
		int encodeNetworkName(char* section);
};
}
}
}
}
}

#endif /*NETWORKNAMEDESCRIPTOR_H_*/
