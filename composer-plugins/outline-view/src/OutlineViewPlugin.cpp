#include "OutlineViewPlugin.h"

#include "core/modules/LanguageControl.h"

OutlineViewPlugin::OutlineViewPlugin()
{
    window = new NCLTreeWidget(0);
    doc = NULL;

    connect (window, SIGNAL(elementAddedByUser(QString, QString, QMap<QString,QString>&,bool)),
             this, SIGNAL(addEntity(QString, QString, QMap<QString,QString>&, bool)));

    connect (window, SIGNAL(elementRemovedByUser(QString)), this,
             SLOT(elementRemovedByUser(QString)));
}

OutlineViewPlugin::~OutlineViewPlugin()
{
    delete window;
}

QWidget* OutlineViewPlugin::getWidget()
{
//    ILanguageProfile *profile = LanguageControl::getInstance()->getProfileFromType(0);
    return window;
}

void OutlineViewPlugin::onEntityAdded(QString ID, Entity *entity)
{
    QString line = "<" + entity->getType() + "> </" + entity->getType() + ">\n";

    QTreeWidgetItem *item;
    if(idToItem.contains(entity->getParentUniqueId())) {
        item = window->addElement(idToItem[entity->getParentUniqueId()], 0, entity->getType(), entity->getUniqueId(), 0, 0);
    }
    else {
        item = window->addElement(0, 0, entity->getType(), entity->getUniqueId(), 0, 0);
    }
    idToItem[entity->getUniqueId()] = item;
}

void OutlineViewPlugin::onEntityAddError(QString error)
{
    qDebug() << "OutlineViewPlugin::onEntityAddError(" << error << ")";
}

void OutlineViewPlugin::onEntityChanged(QString ID, Entity * entity)
{
    QString line = "PLUGIN (" + ID + ") changed the Entity (" +
                   entity->getType() + " - " + entity->getUniqueId() +")";
    //TODO: All
}

void OutlineViewPlugin::onEntityChangeError(QString error)
{
    //TODO: All
}

void OutlineViewPlugin::onEntityAboutToRemove(Entity *)
{

}

void OutlineViewPlugin::onEntityRemoved(QString ID, QString entityID)
{
    QString line = "PLUGIN (" + ID + ") removed Entity (" +
                   entityID + ")";
    //TODO: All
}

void OutlineViewPlugin::onEntityRemoveError(QString error)
{
    //TODO: All
}

void OutlineViewPlugin::elementRemovedByUser(QString id)
{
    qDebug() << "elementRemovedByUser" << id;
    Entity *entity = doc->getEntityBydId(id);
    emit removeEntity(entity, false);
    idToItem.remove(id);
}

bool OutlineViewPlugin::save(){
    //TODO: All
}

void OutlineViewPlugin::updateFromModel() {
    //TODO: All
}

