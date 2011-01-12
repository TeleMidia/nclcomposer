#ifndef PROJECTCONTROL_H
#define PROJECTCONTROL_H

#include <QtDebug>
#include <QReadWriteLock>
#include <QSettings>

#include <QObject>

#include <QMap>
#include <QString>

/*
namespace composer {
namespace core {
namespace module {
class ProjectControl;
} } }
*/

#include "../util/Project.h"
#include "../util/Singleton.h"
using namespace composer::core::util;
#include "../util/DocumentParser.h"
using namespace composer::core::plugin;
#include "PluginControl.h"
using namespace composer::core::util;


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

        public:
            /** Abre uma base privada existente localizada pelo parâmetro
                location. Se a base privada não existir ou se o parâmetro
                location não for informado, uma nova base é criada com o
                identificador baseId. */
             bool openProject (QString projectId, QString location);
             bool activateProject (QString projectId);
             bool deactivateProject (QString projectId);
             bool deleteProject (QString projectId);
             Project* createProject(QString projectId, QString location);
             /** Salva todo o conteúdo da base privada em um dispositivo de
                armazenamento persistente */
             bool saveProject (QString projectId, QString location);
             /** Fecha a base privada e descarta todo o conteúdo da base
                privada */
             bool closeProject (QString projectId);
             bool saveAllProjects();
             bool addDocument (QString projectId, QString uri,
                               QString  documentId, bool copy);

             bool removeDocument (QString projectId, QString documentId);
             bool deleteDocument (QString projectId, QString documentId);
             bool saveDocument (QString projectId, QString documentId,
                                QString location);
             Project *getProject(QString projectId);
             inline Project *getCurrentProject() {return this->activeProject;}

         public slots:
             void onDocumentParsed(QString projectId, QString documentId);
         signals:
             void newActivateProject(Project *);
             void documentCreatedAndParsed(QString documentId,
                                           QString location);
             void launchNewPluginInstance(NclDocument *);
             void errorNotify(QString);

    };
}
}
}
#endif // PROJECTCONTROL_H

