#include "../../include/util/CoreManager.h"

namespace composer {
namespace core {
namespace util {

CoreManager::CoreManager(QObject *parent) :
    QObject(parent) {
    this->projectControl = new ProjectControl();
}

CoreManager::~CoreManager() {
    delete projectControl;
    projectControl = NULL;
}

void CoreManager::addProject(QString projectId,QString location) {

    QDir dir(location);

    if (dir.exists()) {

        projectControl->createProject(projectId,location);
        emit projectCreated(projectId,location);

        QStringList filters;
        filters << "*.ncl";
        dir.setNameFilters(filters);
        dir.setFilter(QDir::Files | QDir::AllDirs);
        dir.setSorting(QDir::DirsFirst | QDir::Name);
    } else { //dir dont exists
        emit onError(tr("Project %1 could not be open,"
                        "maybe the path (%2) has been changed")
                     .arg(projectId).arg(location));
    }
}

void CoreManager::addFilesInDirectory(QDir dir, QString projectId) {
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
       QFileInfo fileInfo = list.at(i);
       if (fileInfo.isFile()) {
           QString fileName = fileInfo.fileName();
           QString filePath = fileInfo.absoluteFilePath();
           if (fileName.endsWith(".ncl")) {
               addDocument(fileName,filePath,projectId,false);
               return;
           }
       } else if (fileInfo.isDir()) {
           //TODO adicionar diretorio no projectTree
           //TODO testar emitindo sinal de projetoCriado
           addFilesInDirectory(fileInfo.dir(),projectId);
       }

     }
}

void CoreManager::createProject(QString name, QString location){
    QDir dir(location);
    QDir dirEx(location + "/" + name);

    if(dir.exists()) {
        if(dirEx.exists()) {
            emit onError(tr("A directory with this name already exists"));
            return;
        }
        if(dir.mkdir(name)) {
            projectControl->createProject(name,location);
            emit projectCreated(name,location);
        } else {
            emit onError(tr("Directory for this project could not be created"));
        }
    } else {
        emit onError(tr("The choosen directory does not exists."));
    }
}

void CoreManager::addDocument(QString name,QString location,
                              QString projectId, bool copy) {

    if(projectControl->addDocument(projectId,location,name,copy)) {
        emit documentAdded(projectId,name,location);
    } else {
        emit onError(tr("Could not add the choosen NCL document"));
    }
}



void CoreManager::createDocument(QString name, QString projectId) {

}

bool CoreManager::saveSettings() {
    if (!projectControl->saveAllProjects()) {
        emit onError(tr("Could not save the current projects"));
        return false;
    }
    return true;
}

}
}
}
