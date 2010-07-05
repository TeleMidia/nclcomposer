#include "../../include/util/Project.h"

Project::Project(QString projectId, QString location) {
    this->location  = location;
    this->projectId = projectId;
    documentFacade  = Document::getInstance();
}

Project::~Project() {
    QWriteLocker locker(&lockDocuments);
    QMapIterator<QString,NclDocument*> it(nclDocuments);
    while(it.hasNext()){
        documentFacade->releaseNclDocument(nclDocuments.take(it.key()));
        it.next();
    }
    nclDocuments.clear();
    documentFacade->releaseInstance();
}



bool Project::addDocument(QString documentId, NclDocument *nclDoc) {
    QReadLocker locker(&lockDocuments);
    if (!nclDocuments.contains(documentId)){
        locker.unlock();
        QWriteLocker lock(&lockDocuments);
        nclDocuments[documentId] = nclDoc;
        return true;
    } else { //document already exists

    }
    return false;
}

bool Project::removeDocument(QString documentId) {
        QReadLocker locker(&lockDocuments);
        if (nclDocuments.contains(documentId)) {
            locker.unlock();
            QWriteLocker lock(&lockDocuments);
            documentFacade->releaseNclDocument(nclDocuments.take(documentId));
            return true;
        }
        return false;

}

bool Project::deleteDocument(QString documentId) {
        if (!removeDocument(documentId)) return false;
        //TODO - deletar o arquivo no sistema de arquivos
}
