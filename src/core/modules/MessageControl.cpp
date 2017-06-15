/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

#include "modules/MessageControl.h"
#include "modules/PluginControl.h"

#include <QMetaObject>
#include <QMetaMethod>
#include <QApplication>
#include <QQueue>
#include <QDomDocument>

#include "util/Commands.h"

CPR_CORE_BEGIN_NAMESPACE

MessageControl::MessageControl(Project *project)
{
  this->_project = project;

  //Register MetaType allowing to be used by invoke
  qRegisterMetaType<Entity *>("Entity *");
  // qRegisterMetaType<QString>("QString");
  // qRegisterMetaType<const void *>("const Entity *");

  _qUndoStack = new QUndoStack(this);
}

MessageControl::~MessageControl()
{

}

void MessageControl::anonymousAddEntity( const QString &type,
                                         const QString &parentEntityId,
                                         const QMap<QString,QString>& atts,
                                         bool force,
                                         bool notifyPlugins)
{
  Q_UNUSED(force)

  Entity *ent = nullptr;

  try
  {
    ent = new Entity(atts, _project->getDomDocument());
    ent->setType(type);
    //TODO - calll validator to check
    _project->addEntity(ent, parentEntityId);

    //send message to All PLUGINS interested in this message.
    if(notifyPlugins)
      sendEntityAddedMessageToPlugins("", ent);
  }
  catch(exception& e)
  {
    delete ent;
    ent = nullptr;

    return;
  }
}

void MessageControl::anonymousAddEntity( Entity *entity,
                                         const QString &parentEntityId,
                                         bool force,
                                         bool notifyPlugins)
{
  Q_UNUSED(force)

  try
  {
    // \todo call validator to check
    _project->addEntity(entity, parentEntityId);

    //send message to All PLUGINS interested in this message.
    if(notifyPlugins)
      sendEntityAddedMessageToPlugins("", entity);
  }
  catch(exception& e)
  {
    // \todo notify error
    return;
  }
}

void MessageControl::anonymousRemoveEntity( const QString &entityUniqueId,
                                            bool force,
                                            bool notifyPlugins)
{
  Q_UNUSED(force)

  try
  {
    Entity *entity = _project->getEntityById(entityUniqueId);
    if(entity != nullptr)
    {
      //send message to All PLUGINS interested in this message.
      if(notifyPlugins)
        sendEntityRemovedMessageToPlugins("", entity);

      QApplication::processEvents();
      // \todo call validator to check
      _project->removeEntity(entity, false);
    }
  }
  catch(exception& e)
  {
    // \todo notify error
    return;
  }
}

void MessageControl::anonymousChangeEntity( const QString &entityId,
                                            const QMap<QString,QString>& atts,
                                            bool force,
                                            bool notifyPlugins)
{
  Q_UNUSED(force)

  Entity *ent = _project->getEntityById(entityId);
  if(ent != nullptr)
  {
    ent->setAtrributes(atts); //do it!

    //send message to All PLUGINS interested in this message.
    if(notifyPlugins)
      sendEntityChangedMessageToPlugins("", ent);
  }
}

void MessageControl::anonymousUpdateFromModel()
{
  QList<IPlugin*>::iterator it;
  QList<IPlugin*> instances =
      PluginControl::getInstance()->getPluginInstances(this->_project);

  QString slotName("updateFromModel()");
  for (it = instances.begin(); it != instances.end(); it++)
  {
    IPlugin *inst = *it;
    int idxSlot = inst->metaObject()
        ->indexOfSlot(slotName.toStdString().c_str());
    if(idxSlot != -1)
    {
      QMetaMethod method = inst->metaObject()->method(idxSlot);
      method.invoke(inst, Qt::DirectConnection);
    }
  }
}

void MessageControl::onAddEntity( const QString &type,
                                  const QString &parentEntityId,
                                  const QMap<QString,QString>& atts,
                                  bool force)
{
  Q_UNUSED(force)

  /* Cast to IPlugin to make sure it's a plugin */
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());
  IDocumentParser *parser = qobject_cast<IDocumentParser*>
      (QObject::sender());
  QString pluginID;

  if(plugin)
      pluginID = plugin->getPluginInstanceID();
  else if (parser)
      pluginID = parser->getParserName();

  Entity *ent = nullptr;

  try
  {
    ent = new Entity(atts, _project->getDomDocument());
    ent->setType(type);
    _qUndoStack->push(new AddCommand(_project, ent, parentEntityId));

    //send message to All PLUGINS interested in this message.
    sendEntityAddedMessageToPlugins(pluginID, ent);
  }
  catch(exception& e)
  {
    if (plugin)
      plugin->errorMessage(e.what());
    else if (parser)
      parser->onEntityAddError(e.what());

    delete ent;
    ent = nullptr;

    return;
  }
}

