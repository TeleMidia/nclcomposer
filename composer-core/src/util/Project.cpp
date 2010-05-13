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



NclDocument* Project::addDocument(QString documentId, QString uri, bool exists) {
    if (!exists) {
        map<string,string> atts;
        atts["id"] = documentId.toStdString();
        atts["uri"] = uri.toStdString();
        return documentFacade->createNclDocument(atts);
    }
}

bool Project::removeDocument(QString documentId) {
        lockDocuments.lockForRead();
        if (nclDocuments.contains(documentId)) {
            lockDocuments.unlock();
            lockDocuments.lockForWrite();
            documentFacade->releaseNclDocument(nclDocuments.take(documentId));
            lockDocuments.unlock();
            return true;
        }
        lockDocuments.unlock();
        return false;

}

bool Project::deleteDocument(QString documentId) {
        if (!removeDocument(documentId)) return false;
        //TODO - deletar o arquivo no sistema de arquivos
}
