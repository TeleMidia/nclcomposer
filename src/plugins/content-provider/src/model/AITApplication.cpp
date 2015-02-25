#include "../../include/model/AITApplication.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace model {
	AITApplication::AITApplication() {
		organizationID = 0;
		applicationID = 0;
		controlCode = AUTOSTART_CONTROL_CODE;
		profile = 0;
		version.major = 0;
		version.minor = 0;
		version.micro = 0;
		serviceBound=false;
		visible = FULL_VISIBILITY;
		priority = 0;
		protocol = OC_PROTOCOL_TYPE;
	}

	AITApplication::~AITApplication() {
		
	}

	unsigned int AITApplication::getOrganizationID() {
		return organizationID;
	}

	void AITApplication::setOrganizationID(unsigned int newOrganizationID) {
		organizationID = newOrganizationID;
	}

	unsigned short AITApplication::getApplicationID() {
		return applicationID;
	}

	void AITApplication::setApplicationID(unsigned short newApplicationID) {
		applicationID = newApplicationID;
	}

	AITApplication::ControlCodeType AITApplication::getControlCode() {
		return controlCode;
	}

	void AITApplication::setControlCode(ControlCodeType newControlCode) {
		controlCode = newControlCode;
	}

	unsigned short AITApplication::getProfile() {
		return profile;
	}

	void AITApplication::setProfile(unsigned short newProfile) {
		profile = newProfile;
	}

	AITApplication::VersionType AITApplication::getVersion() {
		return version;
	}

	void AITApplication::setVersion(
			unsigned short majorVersion,
			unsigned short minorVersion,
			unsigned short microVersion) {

		version.major = majorVersion;
		version.minor = minorVersion;
		version.micro = microVersion;
	}

	AITApplication::VisibilityType AITApplication::getVisibility() {
		return visible;
	}

	void AITApplication::setVisibility(VisibilityType visible) {
		this->visible = visible;
	}

	unsigned short AITApplication::getPriority() {
		return priority;
	}

	void AITApplication::setPriority(unsigned short priority) {
		if (priority > 0xF) {
			this->priority = 0xF;

		} else {
			this->priority = priority;
		}
	}

	AITApplication::TransportProtocolType AITApplication::getProtocol() {
		return protocol;
	}

	void AITApplication::setProtocol(TransportProtocolType newProtocol) {
		protocol = newProtocol;
	}

	string AITApplication::getName() {
		return name;
	}

	void AITApplication::setName(string name) {
		this->name = name;
	}

	string 	AITApplication::getLocator() {
		return locator;
	}

	void AITApplication::setLocator(string locator) {
		this->locator = locator;
	}
}
}
}
}
