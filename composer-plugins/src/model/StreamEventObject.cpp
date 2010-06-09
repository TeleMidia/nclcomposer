#include "../../include/model/StreamEventObject.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace model {
	StreamEventObject::StreamEventObject() {
		id = 0;
		name = "";
	}

	StreamEventObject::~StreamEventObject() {
		
	}

	int StreamEventObject::getID() {
		return id;
	}

	void StreamEventObject::setID(int newID) {
		id = newID;
	}

	string StreamEventObject::getName() {
		return name;
	}

	void StreamEventObject::setName(string newName) {
		name = newName;
	}
}
}
}
}
