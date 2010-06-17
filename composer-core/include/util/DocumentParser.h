#ifndef DOCUMENTPARSER_H
#define DOCUMENTPARSER_H

#include <QObject>
#include <QDomDocument>
#include <QFile>
#include <QtDebug>
#include <QMutex>

#include <map>
using namespace std;

#include "../manager/MessageControl.h"
using namespace manager;

#include "EntityUtil.h"

//TODO - fazer essa classe singleton

class DocumentParser : public QObject
{
    Q_OBJECT
    public:
        DocumentParser();
        ~DocumentParser();
        QDomDocument nclDomDocument;
        QMutex mutex;
        bool setUpParser(QString uri);
        bool parseDocument();
        bool parseElement(QDomElement element, QDomElement parentElement);

    private:
        MessageControl *messageControl;
        EntityUtil *util;
        inline void setDomDocument(QDomDocument document) {
            QMutexLocker locker(&mutex);
            this->nclDomDocument = document;
        }

    signals:
        void addEntity( EntityType entity,
                       string parentEntityId, map<string,string>& atts,
                       bool force);

};

#endif // DOCUMENTPARSER_H

