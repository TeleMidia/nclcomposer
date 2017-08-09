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

#ifndef MESSAGECONTROL_H
#define MESSAGECONTROL_H

#include <QObject>
#include <QUndoStack>

#include <exception>
using namespace std;

#include "util/ComposerCore_global.h"

#include "extensions/IDocumentParser.h"
#include "extensions/IPlugin.h"
#include "model/Entity.h"
#include "model/Comment.h"
#include "model/Project.h"
using namespace cpr::core;

CPR_CORE_BEGIN_NAMESPACE

enum class Message
{
  ENTITY_ADDED,
  ENTITY_REMOVED,
  ENTITY_CHANGED,
  COMMENT_ADDED
};
/*!
 * \ingroup core
 * \brief Manages the messages sent from plugins to NCL Composer Core and
 * vice-versa.
 */
class COMPOSERCORESHARED_EXPORT MessageControl : public QObject
{
  Q_OBJECT

public:
  /*!
   * \brief Constructor.
   *
   * \param project
   */
  explicit MessageControl (Project *project);
  /*!
   * \brief Destructor.
   */
  ~MessageControl ();

public slots:
  /*!
   * \brief addEntity
   * \param type
   * \param parentEntityId
   * \param atts
   */
  void addEntity (const QString &type, const QString &parentEntityId,
                  const QMap<QString, QString> &atts);
  /*!
   * \brief addContent
   * \param entity_content
   * \param parentId
   * \param format
   */
  void addContent (const QString &entity_content, const QString &parentId,
                   Data::Format format);
  /*!
   * \brief addComment
   * \param comment
   * \param parentId
   */
  void addComment (const QString &comment, const QString &parentId);

  /*!
   * \brief editEntity
   * \param entity
   * \param atts
   */
  void editEntity (Entity *entity, const QMap<QString, QString> &atts);

  /*!
   * \brief removeEntity
   * \param entity
   */
  void removeEntity (Entity *entity);

  /*!
   * \brief TODO
   */
  void setListenFilter (const QStringList &list);

  /*!
   * \brief TODO
   */
  void setPluginData (const QByteArray &data);

  /*!
   * \brief TODO
   */
  void setCurrentProjectAsDirty ();

  /*!
   * \brief This message is here, mainly for test purposes.
   *
   * It allows anyone send a addEntityMessage, even when it is not a plugin.
   */
  void anonymousAddEntity (const QString &type, const QString &parentEntityId,
                           const QMap<QString, QString> &atts,
                           bool notifyPlugins = true);

  /*!
   * \brief This is a variation of the previous message, where the user pass
   * the
   *    pointer to an entity already created.
   */
  void anonymousAddEntity (Entity *entity, const QString &parentEntityId,
                           bool notifyPlugins = true);

  /*!
   * \brief TODO.
   */
  void anonymousAddComment (Comment *comment, const QString &parentEntityId,
                             bool notifyPlugins = true);

  /*!
   * \brief This message is here, mainly for test purposes.
   *
   * It allows anyone to send a removeEntityMessage, even if it is not a
   * plugin.
   */
  void anonymousRemoveEntity (const QString &entityUniqueId,
                              bool notifyPlugins = true);
  /*!
   * \brief This message is here, mainly for test purposes.
   *
   * It allows anyone to send a addEntityMessage, even if it is not a plugin.
   */
  void anonymousUpdateFromModel ();

  /*!
   * \brief This message is here, mainly for test purposes.
   *
   * It allows anyone to send an addEntityMessage, even if it is not a plugin.
   */
  void anonymousChangeEntity (const QString &entityId,
                              const QMap<QString, QString> &atts,
                              bool notifyPlugins = true);

  void undo ();
  void redo ();

private:
  Project *_project; /*!< TODO */
  QMap<QString, QStringList> _listenEntities;
  QUndoStack *_qUndoStack;

  /*!
   * \brief sendEntityAddedMessageToPlugins
   * \param pluginInstanceId
   * \param entity
   */
  void sendMessageToPlugins (Message message, const QString &senderId,
                             Entity *entity);
  /*!
   * \brief sendCommentAddedMessageToPlugins
   * \param pluginInstanceId
   * \param entity
   */
  void sendCommentMessageToPlugins (Message message, const QString &senderId,
                                    Comment *comment);
  /*!
   * \brief TODO
   */
  bool pluginIsInterestedIn (const IPlugin *plugin, Entity *entity);
};

CPR_CORE_END_NAMESPACE

#endif // MESSAGECONTROL_H
