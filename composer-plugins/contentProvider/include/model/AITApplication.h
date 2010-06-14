#ifndef AITAPPLICATION_H_
#define AITAPPLICATION_H_

#include <string>

using namespace std;

namespace org {
 namespace isdtv {
  namespace contentProvider {
   namespace model {
   						   
	class AITApplication {
		public:
			enum ControlCodeType { AUTOSTART_CONTROL_CODE = 0x01,
		   						   PRESENT_CONTROL_CODE = 0x02,
		   						   DESTROY_CONTROL_CODE = 0x03,
		   						   KILL_CONTROL_CODE = 0x04,
		   						   PREFETCH_CONTROL_CODE = 0x05, //ONLY FOR DVBHTML
		   						   REMOTE_CONTROL_CODE = 0x06 };
		   	
		   	enum VisibilityType { NO_VISIBILITY,
		   						  APLICATIONS_VISIBILIY,
		   						  FULL_VISIBILITY=3 };
		   						  
		   	enum TransportProtocolType { OC_PROTOCOL_TYPE = 0x0001,
		   								 IP_PROTOCOL_TYPE = 0x0002 };
		   	
		   	typedef struct VersionType {
		   		unsigned short major;
				unsigned short minor;
				unsigned short micro;
		   	};
		private:
			unsigned int organizationID; //32 BITS
			unsigned short applicationID;	//16 BITS
			
			ControlCodeType controlCode;
			
			unsigned short profile;
			VersionType version;
			
			bool serviceBound;
			VisibilityType visible;
			unsigned short priority; //8 BITS
			
			TransportProtocolType protocol;
			string name;
			string locator;
		public:
			AITApplication();
			virtual ~AITApplication();
			
			
			virtual unsigned int 	getOrganizationID();
			virtual void 			setOrganizationID(unsigned int newOrganizationID);
			virtual unsigned short 	getApplicationID();
			virtual void 			setApplicationID(unsigned short newApplicationID);
			
			virtual ControlCodeType getControlCode();
			virtual void 			setControlCode(ControlCodeType newControlCode);
			
			virtual unsigned short getProfile();
			virtual void 		   setProfile(unsigned short newProfile);
			
			virtual VersionType getVersion();
			virtual void 		setVersion(unsigned short majorVersion,unsigned short minorVersion,
					unsigned short microVersion);
			
			virtual VisibilityType 	getVisibility();
			virtual void 			setVisibility(VisibilityType visible);
			virtual unsigned short 	getPriority(); //8 BITS
			virtual void 			setPriority(unsigned short priority); //8 BITS
			
			virtual TransportProtocolType 	getProtocol();
			virtual void					setProtocol(TransportProtocolType newProtocol);
			
			virtual string 	getLocator();
			virtual void 	setLocator(string locator);
			virtual string 	getName();
			virtual void 	setName(string name);
			
	};
   }
  }
 }
}

#endif /*AITAPPLICATION_H_*/
