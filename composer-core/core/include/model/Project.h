#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QList>
#include <QStack>

namespace composer {
    namespace core {
        class PluginControl;
        class MessageControl;
        class ProjectControl;
        class ProjectReader;
} } //end namespace

#include "../model/exception/ParentNotFound.h"
#include "../model/exception/EntityNotFound.h"
#include "../util/Utilities.h"
using namespace composer::core::util;

#include "Entity.h"
using namespace composer::core::model;

namespace composer {
    namespace core {
        namespace model {

class Project : public Entity
{
    Q_OBJECT

    // The following classes are "reliable" and can acess the
    // private and protected members of Document.
    friend class composer::core::PluginControl;
    friend class composer::core::MessageControl;
    friend class composer::core::ProjectControl;
    friend class composer::core::ProjectReader;

public:
    Entity* getEntityById(QString _id);
    QList<Entity*> getEntitiesbyType(QString _type);

    QString getLocation();
    QString getProjectId();

    LanguageType getProjectType();
    /*!
      \brief Get specific plugin data saved in the document.
     */
    QByteArray getPluginData(QString pluginId);

private:
    QMutex lockEntities;
    QMutex lockLocation;
    QMap<QString, Entity*> entities;
    QMap<QString, QByteArray> pluginData;

    QString projectLocation;
    QString projectName;
    LanguageType projectType;

protected:
    explicit Project(QObject *parent = 0);
    Project(QMap<QString,QString> &atts, QObject *parent = 0);
    Project(QString uniqueId, QMap<QString,QString> &atts, QObject *parent = 0);
    ~Project();

    void setLocation(QString location);
    void setProjectType(LanguageType type);

    QString model;
    void setModelString(const QString &model) {this->model = model;}
    QString getModelString() {return this->model;}

    /*! \brief This method is used to add an Entity in the map and as child of
            the parentId.

    \param entity - A Entity to be added
    \param parentId - A QString identifying the parent to be added
                   as a child.
    \return true if success and false otherwise.
    */
    bool addEntity(Entity* entity, QString parentId)
            throw (EntityNotFound, ParentNotFound);

    /*!
      \brief Set specific plugin data to project file.
      \param pluginId
    */
    bool setPluginData(QString pluginId, const QByteArray data);

    /*! \brief This method is used to remove and delete an Entity from the map.

        \param entity - A Entity to be removed
        \param appendChild - If true the children of Entity will be
                    appended in the parentEntity. If false all the
                    children are also deleted.
        \return an boolean depending on the success
    */
    bool removeEntity(Entity* entity, bool appendChild)
            throw (EntityNotFound);

    QString toString();

    bool serialize();

signals:
    void dirty();

public slots:

};

} } } //end namespace

#endif // DOCUMENT_H
