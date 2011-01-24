#ifndef DESCRIPTOR_H_
#define DESCRIPTOR_H_

#include <iostream>
#include <vector>
#include <string>

using namespace std;

namespace org {
namespace isdtv {
namespace contentProvider {
namespace mpeg {
	class Descriptor {
		private:
	
		protected:
			unsigned char descriptorTag;
			unsigned char descriptorLength;
		public:
			Descriptor();
			~Descriptor();
			unsigned char getDescriptorTag();
			unsigned char getDescriptorLength();
			void setDescriptorLength(unsigned char length);
			static Descriptor* getDescriptor(
					vector<Descriptor*>* descriptors, unsigned char Tag);
			static vector<Descriptor*>* getDescriptors(
					vector<Descriptor*>* descriptors, unsigned char Tag);
			static int getDescriptorsLength(vector<Descriptor*>* descriptors);
};
}
}
}
}

#endif /*DESCRIPTOR_H_*/
