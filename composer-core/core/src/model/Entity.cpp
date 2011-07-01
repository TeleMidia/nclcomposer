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
        while(children.size())
        {
            Entity *child = children.at(0);
            if (deleteChildren) {
                delete child;
                child = NULL;
            }
            children.pop_front();
        }
    }

    //! This call deletes the child and its children in a recursive way
    bool Entity::deleteChild(Entity *entity)
    {
        QMutexLocker locker(&lockChildren);
        entity->setDeleteChildren(true);
        delete entity;
        entity = NULL;

        return true;
    }

    //! This call removes the child and append his children to his parent
    bool Entity::removeChildAppendChildren(Entity *entity)
    {
        for(int i= 0; i < children.size(); i++)
        {
            Entity *child = children.at(i);
            child->setParent(this);
        }

        entity->setDeleteChildren(false);
        delete entity;
        entity = NULL;

        return true;
    }

    //! Print the Entity and its children
    void Entity::print()
    {
        for (int i = 0; i < children.size(); i++)
        {
            Entity *child = children.at(i);
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

        for (int i = 0; i < children.size(); i++)
        {
            Entity *child = children.at(i);
            out += child->toString(ntab+1);
        }
        for(int i = 0; i < ntab; i++)
            out += "\t";
        out += "</";
        out += getType();
        out += ">\n";
        return out;
    }

}}} //end namespace
