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

#include <QApplication>
#include <QDomDocument>
#include <QMetaMethod>
#include <QMetaObject>
#include <QQueue>

#include "util/Commands.h"

CPR_CORE_BEGIN_NAMESPACE

MessageControl::MessageControl (Project *project)
{
  this->_project = project;

  // Register MetaType allowing to be used by invoke
  qRegisterMetaType<Entity *> ("Entity *");

  _qUndoStack = new QUndoStack (this);
}

MessageControl::~MessageControl () {}

void
MessageControl::anonymousAddEntity (const QString &type,
                                    const QString &parentEntityId,
                                    const QMap<QString, QString> &atts,
                                    bool notifyPlugins)
{
  Entity *ent = nullptr;

  try
  {
    ent = new Entity (atts, _project->getDomDocument ());
    ent->setType (type);
    // TODO - calll validator to check
    _project->addEntity (ent, parentEntityId);

    // send message to All PLUGINS interested in this message.
    if (notifyPlugins)
      sendMessageToPlugins (Message::ENTITY_ADDED, "", ent);
  }
  catch (exception &e)
  {
    delete ent;
  }
}

void
MessageControl::anonymousAddEntity (Entity *entity,
                                    const QString &parentEntityId,
                                    bool notifyPlugins)
{
  try
  {
    // \todo call validator to check
    _project->addEntity (entity, parentEntityId);

    // send message to All PLUGINS interested in this message.
    if (notifyPlugins)
      sendMessageToPlugins (Message::ENTITY_ADDED, "", entity);
  }
  catch (exception &e)
  {
    // \todo notify error
  }
}

void
MessageControl::anonymousAddComment (Comment *comment,
                                     const QString &parentEntityId,
                                     bool notifyPlugins)
{
  // \todo call validator to check
  _project->addComment (comment, parentEntityId);

  // send message to All PLUGINS interested in this message.
  if (notifyPlugins)
    sendCommentMessageToPlugins (Message::COMMENT_ADDED, "", comment);
}

void
MessageControl::anonymousRemoveEntity (const QString &entityUniqueId,
                                       bool notifyPlugins)
{
  try
  {
    Entity *entity = _project->getEntityById (entityUniqueId);
    if (entity != nullptr)
    {
      // send message to All PLUGINS interested in this message.
      if (notifyPlugins)
        sendMessageToPlugins (Message::ENTITY_REMOVED, "", entity);

      QApplication::processEvents ();
      // \todo call validator to check
      _project->removeEntity (entity, false);
    }
  }
  catch (exception &e)
  {
    // \todo notify error
  }
}

void
MessageControl::anonymousChangeEntity (const QString &entityId,
                                       const QMap<QString, QString> &atts,
                                       bool notifyPlugins)
{
  Entity *ent = _project->getEntityById (entityId);
  if (ent != nullptr)
  {
    ent->setAtrrs (atts);

    if (notifyPlugins)
      sendMessageToPlugins (Message::ENTITY_CHANGED, "", ent);
  }
}

void
MessageControl::anonymousUpdateFromModel ()
{
  QList<IPlugin *> instances
      = PluginControl::getInstance ()->getPluginInstances (this->_project);

  foreach (IPlugin *plugin, instances)
  {
    plugin->updateFromModel ();
  }
}

void
MessageControl::addEntity (const QString &type, const QString &parentEntityId,
                           const QMap<QString, QString> &atts)
{
  // Cast to IPlugin to make sure it's a plugin
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender ());
  IDocumentParser *parser
      = qobject_cast<IDocumentParser *> (QObject::sender ());

  QString senderId;
  if (plugin)
    senderId = plugin->getPluginInstanceID ();
  else if (parser)
    senderId = parser->getParserName ();

  Entity *ent = nullptr;

  try
  {
    ent = new Entity (atts, _project->getDomDocument ());
    ent->setType (type);
    _qUndoStack->push (new AddEntityCmd (_project, ent, parentEntityId));

    sendMessageToPlugins (Message::ENTITY_ADDED, senderId, ent);
  }
  catch (exception &e)
  {
    if (plugin)
      plugin->errorMessage (e.what ());
    else if (parser)
      parser->onEntityAddError (e.what ());

    delete ent;
    return;
  }
}

