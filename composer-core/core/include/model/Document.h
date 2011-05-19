#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QList>

namespace composer {
    namespace core {
        namespace util {
            class Project;
        }
        namespace module {
            class PluginControl;
            class MessageControl;
            class DocumentControl;
        }
    }
}

#include "../model/exception/ParentNotFound.h"
#include "../model/exception/EntityNotFound.h"
#include "../util/Utilities.h"
using namespace composer::core::util;

#include "Entity.h"
using namespace composer::core::model;

namespace composer {
    namespace core {
        namespace model {

            class Document : public Entity
            {
                Q_OBJECT

                // The following classes are "reliable" and can acess the
                // private and protected members of Document.
                friend class composer::core::module::PluginControl;
                friend class composer::core::module::MessageControl;
                friend class composer::core::module::DocumentControl;
                friend class composer::core::util::Project;

            public:
                Entity* getEntityBydId(QString _id);
                QList<Entity*> getEntitiesbyType(QString _type);
                QString getLocation();
                QString getProjectId();
                LanguageType getDocumentType();

            private:
                QMutex lockEntities;
                QMutex lockLocation;
                QMap<QString, Entity*> entities;
                QString documentLocation;
                QString projectId;
                LanguageType documentType;

            protected:
                explicit Document(QObject *parent = 0);
                Document(QMap<QString,QString> &atts, QObject *parent = 0);
                ~Document();

                void setLocation(QString location);
                void setDocumentType(LanguageType type);
                void setProjectId(QString _projectId);

                //! This method is used to add an Entity in the map and as child
                //! of the parentId.
                /*!
                \param entity - A Entity to be added
                \param parentId - A QString identifying the parent to be added
                               as a child.
                \return an boolean depending on the success
                */
                bool addEntity(Entity* entity, QString parentId)
                        throw (EntityNotFound, ParentNotFound);

                //! This method is used to remove and delete an Entity from the
                //!   map
                /*!
                    \param entity - A Entity to be removed
                    \param appendChild - If true the children of Entity will be
                                appended in the parentEntity. If false all the
                                children are also deleted.
                    \return an boolean depending on the success
                */
                bool removeEntity(Entity* entity, bool appendChild)
                        throw (EntityNotFound);

            signals:

            public slots:

            };

        }
    }
}

#endif // DOCUMENT_H
