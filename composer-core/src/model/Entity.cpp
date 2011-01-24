#include "model/Entity.h"


namespace composer {
    namespace core {
        namespace model {

    Entity::Entity(QObject *parent) :
        QObject(parent)
    {
        setUniqueID(QUuid::createUuid().toString());
        QMutexLocker locker(&lockParent);
        this->parent = (Entity*)parent;
        this->deleteChildren = true;
    }

    Entity::Entity(QMap<QString,QString> &atts, QObject *parent) :
        QObject(parent)
    {
        QMutexLocker locker(&lockAtts);
        this->atts = atts;
        setUniqueID(QUuid::createUuid().toString());
        QMutexLocker lo(&lockParent);
        this->parent = (Entity*)parent;
        this->deleteChildren = true;
    }

    Entity::~Entity() {
        QMutexLocker locker(&lockChildren);
        QMapIterator<QString,Entity*> it(children);

        while(it.hasNext()){
            it.next();
            Entity *child = it.value();
            if (deleteChildren) {
                delete child;
                child = NULL;
            }
            if (children.remove(it.key()) != 1)
                qDebug() << "Entity::~Entity() Não conseguiu remover do mapa";
        }
    }


    //! This call deletes the child and his children in a recursive way
    bool Entity::deleteChild(Entity *entity) {
        QMutexLocker locker(&lockChildren);
        entity->setDeleteChildren(true);
        delete entity;
        entity = NULL;
    }

    //! This call removes the child and append his children to his parent
    bool Entity::removeChildAppendChildren(Entity *entity){
        QMutexLocker locker(&lockChildren);
        QMap<QString, Entity*>::iterator it;
        for(it = entity->children.begin();
            it != entity->children.end(); it++) {
            Entity *child = it.value();
            child->setParent(this);
        }
        entity->setDeleteChildren(false);
        delete entity;
        entity = NULL;
    }

        }//end namespace declaration
    }
}
