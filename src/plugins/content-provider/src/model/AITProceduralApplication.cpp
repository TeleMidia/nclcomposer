#include "../../include/model/AITProceduralApplication.h"

namespace org {
namespace isdtv {
namespace contentProvider {
namespace model {
	AITProceduralApplication::AITProceduralApplication() {
		
	}

	AITProceduralApplication::~AITProceduralApplication() {
		
	}

	string AITProceduralApplication::getParameter() {
		return parameter;
	}

	void AITProceduralApplication::setParameter(string newParameter) {
		parameter = newParameter;
	}

	string AITProceduralApplication::getBaseDirectory() {
		return baseDirectory;
	}

	void AITProceduralApplication::setBaseDirectory(string newBaseDirectory) {
		baseDirectory = newBaseDirectory;
	}

	string AITProceduralApplication::getClasspathExtension() {
		return classpathExtension;
	}

	void AITProceduralApplication::setClasspathExtension(
			string newClasspathExtension) {

		classpathExtension = newClasspathExtension;
	}

	string AITProceduralApplication::getInitialClass() {
		return initialClass;
	}

	void AITProceduralApplication::setInitialClass(string newInitialClass) {
		initialClass = newInitialClass;
	}
}
}
}
}