void
MessageControl::addContent (const QString &entity_content,
                            const QString &parentId, Data::Format format)
{
  /// Casts to IPlugin to make sure it's a plugin
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender ());
  IDocumentParser *parser
      = qobject_cast<IDocumentParser *> (QObject::sender ());

  QString senderId;
  if (plugin)
    senderId = plugin->getPluginInstanceID ();
  else if (parser)
    senderId = parser->getParserName ();

  Entity *ent = nullptr;

  if (format == Data::XML)
  {
    QDomDocument doc ("doc");
    if (!doc.setContent (entity_content))
    {
      if (plugin)
        plugin->errorMessage (tr ("Could not parse XML Content"));
      else if (parser)
        parser->onEntityAddError (tr ("Could not parse XML Content"));
    }
    else
    {
      // Everything is all right.  Now, its time to recursivelly add the
      // entities
      QQueue<QDomElement> queue;
      QDomElement node, child;

      queue.push_back (doc.firstChildElement ());
      doc.toElement ().setAttribute ("uuid", parentId);

      bool first = true;

      QMap<QString, QString> atts;
      while (queue.size ())
      {
        node = queue.front ();
        queue.pop_front ();

        // Gets the attributes from the DOM Element
        atts.clear ();
        QDomNamedNodeMap nodeAtts = node.toElement ().attributes ();
        for (int i = 0; i < nodeAtts.count (); i++)
          atts[nodeAtts.item (i).nodeName ()] = nodeAtts.item (i).nodeValue ();

        ent = new Entity (atts, _project->getDomDocument ());
        ent->setType (node.tagName ());
        node.setAttribute ("uuid", ent->getUniqueId ());

        QString nodeParentUuid;
        if (first)
        {
          nodeParentUuid = parentId;
          first = false;
        }
        else
        {
          nodeParentUuid = node.parentNode ().toElement ().attribute ("uuid");
        }

        _qUndoStack->push (new AddEntityCmd (_project, ent, nodeParentUuid));

        sendMessageToPlugins (Message::ENTITY_ADDED, senderId, ent);

        child = node.firstChildElement ();
        while (!child.isNull ())
        {
          queue.push_back (child);
          child = child.nextSiblingElement ();
        }
      }
    }
  }
  else
  {
    qCWarning (CPR_CORE) << "Data format is not recognized!!" << endl;
  }

  return;
}

void
MessageControl::addComment (const QString &content, const QString &parentId)
{
  qCWarning (CPR_CORE) << "MessageControl::addComment";

  // Cast to IPlugin to make sure it's a plugin
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender ());
  IDocumentParser *parser
      = qobject_cast<IDocumentParser *> (QObject::sender ());
  QString senderId;

  if (plugin)
    senderId = plugin->getPluginInstanceID ();
  else if (parser)
    senderId = parser->getParserName ();

  Comment *comment = nullptr;
  try
  {
    comment = new Comment (content, _project->getDomDocument (), _project->getEntityById(parentId));
    _qUndoStack->push (new AddCommentCmd (_project, comment, parentId));

    sendCommentMessageToPlugins (Message::COMMENT_ADDED, senderId, comment);
  }
  catch (exception &e)
  {
    if (plugin)
      plugin->errorMessage (e.what ());
    else if (parser)
      parser->onEntityAddError (e.what ());

    delete comment;
  }
  return;
}

void
MessageControl::editEntity (Entity *entity, const QMap<QString, QString> &atts)
{
  assert (entity != nullptr);

  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender ());
  if (plugin)
  {
    QString senderId = plugin->getPluginInstanceID ();

    try
    {
      _qUndoStack->push (new EditEntityCmd (_project, entity, atts));

      /*! \todo Call validator to check */
      // entity->setAtrributes(atts); //do it!

      // send message to all plugins interested in this message.
      sendMessageToPlugins (Message::ENTITY_CHANGED, senderId, entity);
    }
    catch (exception e)
    {
      plugin->errorMessage (e.what ());
      return;
    }
  }
  else
  {
    return;
  }
}

