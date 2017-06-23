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
#include "model/Entity.h"
#include "model/Project.h"
#include "extensions/IPlugin.h"
using namespace cpr::core;

CPR_CORE_BEGIN_NAMESPACE

/*!
 * \ingroup core
 * \brief Manages the messages sent from plugins to NCL Composer Core and
 * vice-versa.
 */
class COMPOSERCORESHARED_EXPORT MessageControl :
        public QObject
{
  Q_OBJECT

public:
  /*!
   * \brief Constructor.
   *
   * \param project
   */
  MessageControl(Project *_project);
  /*!
   * \brief Destructor.
   */
  ~MessageControl();

public slots:
  /*!
     \brief

     \param type
     \param parentEntityId
     \param atts
     \param force
    */
  void onAddEntity(const QString &type,
                   const QString &parentEntityId,
                   const QMap<QString,QString>& atts);
  /*!
     \brief

     \param type
     \param parentEntityId
     \param atts
     \param force
    */
  void onAddEntity(const QString &entity_content,
                   const QString &parentId,
                   Data::Format format);
  /*!
     \brief

     \param type
     \param parentEntityId
     \param atts
     \param force
    */
  void onAddComment(const QString &comment, const QString &parentId);

  /*!
     \brief

     \param entity
     \param atts
     \param force
    */
  void onEditEntity(Entity *entity, const QMap<QString,QString> &atts);

  /*!
     \brief

     \param
     \param force
    */
  void onRemoveEntity(Entity *entity);

  /*!
   * \brief TODO
   */
  void setListenFilter(const QStringList &list);

  /*!
   * \brief TODO
   */
  void setPluginData(const QByteArray &data);

  /*!
   * \brief TODO
   */
  void setCurrentProjectAsDirty();

  /*!
   * \brief This message is here, mainly for test purposes.
   *
   * It allows anyone send a addEntityMessage, even when it is not a plugin.
   */
  void anonymousAddEntity(const QString &type,
                          const QString &parentEntityId,
                          const QMap<QString,QString>& atts,
                          bool notifyPlugins = true);

  /*!
   * \brief This is a variation of the previous message, where the user pass the
   *    pointer to an entity already created.
   */
  void anonymousAddEntity(Entity *entity,
                          const QString &parentEntityId,
                          bool notifyPlugins = true);

  /*!
   * \brief This message is here, mainly for test purposes.
   *
   * It allows anyone send a removeEntityMessage, even if it is not a plugin.
   */
  void anonymousRemoveEntity(const QString &entityUniqueId,
                             bool notifyPlugins = true);
  /*!
   * \brief This message is here, mainly for test purposes.
   *
   * It allows anyone send a addEntityMessage, even if it is not a plugin.
   */
  void anonymousUpdateFromModel();

  /*!
   * \brief This message is here, mainly for test purposes.
   *
   * It allows anyone send an addEntityMessage, even if it is not a plugin.
   */
  void anonymousChangeEntity(const QString &entityId,
                             const QMap<QString,QString>& atts,
                             bool notifyPlugins = true);

  void undo();
  void redo();

signals:
  void entityAdded(const QString&, Entity*);


private:
  Project *_project; /*!< TODO */
  QMap <QString, QStringList> _listenEntities;
  QUndoStack *_qUndoStack;

  /*!
   * \brief TODO
   */
  void sendEntityAddedMessageToPlugins(const QString &pluginInstanceId,
                                       Entity *entity );
  /*!
   * \brief TODO
   */
  void sendEntityChangedMessageToPlugins(const QString &pluginInstanceId,
                                         Entity *entity );
  /*!
   * \brief TODO
   */
  void sendEntityRemovedMessageToPlugins(const QString &pluginInstanceId,
                                         Entity *entity );

  /*!
   * \brief TODO
   */
  bool pluginIsInterestedIn(const IPlugin *plugin, Entity *entity);
};

CPR_CORE_END_NAMESPACE

#endif // MESSAGECONTROL_H
