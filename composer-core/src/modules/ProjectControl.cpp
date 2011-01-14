#include "../../include/modules/ProjectControl.h"

namespace composer {
namespace core {
namespace module {

    ProjectControl::ProjectControl() {
        this->activeProject = NULL;
//        connect(PluginControl::getInstance(),
//                SIGNAL(newDocumentLaunchedAndCreated(QString,QString)),
//                this,
//                SIGNAL(documentAdded(QString,QString)));
    }

    ProjectControl::~ProjectControl() {
        QWriteLocker locker(&lockProjects);
        QMapIterator<QString,Project*> it(projects);
        while(it.hasNext()){
            it.next();
            Project *p = it.value();
            projects.remove(it.key());
            delete p;
            p = NULL;
        }
        projects.clear();
    }

    Project* ProjectControl::createProject(QString projectId,
                                       QString location) {

        qDebug() << "ProjectControl::createProject(" << projectId << ", "
                 << location << ")";

        lockProjects.lockForRead();
        if (!projects.contains(projectId)) {
            lockProjects.unlock();
            Project *p = new Project(projectId,location);
            lockProjects.lockForWrite();
            projects[projectId] = p;
            lockProjects.unlock();
            return p;
        }
        lockProjects.unlock();
        return NULL;
    }

    bool ProjectControl::activateProject (QString projectId){
        lockProjects.lockForRead();
        if (projects.contains(projectId)) {
            lockProjects.unlock();
            lockProjects.lockForWrite();
            this->activeProject = projects[projectId];
            lockProjects.unlock();
            return true;
        }
        lockProjects.unlock();
        return false;
    }
    bool ProjectControl::deactivateProject (QString projectId){
        lockProjects.lockForRead();
        if (projects.contains(projectId)) {
            lockProjects.unlock();
            lockProjects.lockForWrite();
            this->activeProject = NULL;
            lockProjects.unlock();
            return true;
        }
        lockProjects.unlock();
        return false;
    }

    Project *ProjectControl::getProject(QString projectId) {
        lockProjects.lockForRead();
        if (projects.contains(projectId)) {
            Project *ret = projects[projectId];
            lockProjects.unlock();
            return ret;
        }
        lockProjects.unlock();
        return NULL;
    }

    bool ProjectControl::saveProjects() {
        QWriteLocker locker(&lockProjects);
        QMapIterator<QString,Project*> it(projects);
    #ifdef Q_WS_MAC
        QSettings settings("telemidia.pucrio.br", "composer");
     #else
        QSettings settings("telemidia", "composer");
     #endif
        settings.beginGroup("projects");
        while(it.hasNext()){
            it.next();
            Project *p = it.value();
            settings.setValue(p->getProjectId(), p->getLocation());
            qDebug() << "ProjectControl::saveAllProjects projectId: "
                     << p->getProjectId() <<
                     "location: " << p->getLocation();
            //TODO salvar alterações no NCL
        }
        settings.endGroup();
        settings.sync();
        return true;
    }

    bool ProjectControl::closeProject (QString projectId) {
        //TODO - salvar arquivo de projeto e chamar save para cada NCLDocument
      return true;
    }
    bool ProjectControl::deleteProject (QString projectId) {
        //TODO - deletar todos os NCLDocument e deletar o arquivo de projeto
      return true;
    }


    bool ProjectControl::removeDocument (QString projectId,
                                         QString documentId) {
        //TODO - deletar o NclDocument do projeto mas não o arquivo
      return true;
    }
    bool ProjectControl::deleteDocument (QString projectId, QString documentId) {
        //TODO - deletar o NclDocument do Projeto e deletar o arquivo do sistema
      return true;
    }