void MessageControl::onAddEntity(const QString &entity_content,
                                 const QString &parentId,
                                 Data::Format format,
                                 bool force)
{
  Q_UNUSED(force)

  /* Cast to IPlugin to make sure it's a plugin */
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());
  IDocumentParser *parser = qobject_cast<IDocumentParser*>
      (QObject::sender());
  QString pluginID;

  if(plugin)
      pluginID = plugin->getPluginInstanceID();
  else if (parser)
      pluginID = parser->getParserName();

  Entity *ent = nullptr;

  if (format == Data::XML)
  {
    //recursivelly go throught the elements
    QDomDocument doc ("doc");
    if(!doc.setContent(entity_content))
    {
      //We can not parse the DOM
      if (plugin)
        plugin->errorMessage(tr("Could not parse XML Content"));
      else if (parser)
       parser->onEntityAddError(tr("Could not parse XML Content"));
    }
    else
    {
      //Everything is all right. Now, its time to recursivelly add the entities
      QQueue <QDomElement> queue;
      QDomElement node, child;

      queue.push_back(doc.firstChildElement());
      doc.toElement().setAttribute("uuid", parentId);

      bool first = true;

      QMap<QString,QString> atts;
      while(queue.size())
      {
        node = queue.front();
        queue.pop_front();

        //Get the attributes from the DOM Element
        atts.clear();
        QDomNamedNodeMap nodeAtts = node.toElement().attributes();
        for(int i = 0; i < nodeAtts.count(); i++)
          atts[nodeAtts.item(i).nodeName()] = nodeAtts.item(i).nodeValue();

        //Create the new Entity
        ent = new Entity(atts, _project->getDomDocument());
        ent->setType(node.tagName());
        node.setAttribute("uuid", ent->getUniqueId());

        QString nodeParentUuid;
        if(first)
        {
          nodeParentUuid = parentId;
          first = false;
        }
        else
          nodeParentUuid = node.parentNode().toElement().attribute("uuid");

        // Do the addEntity in the core
        _qUndoStack->push(new AddCommand( _project, ent, nodeParentUuid));

        //send message to All PLUGINS interested in this message.
        sendEntityAddedMessageToPlugins(pluginID, ent);

        child = node.firstChildElement();
        while (!child.isNull())
        {
          queue.push_back(child);
          child = child.nextSiblingElement();
        }
      }
    }
  }
  else
  {
    qWarning() << "Data format is not recognized!!" << endl;
  }

  return;
}

void MessageControl::onAddComment(const QString &comment,
                                  const QString &parentId)
{
  qWarning () << "Trying to add comment :" << comment << parentId;
  return;
}

void MessageControl::onEditEntity( Entity *entity,
                                   const QMap<QString,QString> &atts,
                                   bool force)
{
  Q_UNUSED(force)

  assert (entity != nullptr);

  IPlugin *plugin = qobject_cast<IPlugin *>(QObject::sender());
  if(plugin)
  {
    QString pluginID = plugin->getPluginInstanceID();

    try
    {
      _qUndoStack->push(new EditCommand(_project, entity, atts));

      /*! \todo Call validator to check */
      // entity->setAtrributes(atts); //do it!

      // send message to all plugins interested in this message.
      sendEntityChangedMessageToPlugins(pluginID, entity);
    }
    catch(exception e)
    {
      plugin->errorMessage(e.what());
      return;
    }
  }
  else
  {
    //TODO -- erro on casting
    return;
  }

}

