#ifndef MODULECONTROL_H
#define MODULECONTROL_H

#include <QMutex>
#include <QMutexLocker>

#include "IModule.h"
#include "MessageControl.h"
#include "ProjectControl.h"
using namespace composer::core::module;

namespace composer {
namespace core {
namespace module {

    class ModuleControl : public IModule {
        public:
            IModule* getInstance();
            void     releaseInstance();
            static IModule*  getModule(QString moduleName);

        private:
            ModuleControl();
            ~ModuleControl();

        private:
            static ModuleControl* instance;
            static QMutex instMutex;
            /* Concetrates all the instances of teh Core Modules */
            static MessageControl *messageControl;
            static ProjectControl *projectControl;


};

}
}
}

#endif // MODULECONTROL_H
