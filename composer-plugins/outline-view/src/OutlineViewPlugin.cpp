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
              SLOT( elementAddedByUser( QString,
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

void OutlineViewPlugin::onEntityRemoved(QString pluginID, QString entityID)
{
//    qDebug() << "OutlineViewPlugin::onEntityRemoved ("<< pluginID << " "
//            << entityID << ")";
//    qDebug() << idToItem.contains(entityID);

    if(idToItem.contains(entityID))
    {
        idToItem.remove(entityID);
        window->removeItem(entityID);
    }
}

void OutlineViewPlugin::elementRemovedByUser(QString itemId)
{
    Entity *entity = project->getEntityById(itemId);
    emit removeEntity(entity, false);
}

void OutlineViewPlugin::elementAddedByUser( QString type,
                                             QString parentId,
                                             QMap <QString, QString> & atts,
                                             bool force)
{
    /* If there is no parent, put as child of root */
    if(parentId == "")
        parentId = project->getUniqueId();

    emit addEntity(type, parentId, atts, force);
}

bool OutlineViewPlugin::saveSubsession()
{
    //TODO: All
    return true;
}

void OutlineViewPlugin::init()
{
    //Clear previous tree
    QString key;
    foreach(key, idToItem.keys())
    {
        window->removeItem(key);
    }
    idToItem.clear();

    //Draw new tree
    if(!project->getChildren().size()) return;

    QTreeWidgetItem *item;
    QStack <Entity*> stack;
    Entity *entity = project->getChildren().at(0);

    QMap <QString, QString> attrs;
    QMap <QString, QString>::iterator begin, end, it;

    entity->getAttributeIterator(begin, end);
    for (it = begin; it != end; ++it)
    {
        attrs[it.key()] = it.value();
    }

    item = window->addElement( 0,
                               -1,
                               entity->getType(),
                               entity->getUniqueId(),
                               attrs,
                               0, 0);

    idToItem[entity->getUniqueId()] = item;
    stack.push(entity);

    while(stack.size() > 0)
    {
        entity = stack.top();
        stack.pop();

        QVector <Entity *> children = entity->getChildren();
        for(int i = 0; i < children.size(); i++)
        {
            if(idToItem.contains(children.at(i)->getUniqueId())) continue;

            attrs.clear();
            children.at(i)->getAttributeIterator(begin, end);
            for (it = begin; it != end; ++it)
            {
                attrs[it.key()] = it.value();
            }

            item = window->addElement( idToItem[entity->getUniqueId()],
                                        -1,
                                        children.at(i)->getType(),
                                        children.at(i)->getUniqueId(),
                                        attrs,
                                        0, 0);

            idToItem[children.at(i)->getUniqueId()] = item;
            stack.push_front(children.at(i));
        }
    }
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

} } } //end namespace