void MessageControl::onRemoveEntity(Entity *entity, bool force)
{
  Q_UNUSED(force)

  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());
  if(plugin)
  {
    QString pluginID = plugin->getPluginInstanceID();
    try
    {
      if(entity)
      {
        // Send message to all plugins interested in this message.
        // This message is send before the real delete to try to avoid
        // the plugins keep pointers to invalid memory location.
        QList <Entity*> willBeRemoved;
        QStack <Entity*> stack;

        //remove all children
        /**
         * \todo Change the following code to signal/slots with Project.
         */
        stack.push(entity);
        while(stack.size())
        {
          Entity *currentEntity = stack.top();

          // \fixme While sending messages to plugins it is possible that plugin
          // itself remove the entity (and in future we could get a trash.
          // That is the reason why we keep a clone here!!
          willBeRemoved.push_back(currentEntity->cloneEntity());

          stack.pop();

          QVector <Entity *> children = currentEntity->getChildren();
          for(int i = 0; i < children.size(); i++)
          {
            stack.push(children.at(i));
          }
        }

        /*!
         * \todo remember to change, the append should come from the
         *   plugin.
         */
        for(int i = willBeRemoved.size()-1; i >= 0; i--)
        {
          sendEntityRemovedMessageToPlugins(pluginID, willBeRemoved[i]);

          delete willBeRemoved[i]; // We do not need it anymore!!!
        }

//      This function will release the entity instance and its children
        // \fixme Since the plugin itself could already removed entity we can
        // get a trash here!!!
        // qUndoStack->push(new RemoveCommand(project, entity));
        _project->removeEntity(entity, true);
      }
      else
      {
        plugin->errorMessage(tr("You have tried to remove a nullptr entity!!"));
      }
    }
    catch(exception e)
    {
      plugin->errorMessage(e.what());
      return;
    }
  }
  else
  {
    /*! \todo error on casting management
        */
    return;
  }
}

void MessageControl::setListenFilter(const QStringList &entityList)
{
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());
  if(plugin)
  {
    _listenEntities[plugin->getPluginInstanceID()] = entityList;
  }
}

/*
 * \todo The implementation of the folling three implementations should be
 *   merged into only one function.
 */
void MessageControl::sendEntityAddedMessageToPlugins( const QString &pluginInstanceId,
                                                      Entity *entity)
{
  QList<IPlugin*>::iterator it;
  QList<IPlugin*> instances =
      PluginControl::getInstance()->getPluginInstances(this->_project);

  QString slotName("onEntityAdded(QString,Entity*)"); //Normalized Slot
  IPlugin *pluginMsgSrc = nullptr;

  for (it = instances.begin(); it != instances.end(); it++)
  {
    IPlugin *inst = *it;

    // \fixme: This is an workaround. I am delaying the calling for plugin that
    // triggered the message
    if (inst->getPluginInstanceID() == pluginInstanceId )
    {
      pluginMsgSrc = inst;
      continue;
    }

    if( pluginIsInterestedIn(inst, entity) )
    {
      int idxSlot = inst->metaObject()
          ->indexOfSlot(slotName.toStdString().c_str());

      if(idxSlot != -1)
      {
        QMetaMethod method = inst->metaObject()->method(idxSlot);
        method.invoke(inst, Qt::DirectConnection,
                      Q_ARG(const QString&, pluginInstanceId),
                      Q_ARG(Entity *, entity));
      }
    }
  }

  // \fixme: Now I call for the plugin that asked the message.
  if(pluginMsgSrc != nullptr && pluginIsInterestedIn(pluginMsgSrc, entity))
  {
    int idxSlot = pluginMsgSrc->metaObject()
        ->indexOfSlot(slotName.toStdString().c_str());

    if(idxSlot != -1)
    {
      QMetaMethod method = pluginMsgSrc->metaObject()->method(idxSlot);
      method.invoke(pluginMsgSrc, Qt::DirectConnection,
                    Q_ARG(const QString&, pluginInstanceId),
                    Q_ARG(Entity*, entity));
    }
  }

  emit entityAdded(pluginInstanceId, entity);
}