void
MessageControl::removeEntity (Entity *entity)
{
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender ());
  if (plugin)
  {
    QString pluginID = plugin->getPluginInstanceID ();
    try
    {
      if (entity)
      {
        // Send message to all plugins interested in this message.
        // This message is send before the real delete to try to avoid
        // the plugins keep pointers to invalid memory location.
        QList<Entity *> willBeRemoved;
        QStack<Node *> stack;

        // remove all children
        /**
     * \todo Change the following code to signal/slots with Project.
     */
        stack.push (entity);
        while (stack.size ())
        {
          Node *node = stack.top ();

          // \fixme While sending messages to plugins it is possible that
          // plugin itself remove the entity (and in future we could get a
          // trash.
          // That is the reason why we keep a clone here!!
          Entity *ent = dynamic_cast<Entity *> (node);
          if (ent)
            willBeRemoved.push_back (ent->clone ());

          stack.pop ();

          QList<Node *> children = node->getChildren ();
          for (auto i = children.begin (); i != children.end (); ++i)
          {
            stack.push (*i);
          }
        }

        /*!
     * \todo remember to change, the append should come from the
     *   plugin.
     */
        for (int i = willBeRemoved.size () - 1; i >= 0; i--)
        {
          sendMessageToPlugins (Message::ENTITY_REMOVED, pluginID,
                                willBeRemoved[i]);

          delete willBeRemoved[i]; // We do not need it anymore!!!
        }

        // This function will release the entity instance and its children
        // \fixme Since the plugin itself could already removed entity we
        // can get a trash here!!!
        // qUndoStack->push(new RemoveCommand(project, entity));
        _project->removeEntity (entity, true);
      }
      else
      {
        plugin->errorMessage (
            tr ("You have tried to remove a nullptr entity!!"));
      }
    }
    catch (exception e)
    {
      plugin->errorMessage (e.what ());
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

void
MessageControl::setListenFilter (const QStringList &entityList)
{
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender ());
  if (plugin)
  {
    _listenEntities[plugin->getPluginInstanceID ()] = entityList;
  }
}

/*
 * \todo The implementation of the following three implementations should be
 *   merged into only one function.
 */
void
MessageControl::sendMessageToPlugins (Message message, const QString &senderId,
                                      Entity *entity)
{
  QList<IPlugin *>::iterator it;
  QList<IPlugin *> instances
      = PluginControl::getInstance ()->getPluginInstances (this->_project);

  IPlugin *pluginMsgSrc = nullptr;

  for (it = instances.begin (); it != instances.end (); ++it)
  {
    IPlugin *inst = *it;

    // \fixme: This is an workaround. I am delaying the calling for the plugin
    // that triggered the message
    if (inst->getPluginInstanceID () == senderId)
    {
      pluginMsgSrc = inst;
      continue;
    }

    if (pluginIsInterestedIn (inst, entity))
    {
      switch (message)
      {
        case Message::ENTITY_ADDED:
          inst->onEntityAdded (senderId, entity);
          break;
        case Message::ENTITY_REMOVED:
          inst->onEntityRemoved (senderId, entity->getUniqueId ());
          break;
        case Message::ENTITY_CHANGED:
          inst->onEntityChanged (senderId, entity);
          break;
      }
    }
  }

  // \fixme: Now calling the plugin that asked the message.
  if (pluginMsgSrc != nullptr && pluginIsInterestedIn (pluginMsgSrc, entity))
  {
    switch (message)
    {
      case Message::ENTITY_ADDED:
        pluginMsgSrc->onEntityAdded (senderId, entity);
        break;
      case Message::ENTITY_REMOVED:
        pluginMsgSrc->onEntityRemoved (senderId, entity->getUniqueId ());
        break;
      case Message::ENTITY_CHANGED:
        pluginMsgSrc->onEntityChanged (senderId, entity);
        break;
    }
  }
}

void
MessageControl::sendCommentMessageToPlugins (Message message,
                                             const QString &senderId,
                                             Comment *comment)
{
  QList<IPlugin *>::iterator it;
  QList<IPlugin *> instances
      = PluginControl::getInstance ()->getPluginInstances (this->_project);

  IPlugin *pluginMsgSrc = nullptr;
  for (it = instances.begin (); it != instances.end (); ++it)
  {
    IPlugin *inst = *it;

    // \fixme: This is an workaround. I am delaying the calling for the plugin
    // that triggered the message
    if (inst->getPluginInstanceID () == senderId)
    {
      pluginMsgSrc = inst;
      continue;
    }

    switch (message)
    {
      case Message::COMMENT_ADDED:
        inst->onCommentAdded (senderId, comment);
        break;
    }
  }

  // \fixme: Now I call for the plugin that asked the message.
  if (pluginMsgSrc != nullptr)
  {
      switch (message)
      {
        case Message::COMMENT_ADDED:
          pluginMsgSrc->onCommentAdded (senderId, comment);
          break;
      }
  }
}
bool
MessageControl::pluginIsInterestedIn (const IPlugin *plugin, Entity *entity)
{
  if (!_listenEntities.contains (plugin->getPluginInstanceID ()))
  {
    // \todo An empty Entity should means ALL entities too?
    // Default: Plugin is interested in ALL entities
    return true;
  }

  if (_listenEntities.value (plugin->getPluginInstanceID ())
          .contains (entity->getType ()))
    return true;

  return false;
}

void
MessageControl::setPluginData (const QByteArray &data)
{
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender ());

  // The plugin instance ID is composed of: pluginID#UniqueID
  // So, we have to take just the pluginID of that String:
  QString pluginId = plugin->getPluginInstanceID ().left (
      plugin->getPluginInstanceID ().indexOf ("#"));

  _project->setPluginData (pluginId, data);
}

void
MessageControl::setCurrentProjectAsDirty ()
{
  _project->setDirty (true);
}

void
MessageControl::undo ()
{
  if (_qUndoStack->canUndo ())
    _qUndoStack->undo ();
}

void
MessageControl::redo ()
{
  if (_qUndoStack->canRedo ())
    _qUndoStack->redo ();
}

CPR_CORE_END_NAMESPACE
