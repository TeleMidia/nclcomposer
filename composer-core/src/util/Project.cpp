#include "util/Project.h"

namespace composer {
namespace core {
namespace util {


Project::Project(QString projectId, QString location) {
    this->location  = location;
    this->projectId = projectId;
    qDebug() << "Project::Project(" << projectId << ", " << location << ")";
}

Project::~Project() {
    QWriteLocker locker(&lockDocuments);
    QMapIterator<QString,Document*> it(documents);

    while(it.hasNext()){
        it.next();
        Document *doc = it.value();
        delete doc;
        doc = NULL;
        if (documents.remove(it.key()) != 1)
             qDebug() << "Não conseguiu remover do mapa";
    }
}



bool Project::addDocument(QString documentId, Document *doc) {
    QReadLocker locker(&lockDocuments);
    if (!documents.contains(documentId)){
        locker.unlock();
        QWriteLocker lock(&lockDocuments);
        documents[documentId] = doc;
        qDebug() << "Project::addDocument(" << documentId << ", "
                 << projectId << ")";
        return true;
    } else { //document already exists

    }
    return false;
}

bool Project::removeDocument(QString documentId) {
        QReadLocker locker(&lockDocuments);
        if (documents.contains(documentId)) {
            locker.unlock();
            QWriteLocker lock(&lockDocuments);
            Document *doc = documents.take(documentId);
            delete doc;
            doc = NULL;
            return true;
        }
        return false;

}

bool Project::deleteDocument(QString documentId) {
        if (!removeDocument(documentId)) return false;
        //TODO - deletar o arquivo no sistema de arquivos
}

}
}
}
