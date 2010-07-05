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

#include "../manager/MessageControl.h"
using namespace manager;

#include <model/ncm/NclDocument.h>
using namespace ncm;

#include "EntityUtil.h"

//TODO - fazer essa classe singleton

class DocumentParser : public QObject
{
    Q_OBJECT
    public:
        DocumentParser();
        ~DocumentParser();

        bool setUpParser(QString uri);
        bool parseDocument();
        bool parseElement(QDomElement element);
        inline void setNclDocument(NclDocument *document) {
            QMutexLocker locker(&mutex);
            this->nclDoc = document;
        }
        inline NclDocument* getNclDocument() {
            QMutexLocker locker(&mutex);
            return this->nclDoc;
        }

    private:
        QDomDocument nclDomDocument;
        QMutex mutex;
        MessageControl *messageControl;
        EntityUtil *util;
        NclDocument *nclDoc;
        inline void setDomDocument(QDomDocument document) {
            QMutexLocker locker(&mutex);
            this->nclDomDocument = document;
        }

    public slots:
         void onDocumentCreated(NclDocument *nclDoc);
    signals:
        void addEntity( EntityType entity,
                       string parentEntityId, map<string,string>& atts,
                       bool force);
        void createDocument(map<string,string> &atts);

};

#endif // DOCUMENTPARSER_H

