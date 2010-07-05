#include "../../include/manager/CoreManager.h"

CoreManager::CoreManager(QObject *parent) :
    QObject(parent) {
    this->projectControl = new ProjectControl();
}

CoreManager::~CoreManager() {
    delete projectControl;
    projectControl = NULL;
}

void CoreManager::createProject(QString name, QString location){
    projectControl->createProject(name,location);
    QDir dir(location);
    QDir dirEx(location + "/" + name);

    if(dir.exists()) {
        if(dirEx.exists()) {
            emit onError(tr("A directory with this name already exists"));
            return;
        }
        if(dir.mkdir(name)) {
            emit projectCreated(name,location);
        } else {
            emit onError(tr("Directory for this project could not be created"));
        }
    } else {
        emit onError(tr("The choosen directory does not exists."));
    }
}

void CoreManager::addDocument(QString name,QString location,
                              QString projectId) {

    projectControl->addDocument(projectId,location,name);
}

void CoreManager::createDocument(QString name, QString projectId) {

}


