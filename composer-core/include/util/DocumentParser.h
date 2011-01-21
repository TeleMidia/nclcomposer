#ifndef DOCUMENTPARSER_H
#define DOCUMENTPARSER_H

#include <QObject>
#include <QFile>
#include <QtDebug>
#include <QMutex>
#include <QXmlDefaultHandler>
#include <QStack>


#include "../extension/plugin/IPlugin.h"
using namespace composer::core::extension::plugin;

#include "../model/Entity.h"
#include "../model/Document.h"
using namespace composer::core::model;

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
        QStack<Entity*> elementStack;


   public slots:
        void onEntityAdded(QString ID, Entity *);
        void onEntityAddError(QString error);
        /** TODO Lembrar se ele tiver mudado o ID */
        void onEntityChanged(QString ID, Entity *);
        void onEntityChangeError(QString error);
        /** Lembrar de ele apagar a sua referência interna */
        void onEntityAboutToRemove(Entity *);
        void onEntityRemoved(QString ID, QString entityID);
        void onEntityRemoveError(QString error);

};

}
}
}

#endif // DOCUMENTPARSER_H

