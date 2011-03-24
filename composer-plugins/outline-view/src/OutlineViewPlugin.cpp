#include "OutlineViewPlugin.h"

#include "core/modules/LanguageControl.h"

OutlineViewPlugin::OutlineViewPlugin()
{
    window = new NCLTreeWidget(0);
    doc = NULL;

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
}

OutlineViewPlugin::~OutlineViewPlugin()
{
    delete window;
}

QWidget* OutlineViewPlugin::getWidget()
{
    return window;
}

void OutlineViewPlugin::onEntityAdded(QString ID, Entity *entity)
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
                                   0,
                                   entity->getType(),
                                   entity->getUniqueId(),
                                   attrs,
                                   0, 0);
    }
    else {
        item = window->addElement( 0,
                                   0,
                                   entity->getType(),
                                   entity->getUniqueId(),
                                   attrs,
                                   0, 0);
    }
    idToItem[entity->getUniqueId()] = item;
}

void OutlineViewPlugin::onEntityAddError(QString error)
{
    qDebug() << "OutlineViewPlugin::onEntityAddError(" << error << ")";
}

void OutlineViewPlugin::onEntityChanged(QString pluginID, Entity * entity)
{
    QString line = "PLUGIN (" + pluginID + ") changed the Entity (" +
                   entity->getType() + " - " + entity->getUniqueId() +")";
    //TODO: All
}

void OutlineViewPlugin::onEntityChangeError(QString error)
{
    //TODO: All
}

void OutlineViewPlugin::onEntityAboutToRemove(Entity *entity)
{
//    entityToRemoveId = ""
}

void OutlineViewPlugin::onEntityRemoved(QString pluginID, QString entityID)
{
    QString line = "PLUGIN (" + pluginID + ") removed Entity (" +
                   entityID + ")";

    qDebug() << line;
    idToItem.remove(entityID);
    window->removeItem(entityID);

}

void OutlineViewPlugin::onEntityRemoveError(QString error)
{
    //TODO: All
}

void OutlineViewPlugin::elementRemovedByUser(QString itemId)
{
    qDebug() << "elementRemovedByUser (id ='" << itemId << "')";
    Entity *entity = doc->getEntityBydId(itemId);
    emit removeEntity(entity, false);
}

bool OutlineViewPlugin::save()
{
    //TODO: All
}

void OutlineViewPlugin::updateFromModel()
{
    //TODO: All
}

