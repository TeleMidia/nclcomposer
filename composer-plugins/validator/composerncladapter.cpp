#include "composerncladapter.h"

ComposerNCLAdapter::ComposerNCLAdapter()
{
}

void ComposerNCLAdapter::addElement(QString ID, Entity *entity)
{
    qDebug() << "**********Validator: Begin addElement";
    if (!entity){
        qDebug() << "entity = NULL";
        return;
    }

    qDebug () << entity->getType() << " " << entity->getUniqueId();

    QMap <QString, QString>::iterator begin;
    QMap <QString, QString>::iterator end;

    entity->getAttributeIterator(begin, end);

    std::vector <nclValidator::Attribute> attributes;

    while (begin != end){
        QString name = begin.key();
        QString value = begin.value();

//        qDebug() << name << " " << value;

        nclValidator::Attribute attr (name.toStdString(), value.toStdString());

//        std::cout << attr.name() << " " << attr.value();

        attributes.push_back(attr);
        begin++;
    }

    if (!idToVirtualId.count(entity->getParentUniqueId())){
        Entity *parent = entity->getParent();

        if (parent->getType() != "document")
            addElement(parent->getUniqueId(), parent);
    }

    nclValidator::virtualId virtualId = nclModel.addElement(entity->getType().toStdString(), attributes);

    if (virtualId != "")
        idToVirtualId.insert(entity->getUniqueId(), virtualId);

    if (entity->getParent()->getType() != "document"){
        nclModel.addChild(idToVirtualId[entity->getParentUniqueId()], virtualId);
    }


    qDebug() << "**********Validator: End addElement";
}

void ComposerNCLAdapter::changeElement(QString, Entity *)
{

}

void ComposerNCLAdapter::removeElement(QString, QString entityID)
{
    nclValidator::virtualId virtualId = idToVirtualId [entityID];

    if (virtualId != "")
        nclModel.removeElement(virtualId);
}

std::vector <std::string> ComposerNCLAdapter::validate()
{
    qDebug() << "**********Validator: Begin validate";

    std::vector<std::string> msgs = nclValidator::Validator::validate(nclModel);

    qDebug() << "**********Validator: End validate";

    return msgs;
}
