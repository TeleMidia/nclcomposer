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

/*namespace composer {
namespace core {
namespace plugin {
class DocumentParser;
} } } */

#include "../plugin/IPlugin.h"
using namespace composer::core::plugin;

#include <model/ncm/NclDocument.h>
using namespace ncm;

#include "../modules/MessageControl.h"
using namespace composer::core::module;

#include "EntityUtil.h"
using namespace composer::core::util;


//TODO - Modelar para ter 1 DocumentParser por Project
namespace composer {
namespace core {
namespace plugin {

    class DocumentParser : public IPlugin
{
    Q_OBJECT
    public:
        DocumentParser(QString documentId, QString projectId);
        ~DocumentParser();

        bool setUpParser(QString uri);
        bool parseDocument();
        bool parseElement(QDomElement element);

        bool listenFilter(EntityType );

        QWidget* getWidget() { return NULL;}

        inline QString getProjectId() {
            return this->projectId;
        }

        inline QString getDocumentId() {
            return this->documentId;
        }

    private:
        QDomDocument nclDomDocument;
        QMutex mutex;
        QString documentId;
        QString projectId;

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
        void documentParsed(QString projectId, QString documentId);

};

}
}
}

#endif // DOCUMENTPARSER_H

