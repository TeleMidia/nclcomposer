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
            virtual IModule* getInstance() = 0;
            virtual void     releaseInstance() = 0;
    };

        }
    }
}


#endif // IMODULE_H
