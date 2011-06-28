#include "OutlineViewPlugin.h"

#include "core/modules/LanguageControl.h"

namespace composer {
    namespace plugin {
        namespace outline {

OutlineViewPlugin::OutlineViewPlugin()
{
    window = new NCLTreeWidget(0);
    project = NULL;

    connect ( window,
              SIGNAL( elementAddedByUser ( QString,
                                            QString,
                                            QMap <QString, QString> &,
                                            bool)),
              this,
              SIGNAL( addEntity( QString,
                                 QString,
                                 QMap <QString, QString> &,
                                 bool)));

    connect (window, SIGNAL(elementRemovedByUser(QString)), this,
             SLOT(elementRemovedByUser(QString)));

    connect(window,
            SIGNAL(itemSelectionChanged()),
            this,
            SLOT(itemSelectionChanged()));

    selectedId = NULL;
}

OutlineViewPlugin::~OutlineViewPlugin()
{
    delete selectedId;
    delete window;
}

QWidget* OutlineViewPlugin::getWidget()
{
    return window;
}

void OutlineViewPlugin::onEntityAdded(QString pluginID, Entity *entity)
{
    QString line = "<" + entity->getType() + "> </" + entity->getType() + ">\n";

    QTreeWidgetItem *item;
    QMap <QString, QString> attrs;
    QMap <QString, QString>::iterator begin, end, it;

    entity->getAttributeIterator(begin, end);
    for (it = begin; it != end; ++it)
    {
        attrs[it.key()] = it.value();
    }

    if(idToItem.contains(entity->getParentUniqueId())) {
        item = window->addElement( idToItem[entity->getParentUniqueId()],
                                   -1,
                                   entity->getType(),
                                   entity->getUniqueId(),
                                   attrs,
                                   0, 0);
    }
    else {
        item = window->addElement( 0,
                                   -1,
                                   entity->getType(),
                                   entity->getUniqueId(),
                                   attrs,
                                   0, 0);
    }
    idToItem[entity->getUniqueId()] = item;
}

void OutlineViewPlugin::errorMessage(QString error)
{
    qDebug() << "OutlineViewPlugin::onEntityAddError(" << error << ")";
}

void OutlineViewPlugin::onEntityChanged(QString pluginID, Entity * entity)
{
    QString line = "PLUGIN (" + pluginID + ") changed the Entity (" +
                   entity->getType() + " - " + entity->getUniqueId() +")";

    QMap <QString, QString> attrs;
    QMap <QString, QString>::iterator begin, end, it;

    entity->getAttributeIterator(begin, end);
    for (it = begin; it != end; ++it)
    {
        attrs[it.key()] = it.value();
    }

    window->updateItem(idToItem[entity->getUniqueId()], entity->getType(),
                       attrs);

}

/*void OutlineViewPlugin::onEntityAboutToRemove(Entity *entity)
{
//    entityToRemoveId = ""
}*/

void OutlineViewPlugin::onEntityRemoved(QString pluginID, QString entityID)
{
    if(idToItem.contains(entityID))
    {
        idToItem.remove(entityID);
        window->removeItem(entityID);
    }
}

void OutlineViewPlugin::elementRemovedByUser(QString itemId)
{
    qDebug() << "elementRemovedByUser (id ='" << itemId << "')";
    Entity *entity = project->getEntityBydId(itemId);
    emit removeEntity(entity, false);
}

bool OutlineViewPlugin::saveSubsession()
{
    //TODO: All
    return true;
}

void OutlineViewPlugin::updateFromModel()
{
    //TODO: All
}

void OutlineViewPlugin::debugHasSendClearAll(void *param)
{
    qDebug() << "OutlineViewPlugin::debugHasSendClearAll";
}

void OutlineViewPlugin::itemSelectionChanged()
{
    if(selectedId != NULL)
        delete selectedId;


    QList <QTreeWidgetItem*> selecteds = window->selectedItems();

    if(selecteds.size())
    {
        selectedId = new QString(selecteds.at(0)->text(2));
        emit sendBroadcastMessage("changeSelectedEntity", selectedId);
    }
}

}}} //end namespace
