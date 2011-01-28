#include "model/Document.h"


namespace composer {
    namespace core {
        namespace model {

    Document::Document(QObject *parent) :
        Entity(parent)
    {
        setType("document");
        entities[this->getUniqueId()] = this;
    }

    Document::Document(QMap<QString,QString> &atts, QObject *parent) :
        Entity(atts, parent)
    {
        setType("document");
        entities[this->getUniqueId()] = this;
    }

    Document::~Document() {
        QMutexLocker locker(&lockEntities);
        entities.clear();
    }

    LanguageType Document::getDocumentType()
    {
        return this->documentType;
    }

    void Document::setDocumentType(LanguageType type)
    {
        this->documentType = type;
    }

    Entity* Document::getEntityBydId(QString _id) {
        QMutexLocker locker(&lockEntities);
        return entities.contains(_id) ? entities[_id] : NULL;
    }

    QList<Entity*> Document::getEntitiesbyType(QString _type) {
        QMutexLocker locker(&lockEntities);
        QMapIterator<QString, Entity*> it(entities);
        QList<Entity*> listRet;
        while(it.hasNext()){
            it.next();
            Entity* ent = it.value();
            if (ent->getType() == _type)
                listRet.append(ent);
        }
        return listRet;
    }

    QString Document::getLocation() {
        QMutexLocker locker(&lockLocation);
        return this->documentLocation;
    }

    void Document::setLocation(QString location) {
        QMutexLocker locker(&lockLocation);
        this->documentLocation = location;
    }

    void Document::setProjectId(QString _projectId)
    {
        this->projectId = _projectId;
    }

    QString Document::getProjectId()
    {
        return this->projectId;
    }

    bool Document::addEntity(Entity* entity, QString parentId)
        throw (EntityNotFound,ParentNotFound)
    {
        QMutexLocker locker(&lockEntities);
        if (!entities.contains(parentId)) {
            throw ParentNotFound(entity->getType(),entity->getType(),
                                     parentId);
            return false;
        }
        if (entities.contains(entity->getUniqueId())) {
            throw EntityNotFound(entity->getType(),entity->getUniqueId());
            return false;
        }

        Entity *parent = entities[parentId];
        parent->addChild(entity);
        entities[entity->getUniqueId()] = entity;
    }

    bool Document::removeEntity(Entity* entity, bool appendChild)
         throw (EntityNotFound)
    {
        QMutexLocker locker(&lockEntities);
        QString _id = entity->getUniqueId();

        if (entities.contains(entity->getUniqueId())) {
            Entity *parent = entity->getParent();
            if (parent) {
                if (appendChild)
                        parent->removeChildAppendChildren(entity);
                else parent->deleteChild(entity);
            } else { //does not have a parent dont append
                delete entity;
                entity = NULL;
                return true;
            }
        } else {
            throw EntityNotFound(entity->getType(),entity->getUniqueId());
            return false; // entity does not exist in the model
        }

        entities.remove(_id);
        return true;

    }

        }
    }
}
