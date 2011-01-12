#include "../../include/modules/ProjectControl.h"

namespace composer {
namespace core {
namespace module {

    ProjectControl::ProjectControl() {
        this->activeProject = NULL;
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

        QMapIterator<QString,DocumentParser*> itP(parsers);
        while(itP.hasNext()){
            itP.next();
            DocumentParser *doc = itP.value();
            parsers.remove(itP.key());
            delete doc;
            doc = NULL;
        }
        parsers.clear();
    }

    bool ProjectControl::openProject (QString projectId, QString location) {
        //TODO - fazer parser do arquivo de projeto
        return true;
    }

    Project* ProjectControl::createProject(QString projectId, QString location) {

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

    bool ProjectControl::saveProject (QString projectId, QString location) {
        //TODO - salvar arquivo de projeto e chamar save para cada NCLDocument
        return true;
    }

    bool ProjectControl::saveAllProjects() {
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

    bool ProjectControl::addDocument (QString projectId, QString uri,
                                      QString  documentId, bool copy) {
        Project *p = getProject(projectId);
        QString projectLocation = p->getLocation();

        if (!documentId.endsWith(".ncl")) documentId += ".ncl";
        QString destUri = projectLocation+QDir::separator()+projectId+
                          QDir::separator()+documentId;

        /* Adding a existing document */
        if (copy) {
           if (!QFile::copy(uri,destUri)) {
               qDebug() << "ProjectControl::addDocument" <<
                           "fails on copy the document";
               return false;
           }           
        } /*else { /* Creating a brand new Document /
            QFile nclFile(destUri);
            if (!nclFile.open( QIODevice::ReadWrite ) ) {
                emit errorNotify(tr("Could not created the NCLFile (%1)")
                                 .arg(destUri));
                return false;
            } else {
                nclFile.close();
            }
        }*/

        uri = destUri;
        DocumentParser *docParser = new DocumentParser(documentId,projectId);
        parsers[projectId+documentId] = docParser;

        connect(docParser,SIGNAL(documentParsed(QString,QString)),this,
             SLOT(onDocumentParsed(QString,QString)));

        if (!docParser->setUpParser(uri)) {
           qDebug() << "ProjectControl::addDocument fails in setUpParser";
           return false;
        }
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

    bool ProjectControl::saveDocument (QString projectId, QString documentId,
                                       QString location) {
        //TODO - salvar o documento Ncl
      return true;
    }

    void ProjectControl::onDocumentParsed(QString projectId,
                                          QString documentId)
    {
        Project *p = getProject(projectId);
        if (p == NULL) {
            //Delete parser
            return;
        }

        if (parsers.count(projectId+documentId) != 0) {
            DocumentParser *parser = parsers[projectId+documentId];
            NclDocument *nclDoc = parser->getNclDocument();
            if (nclDoc != NULL) {
                if (p->addDocument(documentId,nclDoc)) {
                    qDebug() << "ProjectControl::onDocumentParsed("
                             << projectId << ", " << documentId << ")";
                    emit documentCreatedAndParsed(documentId,
                                                  p->getLocation());
                }
            }
        }
    }

}//fim
}//fim
}//fim namespace
