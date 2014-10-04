/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "modules/MessageControl.h"
#include "modules/PluginControl.h"

#include <QMetaObject>
#include <QMetaMethod>
#include <QApplication>

#include "util/Commands.h"

namespace composer {
namespace core {

MessageControl::MessageControl(Project *project)
{
  this->project = project;

  //Register MetaType allowing to be used by invoke
  qRegisterMetaType<void *>("Entity *");

  qUndoStack = new QUndoStack(this);
}

MessageControl::~MessageControl()
{

}

void MessageControl::anonymousAddEntity( QString type,
                                         QString parentEntityId,
                                         QMap<QString,QString>& atts,
                                         bool force,
                                         bool notifyPlugins)
{
  Entity *ent = NULL;

  try
  {
    ent = new Entity(atts);
    ent->setType(type);
    //TODO - calll validator to check
    project->addEntity(ent, parentEntityId);

    //send message to All PLUGINS interested in this message.
    if(notifyPlugins)
      sendEntityAddedMessageToPlugins("", ent);
  }
  catch(exception& e)
  {
    delete ent;
    ent = NULL;

    return;
  }
}

void MessageControl::anonymousAddEntity( Entity *entity,
                                         QString parentEntityId,
                                         bool force,
                                         bool notifyPlugins)
{
  try
  {
    // \todo call validator to check
    project->addEntity(entity, parentEntityId);

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

void MessageControl::anonymousRemoveEntity( QString entityUniqueId, bool force,
                            bool notifyPlugins)
{
  try
  {
    Entity *entity = project->getEntityById(entityUniqueId);
    if(entity != NULL)
    {
      //send message to All PLUGINS interested in this message.
      if(notifyPlugins)
        sendEntityRemovedMessageToPlugins("", entity);

      QApplication::processEvents();
      // \todo call validator to check
      project->removeEntity(entity, false);
    }
  }
  catch(exception& e)
  {
    // \todo notify error
    return;
  }
}

void MessageControl::anonymousChangeEntity( QString entityId,
                                            QMap<QString,QString>& atts,
                                            bool force,
                                            bool notifyPlugins)
{
  Entity *ent = project->getEntityById(entityId);
  if(ent != NULL)
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
      PluginControl::getInstance()->getPluginInstances(this->project);

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

void MessageControl::onAddEntity( QString type, QString parentEntityId,
                                  QMap<QString,QString>& atts, bool force)
{
  /* Cast to IPlugin to make sure it's a plugin */
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());
  IDocumentParser *parser = qobject_cast<IDocumentParser*>
      (QObject::sender());
  QString pluginID;

  if(plugin) pluginID = plugin->getPluginInstanceID();
  else if (parser) pluginID = parser->getParserName();

  Entity *ent = NULL;

  try
  {
    ent = new Entity(atts);
    ent->setType(type);
    qUndoStack->push(new AddCommand(project, ent, parentEntityId));

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
    ent = NULL;

    return;
  }
}

void MessageControl::onEditEntity(Entity *entity, QMap<QString,QString> atts,
                                  bool force)
{
  assert (entity != NULL);

  IPlugin *plugin = qobject_cast<IPlugin *>(QObject::sender());
  if(plugin)
  {
    QString pluginID = plugin->getPluginInstanceID();

    try
    {
      qUndoStack->push(new EditCommand(project, entity, atts));

      /*! \todo Call validator to check */
      entity->setAtrributes(atts); //do it!

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
        qUndoStack->push(new RemoveCommand(project, entity));
//        project->removeEntity(entity, true);
      }
      else
      {
        plugin->errorMessage(tr("You have tried to remove a NULL entity!!"));
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

void MessageControl::setListenFilter(QStringList entityList)
{
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());
  if(plugin)
  {
    listenEntities[plugin->getPluginInstanceID()] = entityList;
  }
}

/*
 * \todo The implementation of the folling three implementations should be
 *   merged into only one function.
 */
void MessageControl::sendEntityAddedMessageToPlugins( QString pluginInstanceId,
                                                      Entity *entity)
{
  QList<IPlugin*>::iterator it;
  QList<IPlugin*> instances =
      PluginControl::getInstance()->getPluginInstances(this->project);

  QString slotName("onEntityAdded(QString,Entity*)");

  for (it = instances.begin(); it != instances.end(); it++)
  {
    IPlugin *inst = *it;

    if(pluginIsInterestedIn(inst, entity))
    {
      int idxSlot = inst->metaObject()
          ->indexOfSlot(slotName.toStdString().c_str());
      if(idxSlot != -1)
      {
        QMetaMethod method = inst->metaObject()->method(idxSlot);
        method.invoke(inst, Qt::DirectConnection,
                      Q_ARG(QString, pluginInstanceId),
                      Q_ARG(Entity*, entity));
      }
    }
  }

  emit entityAdded(pluginInstanceId, entity);
}


void MessageControl::sendEntityChangedMessageToPlugins(QString pluginInstanceId,
                                                       Entity *entity)
{
  QList<IPlugin*>::iterator it;
  QList<IPlugin*> instances =
      PluginControl::getInstance()->getPluginInstances(this->project);

  QString slotName("onEntityChanged(QString,Entity*)");

  for (it = instances.begin(); it != instances.end(); it++)
  {
    IPlugin *inst = *it;

    if(pluginIsInterestedIn(inst, entity))
    {
      int idxSlot = inst->metaObject()
          ->indexOfSlot(slotName.toStdString().c_str());
      if(idxSlot != -1)
      {
        QMetaMethod method = inst->metaObject()->method(idxSlot);
        method.invoke(inst, Qt::DirectConnection,
                      Q_ARG(QString, pluginInstanceId),
                      Q_ARG(Entity*, entity));
      }
    }
  }
}

void MessageControl::sendEntityRemovedMessageToPlugins(QString pluginInstanceId,
                                                       Entity *entity)
{
  QList<IPlugin*>::iterator it;
  QList<IPlugin*> instances =
      PluginControl::getInstance()->getPluginInstances(this->project);

  QString slotName("onEntityRemoved(QString,QString)");
  QString entityId = entity->getUniqueId();

  for (it = instances.begin(); it != instances.end(); it++)
  {
    IPlugin *inst = *it;

    if(pluginIsInterestedIn(inst, entity))
    {
      int idxSlot = inst->metaObject()
          ->indexOfSlot(slotName.toStdString().c_str());
      if(idxSlot != -1)
      {
        QMetaMethod method = inst->metaObject()->method(idxSlot);
        method.invoke(inst, Qt::DirectConnection,
                      Q_ARG(QString, pluginInstanceId),
                      Q_ARG(QString, entityId));
      }
    }
  }
}

bool MessageControl::pluginIsInterestedIn(IPlugin *plugin, Entity *entity)
{
  if(!listenEntities.contains(plugin->getPluginInstanceID()))
  {
    // \todo An empty Entity should means ALL entities too?
    // Default: Plugin is interested in ALL entities
    return true;
  }

  if(listenEntities.value(
       plugin->getPluginInstanceID()).contains(entity->getType())
     )
    return true;

  return false;
}

void MessageControl::setPluginData(QByteArray data)
{
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());

  // The plugin instance ID is composed of: pluginID#UniqueID
  // So, we have to take just the pluginID of that String:
  QString pluginId = plugin->getPluginInstanceID().left(
        plugin->getPluginInstanceID().indexOf("#"));

  project->setPluginData(pluginId, data);
}

void MessageControl::setCurrentProjectAsDirty()
{
  project->setDirty(true);
}

void MessageControl::undo()
{
  if(qUndoStack->canUndo())
    qUndoStack->undo();
}

void MessageControl::redo()
{
  if(qUndoStack->canRedo())
    qUndoStack->redo();
}

} } //end namespace