    void ProjectControl::addExistingProject(QString projectId,
                                            QString location) {

        qDebug() << "ProjectControl::addExistingProject(" << projectId << ", "
                 << location << ")";

        QDir dir(location + QDir::separator() + projectId);

        if (dir.exists()) {
            createProject(projectId,location);
            emit projectCreated(projectId,location);
            //TODO - fazer o filtro certinho
            //QStringList filters;
            //filters << "*.ncl";
            //dir.setNameFilters(filters);
            dir.setFilter(QDir::Files | QDir::AllDirs
                          | QDir::NoDotAndDotDot | QDir::NoSymLinks);
            dir.setSorting(QDir::DirsFirst | QDir::Name);
            addFilesInDirectory(dir,projectId);
        } else { //dir dont exists
            emit notifyError(tr("Project %1 could not be open,"
                            "maybe the path (%2) has been changed")
                         .arg(projectId).arg(location));
        }
    }

    void ProjectControl::addFilesInDirectory(QDir dir, QString projectId) {
        QFileInfoList list = dir.entryInfoList();

        qDebug() << "ProjectControl::addFilesInDirectory(" <<
                    dir << ", " << projectId;

        for (int i = 0; i < list.size(); ++i) {
           QFileInfo fileInfo = list.at(i);
           qDebug() << "ProjectControl::addFilesInDirectory parsing file: " <<
                        fileInfo.fileName();
           if (fileInfo.isFile()) {
               QString fileName = fileInfo.fileName();
               QString filePath = fileInfo.absoluteFilePath();
               if (fileName.endsWith(".ncl")) {
                   qDebug() << "ProjectControl::addFilesInDirectory " <<
                                "Adding NCL File: " << fileName <<
                                "in: " << projectId;
                   addDocument(fileName,filePath,projectId,false);
               }
           } else if (fileInfo.isDir()) {
               //TODO adicionar diretorio no projectTree
               //TODO testar emitindo sinal de projetoCriado
               addFilesInDirectory(fileInfo.dir(),projectId);
           }

         }
    }

    void ProjectControl::addProject(QString projectId, QString location){

        qDebug() << "ProjectControl::addProject(" << projectId << ", "
                 << location << ")";

        QString fullPath = location + QDir::separator() + projectId;
        QDir dir(fullPath);

        if(dir.exists()) {
            QMessageBox mBox;
            mBox.setText(tr("The directory (%1) already exists").arg(fullPath));
            mBox.setInformativeText(tr("Do you want add as Project ?"));
            mBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            mBox.setDefaultButton(QMessageBox::Yes);
            mBox.setIcon(QMessageBox::Question);
            if ( mBox.exec() == QMessageBox::Yes) {
                return addExistingProject(projectId,location);
            } else {
                emit notifyError(
                     tr("The project was not created due the user choice!"));
                return;
            }
        } else { //Directory does not exists
            if(dir.mkpath(fullPath)) {
                createProject(projectId,location);
                emit projectCreated(projectId,location);
            } else {
                emit notifyError(tr("Directory for this project could not be created"));
            }
        }
    }

    void ProjectControl::addDocument(QString documentId,QString location,
                                  QString projectId, bool copy) {

        //TODO - rever esses nomes de argumentos

        qDebug() << "ProjectControl::addDocument(" << documentId << ", "
                 << location << ")";

        Project *p = getProject(projectId);
        QString projectLocation = p->getLocation();

        QString uri = location;
        if (!documentId.endsWith(".ncl")) documentId += ".ncl";
        QString destUri = projectLocation+QDir::separator()+projectId+
                          QDir::separator()+documentId;

        /* Adding a existing document */
        if (copy) {
            if (!QFile::copy(uri, destUri)) {
               qDebug() << "ProjectControl::addDocument" <<
                           "fails on copy the document";
               emit notifyError(tr("Could not copy NCLFile (%1)"
                                   "to location (%2)").arg(documentId,destUri));
               return;
           }
        } else { /* Creating a brand new Document */
            QFile nclFile(destUri);
            if (!nclFile.open( QIODevice::ReadWrite ) ) {
                emit notifyError(tr("Could not created the NCLFile (%1)")
                                 .arg(destUri));
                return;
            } else {
                nclFile.close();
            }
        }

        uri = destUri;

        //TODO - rever se eh projectLocation mesmo
        PluginControl::getInstance()->onNewDocument(documentId,uri);

    }

}//fim
}//fim
}//fim namespace
