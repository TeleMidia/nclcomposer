/*
 * Copyright 2011 TeleMidia/PUC-Rio.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>. 
 */
#include "OutlineViewPlugin.h"

#include "core/modules/LanguageControl.h"

OutlineViewPlugin::OutlineViewPlugin() :
  window(new NCLTreeWidget(0)), windowBuffering(new NCLTreeWidget(0))
{
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
    if(selectedId != NULL)
        delete selectedId;
    delete window;
    delete windowBuffering;
}

QWidget* OutlineViewPlugin::getWidget()
{
    return window;
}

void OutlineViewPlugin::onEntityAdded(QString pluginID, Entity *entity)
{
    (void) pluginID;
//  QString line = "<" + entity->getType() + "> </" + entity->getType() + ">\n";

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

    if(entity->getType() == "ncl" ||
       entity->getType() == "body" ||
       entity->getType() == "link" ||
       entity->getType() == "media" ||
       entity->getType() == "context" ||
       entity->getType() == "switch" ||
       entity->getType() == "port" ||
       entity->getType() == "switchPort" ||
       entity->getType() == "regionBase" ||
       entity->getType() == "descriptorBase" ||
       entity->getType() == "connectorBase" ||
       entity->getType() == "ruleBase" ||
       entity->getType() == "transitionBase" )
    {
      if(!attrs.keys().contains("id"))
      {
        attrs.insert("id", project->generateUniqueNCLId(entity->getType()));
        emit setAttributes(entity, attrs, false);
      }
    }

    // \todo This must be incremental
    clearErrorMessages();
//    emit sendBroadcastMessage("askAllValidationMessages", NULL);
}

void OutlineViewPlugin::errorMessage(QString error)
{
    qDebug() << "OutlineViewPlugin::onEntityAddError(" << error << ")";
}

void OutlineViewPlugin::onEntityChanged(QString pluginID, Entity * entity)
{
    QMap <QString, QString> attrs;
    QMap <QString, QString>::iterator begin, end, it;

    entity->getAttributeIterator(begin, end);
    for (it = begin; it != end; ++it)
    {
        attrs[it.key()] = it.value();
    }

    idToItem[entity->getUniqueId()]->setTextColor(0, Qt::black);
    idToItem[entity->getUniqueId()]->setToolTip(0, "");

    window->updateItem(idToItem[entity->getUniqueId()], entity->getType(),
                       attrs);

    // \todo This must be incremental
    clearErrorMessages();
//    emit sendBroadcastMessage("askAllValidationMessages", NULL);
}

void OutlineViewPlugin::onEntityRemoved(QString pluginID, QString entityID)
{
    (void) pluginID;
//    qDebug() << "OutlineViewPlugin::onEntityRemoved ("<< pluginID << " "
//            << entityID << ")";
//    qDebug() << idToItem.contains(entityID);

    if(idToItem.contains(entityID))
    {
        idToItem.remove(entityID);
        window->removeItem(entityID);
        if (selectedId != NULL && entityID == *selectedId)
        {
            delete selectedId;
            selectedId = NULL;
        }
    }

    clearErrorMessages();
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

void OutlineViewPlugin::debugHasSendClearAll(QString pluginID, void *param)
{
    (void) pluginID;
    (void) param;

    qDebug() << "OutlineViewPlugin::debugHasSendClearAll";
}

void OutlineViewPlugin::itemSelectionChanged()
{
    if(selectedId != NULL)
    {
        delete selectedId;
        selectedId = NULL;
    }

    QList <QTreeWidgetItem*> selecteds = window->selectedItems();

    if(selecteds.size())
    {
        selectedId = new QString(selecteds.at(0)->text(2));
        emit sendBroadcastMessage("changeSelectedEntity", selectedId);
    }
}

void OutlineViewPlugin::changeSelectedEntity(QString pluginID, void *param){
    if(pluginID != this->pluginInstanceID)
    {
        QString *id = (QString*)param;
        QTreeWidgetItem *item = window->getItemById(*id);
        if(item != NULL)
            window->setCurrentItem(item, 0);
        else
            qWarning() << "The OutlineViewPlugin receive a message to select an"
                    << " Entity that it doesn't know.";
    }
}

void OutlineViewPlugin::textualStartSync(QString, void*)
{
  window->collapseAll();
}

void OutlineViewPlugin::textualFinishSync(QString, void*)
{
  window->expandAll();
}

void OutlineViewPlugin::clearErrorMessages()
{
  foreach (QTreeWidgetItem *item, idToItem.values())
  {
    item->setTextColor(0, Qt::black);
    item->setToolTip(0, "");
  }
}

void OutlineViewPlugin::validationError(QString pluginID, void * param)
{
  if (param)
  {
     pair <QString , QString> *p = (pair <QString, QString> *) param;

     QString uid = p->first;

     QTreeWidgetItem *item = window->getItemById(uid);
     if(item != NULL)
     {
       item->setTextColor(0, Qt::red);
       item->setToolTip(0, p->second);
     }
  }
}
