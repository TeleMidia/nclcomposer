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
    if (deleteChildren) {
        while(children.size())
        {
            Entity *child = children.at(0);
            delete child;
            child = NULL;
            children.pop_front();
        }
    }
}

void Entity::setAttribute(QString name, QString value)
{
    QMutexLocker locker(&lockAtts);
    atts[name] = value;
}

void Entity::setAtrributes(QMap<QString,QString> &newatts)
{
    QMutexLocker locker(&lockAtts);
    // this->atts.clear(); // Should it??!
    for ( QMap<QString,QString>::iterator it = newatts.begin();
            it != newatts.end(); ++it)
    {
        this->atts[it.key()] = it.value();
    }
}

void Entity::setType(QString type)
{
    QMutexLocker locker(&lockType);
    this->type = type;
}

void Entity::setUniqueID(QString uniqueId)
{
    QMutexLocker locker(&lockID);
    this->_id = uniqueId;
}

void Entity::setParent(Entity *parent)
{
    this->parent = parent;
}

bool Entity::addChild(Entity *entity)
{
    QMutexLocker locker(&lockChildren);
    QString _id = entity->getUniqueId();
    qDebug() << children.size();
    // Check if the entity is already children of this entity
    // TODO: THIS CAN BE IMPROVED!! Maybe checking if the parentUniqueID.
    for(int i = 0; i < children.size(); i++)
    {
        if(children.at(i)->getUniqueId() == _id )
            return false;
    }
    children.push_back(entity);
    entity->setParent(this);
    return true;
}

//! Deletes the child and its children in a recursive way
bool Entity::deleteChild(Entity *entity)
{
    QMutexLocker locker(&lockChildren);
    entity->setDeleteChildren(true);
    qDebug() << "deleteChild" << children.size();
    for(int i = 0; i < children.size(); i++)
    {
        if(children.at(i) == entity)
        {
            children.remove(i);
        }
    }
    qDebug() << "deleteChild" << children.size();
    delete entity;
    entity = NULL;

    return true;
}

QString Entity::getAttribute(QString name)
{
    QMutexLocker locker(&lockAtts);
    return atts.contains(name) ? atts[name] : "";
}

void Entity::getAttributeIterator (QMap<QString,QString>::iterator &begin,
                                   QMap<QString,QString>::iterator &end)
{
    QMutexLocker locker(&lockAtts);
    begin = this->atts.begin();
    end = this->atts.end();
}

bool Entity::hasAttribute(const QString &name)
{
    QMutexLocker locker(&lockAtts);
    return this->atts.contains(name);
}

QString Entity::getType()
{
    QMutexLocker locker(&lockType);
    return this->type;
}

Entity* Entity::getParent()
{
    QMutexLocker locker(&lockParent);
    return parent;
}

QString Entity::getParentUniqueId()
{
    QMutexLocker loecker(&lockParent);
    return parent->getUniqueId();
}

void Entity::setDeleteChildren(bool _delete)
{
    this->deleteChildren = _delete;
}

QVector <Entity *> Entity::getChildren()
{
    return this->children;
}

//! Removes the child and append his children to his parent
// Is it useful ??
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

//! Prints the Entity and its children
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
    /*foreach(QString attr, atts.keys()){
        out += " ";
        out.append(attr);
        out += "=\"";
        out += atts.value(attr);
        out += "\"";
    }*/
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
