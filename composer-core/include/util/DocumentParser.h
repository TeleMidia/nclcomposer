#ifndef DOCUMENTPARSER_H
#define DOCUMENTPARSER_H

#include <QObject>
#include <QDomDocument>
#include <QFile>
#include <QtDebug>
#include <QMutex>
#include <QDomNodeList>

#include <map>
using namespace std;

#include "../plugin/IPluginMessage.h"
using namespace composer::core::plugin;

#include <model/ncm/NclDocument.h>
using namespace ncm;

#include "../modules/ModuleControl.h"
using namespace composer::core::module;

#include "EntityUtil.h"
using namespace composer::core::util;

namespace composer {
namespace core {
namespace plugin {

    class DocumentParser : public IPluginMessage
{
    Q_OBJECT
    public:
        DocumentParser();
        ~DocumentParser();

        bool setUpParser(QString uri);
        bool parseDocument();
        bool parseElement(QDomElement element);

        bool listenFilter(EntityType );

    private:
        QDomDocument nclDomDocument;
        QMutex mutex;
        EntityUtil *util;

        inline void setDomDocument(QDomDocument document) {
            QMutexLocker locker(&mutex);
            this->nclDomDocument = document;
        }


   public slots:
        void onEntityAdded(Entity *);
        void onEntityAddError(string error);
        /** TODO Lembrar se ele tiver mudado o ID */
        void onEntityChanged(Entity *);
        void onEntityChangeError(string error);
        /** Lembrar de ele apagar a sua referência interna */
        void onEntityAboutToRemove(Entity *);
        void onEntityRemoved(string entityID);
        void onEntityRemoveError(string error);

    signals:
        void createDocument(map<string,string> &atts);

};

}
}
}

#endif // DOCUMENTPARSER_H

