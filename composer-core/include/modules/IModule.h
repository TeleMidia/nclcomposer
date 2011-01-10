#ifndef IMODULE_H
#define IMODULE_H

#include <QObject>

namespace composer{
    namespace core {
        namespace module{

    class IModule : public QObject {
        Q_OBJECT
        public:
            virtual ~IModule() {};
            /** WARNING: THIS FUNCTIONS MUST BE REIMPLEMENTED */
            static IModule* getInstance() { return NULL; };
            static void     releaseInstance() { return; };
    };

        }
    }
}


#endif // IMODULE_H
