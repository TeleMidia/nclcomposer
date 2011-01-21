#ifndef PROJECT_H
#define PROJECT_H

#include <QReadWriteLock>
#include <QString>
#include <QMap>
#include <QDir>
#include <QDebug>

#include "../model/Document.h"
using namespace composer::core::model;

namespace composer {
namespace core {
namespace util {

class Project {
    private:
        QReadWriteLock lockString;
        QReadWriteLock lockDocuments;
        /*top dir of the project excluding the project name and
        separator */
        QString location;
        QString projectId;
        QMap<QString, Document*> documents;

    public:
        Project(QString, QString);
        ~Project();
        inline QString getLocation() {
            QReadLocker locker(&lockString);
            return this->location;
        }

        inline QString getProjectId() {
            QReadLocker locker(&lockString);
            return this->projectId;
        }
        inline Document* getDocument(QString documentId) {
                QReadLocker locker(&lockDocuments);
                return documents.contains(documentId) ?
                       documents.value(documentId) : NULL;
        }
        inline QMap<QString, Document*> getNclDocuments() {
                QReadLocker locker(&lockDocuments);
                return documents;
        }
        bool addDocument(QString documentId, Document*);
        bool removeDocument(QString documentId);
        bool deleteDocument(QString documentId);

};
}
}
}

#endif // PROJECT_H
