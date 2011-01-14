#ifndef DOCUMENTPARSER_H
#define DOCUMENTPARSER_H

#include <QObject>
#include <QFile>
#include <QtDebug>
#include <QMutex>
#include <QXmlDefaultHandler>
#include <QStack>

/*namespace composer {
namespace core {
namespace plugin {
class DocumentParser;
} } } */

#include <map>
#include <string>
using namespace std;

#include "../plugin/IPlugin.h"
using namespace composer::core::plugin;

#include <model/ncm/Entity.h>
#include <model/ncm/NclDocument.h>
using namespace composer::model::ncm;

#include <model/util/EntityUtil.h>
using namespace composer::model::util;

namespace composer {
namespace core {
namespace util {

    class DocumentParser : public IPlugin, public QXmlDefaultHandler
{
    Q_OBJECT
    public:
        DocumentParser(QString documentId, QString projectId);
        DocumentParser();
        ~DocumentParser();

        bool setUpParser(QString uri);
        bool parseDocument();

        bool listenFilter(EntityType );

        QWidget* getWidget() { return NULL;}

        inline QString getProjectId() {
            return this->projectId;
        }

        inline QString getDocumentId() {
            return this->documentId;
        }

    protected:
        bool startElement(const QString &namespaceURI,
                          const QString &localName,
                          const QString &qName,
                          const QXmlAttributes &attributes);
        bool endElement(const QString &namespaceURI,
                        const QString &localName,
                        const QString &qName);
        bool characters(const QString &str);
        bool fatalError(const QXmlParseException &exception);

    private:
        QMutex mutex;
        QString documentId;
        QString projectId;
        QFile *file;
        EntityUtil *util;
        QStack<Entity*> elementStack;


   public slots:
        void onEntityAdded(QString ID, Entity *);
        void onEntityAddError(string error);
        /** TODO Lembrar se ele tiver mudado o ID */
        void onEntityChanged(QString ID, Entity *);
        void onEntityChangeError(string error);
        /** Lembrar de ele apagar a sua referência interna */
        void onEntityAboutToRemove(Entity *);
        void onEntityRemoved(QString ID, string entityID);
        void onEntityRemoveError(string error);

};

}
}
}

#endif // DOCUMENTPARSER_H

