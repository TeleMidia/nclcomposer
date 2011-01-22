#ifndef PROJECTCONTROL_H
#define PROJECTCONTROL_H

#include <QtDebug>
#include <QReadWriteLock>
#include <QSettings>
#include <QMessageBox>
#include <QObject>
#include <QMap>
#include <QString>

#include "../util/Project.h"
#include "../util/DocumentParser.h"
#include "../util/Singleton.h"
using namespace composer::core::util;


#include "PluginControl.h"
using namespace composer::core::module;


namespace composer {
namespace core {
namespace module {

    class ProjectControl : public QObject , public Singleton<ProjectControl> {
        Q_OBJECT

        friend class Singleton<ProjectControl>;
        private:
            ProjectControl();
            ~ProjectControl();

            QReadWriteLock lockProjects;
            Project *activeProject;
            QMap<QString, Project*> projects;
            QMap<QString, DocumentParser* > parsers;

            void addFilesInDirectory(QDir dir, QString projectId);
            void addExistingProject(QString projectId,QString location);

        public:
            /** Abre uma base privada existente localizada pelo parâmetro
                location. Se a base privada não existir ou se o parâmetro
                location não for informado, uma nova base é criada com o
                identificador baseId. */

             bool activateProject (QString projectId);
             bool deactivateProject (QString projectId);
             bool deleteProject (QString projectId);

             /* the physical dir already exists, creating the Project* */
             bool createProject(QString projectId, QString location);

             /** Fecha a base privada e descarta todo o conteúdo da base
                privada */
             bool closeProject (QString projectId);
             bool saveProjects();

             bool removeDocument (QString projectId, QString documentId);
             bool deleteDocument (QString projectId, QString documentId);
             bool saveDocument (QString projectId, QString documentId,
                                QString location);
             Project *getProject(QString projectId);
             inline Project *getCurrentProject() {return this->activeProject;}

         public slots:
             void addProject(QString projectId, QString location);

             void addDocument(QString documentId,QString location,
                              QString projectId, bool copy);

         signals:
             void projectCreated(QString projectId,QString location);
             void newActivateProject(Project *);
             void documentAdded(QString documentId, QString location);
             void launchNewPluginInstance(Document *);
             void notifyError(QString);

    };
}
}
}
#endif // PROJECTCONTROL_H

