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

    Entity::Entity(QString uniqueId, QMap<QString,QString> &atts,
                   QObject *parent) :
        QObject(parent)
    {
        QMutexLocker locker(&lockAtts);
        this->atts = atts;
        setUniqueID(uniqueId);
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
            children.remove(it.key());
        }
    }

    //! This call deletes the child and its children in a recursive way
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

    //! Print the Entity and its children
    void Entity::print()
    {
            QMap<QString, Entity*>::iterator it;
            for (it = children.begin() ; it != children.end(); it++)
            {
                    Entity *child = it.value();
                    child->print();
            }
    }

    QString Entity::toString(int ntab)
    {
        QString out = "";
        for(int i = 0; i < ntab; i++)
            out += "\t";

        out += "<";
        out.append(getType().toAscii());
        foreach(QString attr, atts.keys()){
            out += " ";
            out.append(attr);
            out += "=\"";
            out += atts.value(attr);
            out += "\"";
        }
        out += " uniqueEntityId=\"";
        out += getUniqueId();
        out += "\">\n";

        QMap<QString, Entity*>::iterator it;
        for (it = children.begin() ; it != children.end(); it++)
        {
                Entity *child = it.value();
                out += child->toString(ntab+1);
        }
        out += "\n";
        for(int i = 0; i < ntab; i++)
            out += "\t";
        out += "</";
        out += getType();
        out += ">\n";
        return out;
    }

}}} //end namespace
