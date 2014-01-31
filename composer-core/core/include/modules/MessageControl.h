/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef MESSAGECONTROL_H
#define MESSAGECONTROL_H

#include <QObject>
#include <QDebug>
#include <QUndoStack>

#include <exception>
using namespace std;

#include "../util/ComposerCoreControl_global.h"

#include "../extensions/IDocumentParser.h"
using namespace composer::extension;

#include "../model/Entity.h"
#include "../model/Project.h"
using namespace composer::core::model;

#include "../extensions/IPlugin.h"
using namespace composer::extension;

namespace composer {
  namespace core {

/*!
 \brief Manages the messages sent from plugins to NCL Composer Core and
    vice-versa.
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
  MessageControl(Project *project);
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
  void onAddEntity( QString type,
                    QString parentEntityId,
                    QMap<QString,QString>& atts,
                    bool force );
  /*!
     \brief

     \param entity
     \param atts
     \param force
    */
  void onEditEntity(Entity *entity, QMap<QString,QString> atts, bool force);
  /*!
     \brief

     \param
     \param force
    */
  void onRemoveEntity(Entity *, bool force);
  /*!
   * \brief TODO
   */
  void setListenFilter(QStringList list);

  /*!
   * \brief TODO
   */
  void setPluginData(QByteArray data);

  /*!
   * \brief TODO
   */
  void setCurrentProjectAsDirty();

  /*!
   * \brief This message is here, mainly for test purposes.
   *
   * It allows anyone send a addEntityMessage, even when it is not a plugin.
   */
  void anonymousAddEntity(QString type,
                          QString parentEntityId,
                          QMap<QString,QString>& atts,
                          bool force = false,
                          bool notifyPlugins = true);

  /*!
   * \brief This is a variation of the previous message, where the user pass the
   *    pointer to an entity already created.
   */
  void anonymousAddEntity( Entity *entity,
                           QString parentEntityId,
                           bool force = false,
                           bool notifyPlugins = true);

  /*!
   * \brief This message is here, mainly for test purposes.
   *
   * It allows anyone send a removeEntityMessage, even if it is not a plugin.
   */
  void anonymousRemoveEntity( QString entityUniqueId,
                              bool force = false,
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
  void anonymousChangeEntity( QString entityId,
                              QMap<QString,QString>& atts,
                              bool force = false,
                              bool notifyPlugins = true);

  void undo();
  void redo();

signals:
  void entityAdded(QString, Entity*);


private:
  Project *project; /*!< TODO */
  QMap <QString, QStringList> listenEntities;
  QUndoStack *qUndoStack;

  /*!
   * \brief TODO
   */
  void sendEntityAddedMessageToPlugins( QString pluginInstanceId,
                                        Entity *entity );
  /*!
   * \brief TODO
   */
  void sendEntityChangedMessageToPlugins( QString pluginInstanceId,
                                          Entity *entity );
  /*!
   * \brief TODO
   */
  void sendEntityRemovedMessageToPlugins( QString pluginInstanceId,
                                          Entity *entity );

  /*!
   * \brief TODO
   */
  bool pluginIsInterestedIn(IPlugin *plugin, Entity *entity);
};
} } //end namespace
#endif // MESSAGECONTROL_H
