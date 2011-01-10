#include "../../include/modules/ModuleControl.h"
#include "../../include/modules/MessageControl.h"
#include "../../include/modules/ProjectControl.h"

namespace composer {
namespace core {
namespace module {


ModuleControl::ModuleControl() {
    messageControl = new MessageControl();
    projectControl = new ProjectControl();
}

ModuleControl::~ModuleControl(){
    messageControl->releaseInstance();
    projectControl->releaseInstance();
}

IModule* ModuleControl::getInstance(){
    QMutexLocker locker(&instMutex);
    if (!instance) {
        instance = new ModuleControl();
    }
    return instance;
}

void     ModuleControl::releaseInstance() {
    QMutexLocker locker(&instMutex);
    if (instance != NULL) {
        delete instance;
        instance = NULL;
    }
}

IModule* ModuleControl::getModule(QString moduleName) {
    moduleName.toLower();
    if (moduleName == "message")
            return messageControl;
    if (moduleName == "project")
            return projectControl;
    if (moduleName == "module") {
            //return this->getInstance();
            //return ModuleControl::getInstance();
        QMutexLocker locker(&instMutex);
        if (!instance) {
            instance = new ModuleControl();
        }
        return instance;
    }
    return NULL;
}

}
}
}
