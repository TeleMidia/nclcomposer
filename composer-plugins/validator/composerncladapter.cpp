#include "composerncladapter.h"

ComposerNCLAdapter::ComposerNCLAdapter()
{
}

void ComposerNCLAdapter::addElement(Entity *entity)
{

    if (!entity){
        return;
    }


    std::vector <nclValidator::Attribute> attributes = createVectorAttribute(entity);

    if (!idToVirtualId.count(entity->getParentUniqueId())){
        Entity *parent = entity->getParent();

        if (parent->getType() != "document")
            addElement(parent);
    }

    nclValidator::virtualId virtualId = nclModel.addElement(entity->getType().toStdString(), attributes);

    if (virtualId != "")
        idToVirtualId.insert(entity->getUniqueId(), virtualId);

    if (entity->getParent()->getType() != "document"){
        nclModel.addChild(idToVirtualId[entity->getParentUniqueId()], virtualId);
    }

    nclValidator::ModelElement *el = nclModel.element(virtualId);
    if (el){
        el->setData(entity);
//        el->setComposerModelId(entity->getUniqueId());
    }

    ////qDebug() << "**********Validator: End addElement";
}

void ComposerNCLAdapter::changeElement(Entity * entity)
{
    //qDebug() << "****************************Start editElement()";

    nclValidator::virtualId virtualId = idToVirtualId[entity->getUniqueId()];

    if (virtualId == "")
        return;

    std::vector <nclValidator::Attribute> attributes = createVectorAttribute(entity);

    nclModel.editElement(virtualId, attributes);
    //qDebug() << "****************************End editElement()";
}

void ComposerNCLAdapter::removeElement(QString entityID)
{
    //qDebug() << "****************************Start removeElement()";

    nclValidator::virtualId virtualId = idToVirtualId [entityID];

    if (virtualId != ""){
        //qDebug () << "remove!";
        nclModel.removeElement(virtualId);
    }

    //qDebug() << "****************************End removeElement()";
}

std::vector <std::pair<void *, std::string> > ComposerNCLAdapter::validate()
{
    //qDebug() << "**********Validator: Begin validate";

    std::vector<std::pair<void *, std::string> > msgs = nclValidator::Validator::validate(nclModel);

    ////qDebug() << "**********Validator: End validate";

    nclModel.clearMarkedElements();

    return msgs;
}

std::vector <nclValidator::Attribute> ComposerNCLAdapter::createVectorAttribute(Entity * entity)
{
    QMap <QString, QString>::iterator begin;
    QMap <QString, QString>::iterator end;

    entity->getAttributeIterator(begin, end);

    std::vector <nclValidator::Attribute> attributes;

    while (begin != end){
        QString name = begin.key();
        QString value = begin.value();

        ////qDebug() << name << " " << value;

        nclValidator::Attribute attr (name.toStdString(), value.toStdString());

        std::cout << attr.name() << " " << attr.value();

        attributes.push_back(attr);
        begin++;
    }

    return attributes;
}
