/*
 * Copyright 2011-2013 TeleMidia/PUC-Rio.
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

#include "modules/LanguageControl.h"

OutlineViewPlugin::OutlineViewPlugin() :
  _window(new NCLTreeWidget(0)),
  _windowBuffering(new NCLTreeWidget(0))
{
  project = nullptr;

  connect ( _window,
            SIGNAL( elementAddedByUser ( QString,
                                         QString,
                                         QMap <QString, QString> &,
                                         bool)),
            this,
            SLOT( elementAddedByUser( QString,
                                      QString,
                                      QMap <QString, QString> &,
                                      bool)));

  connect (_window, SIGNAL(elementRemovedByUser(QString)),
           this, SLOT(elementRemovedByUser(QString)));

  connect(_window,
          SIGNAL(itemSelectionChanged()),
          this,
          SLOT(itemSelectionChanged()));

  connect(_window,
          SIGNAL(userAskedToOpenWithDefaultSystemEditor(QString)),
          this,
          SLOT(openWithDefaultSystemEditor(QString)));

  _selectedId = nullptr;
  _isSyncingFromTextual = false;
}

OutlineViewPlugin::~OutlineViewPlugin()
{
  if(_selectedId != nullptr)
    delete _selectedId;

  delete _window;
  delete _windowBuffering;
}

QWidget* OutlineViewPlugin::getWidget()
{
  return _window;
}

void OutlineViewPlugin::onEntityAdded(const QString &pluginID, Entity *entity)
{
  Q_UNUSED(pluginID)

  if(_isSyncingFromTextual)
    return;

  qDebug() << "OutlineViewPlugin::onEntityAdded(" << pluginID << entity << endl;

  QTreeWidgetItem *item;
  QMap <QString, QString> attrs;
  QMap <QString, QString>::iterator begin, end, it;

  entity->getAttributeIterator(begin, end);
  for (it = begin; it != end; ++it)
  {
    attrs[it.key()] = it.value();
  }

  if(_idToItem.contains(entity->getParentUniqueId()))
  {
    item = _window->addElement( _idToItem[entity->getParentUniqueId()],
        -1,
        entity->getType(),
        entity->getUniqueId(),
        attrs,
        0, 0);
  }
  else
  {
    item = _window->addElement( 0,
                                -1,
                                entity->getType(),
                                entity->getUniqueId(),
                                attrs,
                                0, 0);
  }

  _idToItem[entity->getUniqueId()] = item;

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
  // emit sendBroadcastMessage("askAllValidationMessages", nullptr);
}

void OutlineViewPlugin::errorMessage(const QString &error)
{
  qDebug() << "OutlineViewPlugin::onEntityAddError(" << error << ")";
}

void OutlineViewPlugin::onEntityChanged(const QString &pluginID, Entity * entity)
{
  Q_UNUSED(pluginID)

  if(_isSyncingFromTextual)
    return;

  QMap <QString, QString> attrs;
  QMap <QString, QString>::iterator begin, end, it;

  entity->getAttributeIterator(begin, end);
  for (it = begin; it != end; ++it)
  {
    attrs[it.key()] = it.value();
  }

  // \fixme This "if" should not be here. It is here because after adding an
  // entity, layout view change their content (before outline add the region)
  // so it will receive an change before the added. CORE should handle that to
  // the operations be received in order.
  if(_idToItem.contains(entity->getUniqueId()))
  {
    _idToItem[entity->getUniqueId()]->setTextColor(0, Qt::black);
    _idToItem[entity->getUniqueId()]->setToolTip(0, "");

    _window->updateItem( _idToItem[entity->getUniqueId()],
                         entity->getType(),
                         attrs );
  }

  // \todo This must be incremental
  clearErrorMessages();
  // emit sendBroadcastMessage("askAllValidationMessages", nullptr);
}

void OutlineViewPlugin::onEntityRemoved( const QString &pluginID,
                                         const QString &entityID )
{
  if(_isSyncingFromTextual)
    return;

  (void) pluginID;

  if(_idToItem.contains(entityID))
  {
    _idToItem.remove(entityID);
    _window->removeItem(entityID);
    if (_selectedId != nullptr && entityID == *_selectedId)
    {
      delete _selectedId;
      _selectedId = nullptr;
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

  qWarning() << "OutlineViewPlugin::elementAddedByUser calling addEntity";
  emit addEntity(type, parentId, atts, force);
}

bool OutlineViewPlugin::saveSubsession()
{
  //TODO: All
  return true;
}

void OutlineViewPlugin::updateFromModel()
{
  _window->hide();
  _window->collapseAll();

  // \todo This could be a default implementation for updateFromModel
  _window->clear();
  _idToItem.clear();

  if(project->getChildren().size())
  {
    Entity *entity = project;
    QList <Entity *> entities;
    entities.push_back(entity);
    bool first = true;

    while(entities.size())
    {
      entity = entities.front();
      entities.pop_front();

      if(!first) //ignore the project root
        onEntityAdded("xxx", entity);
      else
        first = false;

      QVector<Entity *> children = entity->getChildren();
      for(int i = 0; i < children.size(); i++)
      {
        entities.push_back(children.at(i));
      }
    }
  }

  _window->expandAll();
  _window->show();
}

void OutlineViewPlugin::init()
{
  //Clear previous tree
  QString key;
  foreach(key, _idToItem.keys())
  {
    _window->removeItem(key);
  }
  _idToItem.clear();

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

  item = _window->addElement( 0,
                              -1,
                              entity->getType(),
                              entity->getUniqueId(),
                              attrs,
                              0, 0);

  _idToItem[entity->getUniqueId()] = item;
  stack.push(entity);

  while(stack.size() > 0)
  {
    entity = stack.top();
    stack.pop();

    QVector <Entity *> children = entity->getChildren();
    for(int i = 0; i < children.size(); i++)
    {
      if(_idToItem.contains(children.at(i)->getUniqueId())) continue;

      attrs.clear();
      children.at(i)->getAttributeIterator(begin, end);
      for (it = begin; it != end; ++it)
      {
        attrs[it.key()] = it.value();
      }

      item = _window->addElement( _idToItem[entity->getUniqueId()],
          -1,
          children.at(i)->getType(),
          children.at(i)->getUniqueId(),
          attrs,
          0, 0);

      _idToItem[children.at(i)->getUniqueId()] = item;
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
  if(_selectedId != nullptr)
  {
    delete _selectedId;
    _selectedId = nullptr;
  }

  QList <QTreeWidgetItem*> selecteds = _window->selectedItems();

  if(selecteds.size())
  {
    _selectedId = new QString(selecteds.at(0)->text(2));
    emit sendBroadcastMessage("changeSelectedEntity", _selectedId);
  }
}

void OutlineViewPlugin::changeSelectedEntity(QString pluginID, void *param)
{
  if(_isSyncingFromTextual)
    return;

  if(pluginID != this->pluginInstanceID)
  {
    QString *id = (QString*)param;
    QTreeWidgetItem *item = _window->getItemById(*id);
    if(item != nullptr)
      _window->setCurrentItem(item, 0);
    else
      qWarning() << "The OutlineViewPlugin receive a message to select an"
                 << " Entity that it doesn't know.";
  }
}

void OutlineViewPlugin::textualStartSync(QString, void*)
{
  _isSyncingFromTextual = true;
}

void OutlineViewPlugin::textualFinishSync(QString, void*)
{
  _isSyncingFromTextual = false;

  updateFromModel();
}

void OutlineViewPlugin::clearErrorMessages()
{
  if(_isSyncingFromTextual)
    return;

  qDebug() << "OutlineViewPlugin::clearErrorMessages" << endl;

  foreach (QTreeWidgetItem *item, _idToItem.values())
  {
    item->setTextColor(0, Qt::black);
    item->setToolTip(0, "");
  }
}

void OutlineViewPlugin::clearValidationMessages(QString pluginID, void *param)
{
  Q_UNUSED(pluginID)
  Q_UNUSED(param)

  clearErrorMessages();
}

void OutlineViewPlugin::validationError(QString pluginID, void *param)
{
  Q_UNUSED(pluginID)

  if(_isSyncingFromTextual)
    return;

  if (param)
  {
    pair <QString , QString> *p = (pair <QString, QString> *) param;

    QString uid = p->first;

    QTreeWidgetItem *item = _window->getItemById(uid);
    if(item != nullptr)
    {
      item->setTextColor(0, Qt::red);
      item->setToolTip(0, p->second);
    }
  }
}

void OutlineViewPlugin::openWithDefaultSystemEditor(QString entityId)
{
  QString url;
  Entity *entity = project->getEntityById(entityId);
  QFileInfo projectInfo(project->getLocation());

  if(entity->getType() == "media")
  {
    url = entity->getAttribute("src");
  }
  else if(entity->getType() == "importBase")
  {
    url = entity->getAttribute("documentURI");
  }

  if(!QFile::exists(url))
    url = projectInfo.absolutePath() + "/" + url;

  if(QFile::exists(url))
    QDesktopServices::openUrl(url);
  else
    QMessageBox::warning(nullptr,
                         tr("Warning"),
                         tr("Could not open file. File does not exist!"));
}