void MessageControl::sendEntityChangedMessageToPlugins(const QString &pluginInstanceId,
                                                       Entity *entity)
{
  QList<IPlugin*>::iterator it;
  QList<IPlugin*> instances =
      PluginControl::getInstance()->getPluginInstances(this->_project);

  QString slotName("onEntityChanged(QString,Entity*)");
  IPlugin *pluginMsgSrc = nullptr;

  for (it = instances.begin(); it != instances.end(); it++)
  {
    IPlugin *inst = *it;

    // \fixme: This is an workaround. I am delaying the calling for plugin that
    // triggered the message
    if (inst->getPluginInstanceID() == pluginInstanceId )
    {
      pluginMsgSrc = inst;
      continue;
    }

    if(pluginIsInterestedIn(inst, entity))
    {
      int idxSlot = inst->metaObject()
          ->indexOfSlot(slotName.toStdString().c_str());
      if(idxSlot != -1)
      {
        QMetaMethod method = inst->metaObject()->method(idxSlot);
        method.invoke(inst, Qt::DirectConnection,
                      Q_ARG(const QString&, pluginInstanceId),
                      Q_ARG(Entity*, entity));
      }
    }
  }

  // \fixme: Now I call for the plugin that asked the message.
  if(pluginMsgSrc != nullptr && pluginIsInterestedIn(pluginMsgSrc, entity))
  {
    int idxSlot = pluginMsgSrc->metaObject()
        ->indexOfSlot(slotName.toStdString().c_str());
    if(idxSlot != -1)
    {
      QMetaMethod method = pluginMsgSrc->metaObject()->method(idxSlot);
      method.invoke(pluginMsgSrc, Qt::DirectConnection,
                    Q_ARG(const QString&, pluginInstanceId),
                    Q_ARG(Entity*, entity));
    }
  }
}

void MessageControl::sendEntityRemovedMessageToPlugins(const QString &pluginInstanceId,
                                                       Entity *entity)
{
  QList<IPlugin*>::iterator it;
  QList<IPlugin*> instances =
      PluginControl::getInstance()->getPluginInstances(this->_project);

  QString slotName("onEntityRemoved(QString,QString)");
  IPlugin *pluginMsgSrc = nullptr;
  QString entityId = entity->getUniqueId();

  for (it = instances.begin(); it != instances.end(); it++)
  {
    IPlugin *inst = *it;

    // \fixme: This is an workaround. I am delaying the calling for plugin that
    // triggered the message
    if (inst->getPluginInstanceID() == pluginInstanceId )
    {
      pluginMsgSrc = inst;
      continue;
    }

    if(pluginIsInterestedIn(inst, entity))
    {
      int idxSlot = inst->metaObject()
          ->indexOfSlot(slotName.toStdString().c_str());
      if(idxSlot != -1)
      {
        QMetaMethod method = inst->metaObject()->method(idxSlot);
        method.invoke(inst, Qt::DirectConnection,
                      Q_ARG(const QString&, pluginInstanceId),
                      Q_ARG(const QString&, entityId));
      }
    }
  }

  // \fixme: Now I call for the plugin that asked the message.
  if(pluginMsgSrc != nullptr && pluginIsInterestedIn(pluginMsgSrc, entity))
  {
    int idxSlot = pluginMsgSrc->metaObject()
        ->indexOfSlot(slotName.toStdString().c_str());
    if(idxSlot != -1)
    {
      QMetaMethod method = pluginMsgSrc->metaObject()->method(idxSlot);
      method.invoke(pluginMsgSrc, Qt::DirectConnection,
                    Q_ARG(const QString&, pluginInstanceId),
                    Q_ARG(const QString&, entityId));
    }
  }
}

bool MessageControl::pluginIsInterestedIn(const IPlugin *plugin, Entity *entity)
{
  if(!_listenEntities.contains(plugin->getPluginInstanceID()))
  {
    // \todo An empty Entity should means ALL entities too?
    // Default: Plugin is interested in ALL entities
    return true;
  }

  if(_listenEntities.value(
       plugin->getPluginInstanceID()).contains(entity->getType())
     )
    return true;

  return false;
}

void MessageControl::setPluginData(const QByteArray &data)
{
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());

  // The plugin instance ID is composed of: pluginID#UniqueID
  // So, we have to take just the pluginID of that String:
  QString pluginId = plugin->getPluginInstanceID().left(
        plugin->getPluginInstanceID().indexOf("#"));

  _project->setPluginData(pluginId, data);
}

void MessageControl::setCurrentProjectAsDirty()
{
  _project->setDirty(true);
}

void MessageControl::undo()
{
  if(_qUndoStack->canUndo())
    _qUndoStack->undo();
}

void MessageControl::redo()
{
  if(_qUndoStack->canRedo())
    _qUndoStack->redo();
}

CPR_CORE_END_NAMESPACE
