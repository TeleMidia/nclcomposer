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

#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QEvent>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>

#include "model/Entity.h"
#include "model/Project.h"
#include "modules/LanguageControl.h"
using namespace cpr::core;

CPR_CORE_BEGIN_NAMESPACE

namespace Data
{
enum Format
{
  XML,
  JSON
};
}

/*!
 * \ingroup core
 * \brief The interface for communication between the core and the plugin.
 *
 * This extension point is used by the core to emit and receive signals.
 * These signals are transmited by the plugin to the core, and then the core
 * retransmit this signal to the other plugins associated with the NclDocument.
 * All the communication is performed using the signal/slot mechanism
 * provided by QT. The plugin developer needs to implement all the slots in
 * order to capture the changes in the NclDocument and emit the signals when
 * itself makes a change.
 *
 * In short, the SLOT are messages from the CORE to PLUGIN, while the
 * signals bellow are messages from PLUGIN to CORE.
 */
class COMPOSERCORESHARED_EXPORT IPlugin : public QObject
{
  Q_OBJECT

public:
  inline QString
  getPluginInstanceID () const
  {
    return this->pluginInstanceID;
  }

  inline void
  setPluginInstanceID (const QString &pluginInstID)
  {
    this->pluginInstanceID = pluginInstID;
  }

  inline void
  setLanguageProfile (ILanguageProfile *languageProfile)
  {
    this->languageProfile = languageProfile;
  }

  inline ILanguageProfile *
  getLanguageProfile ()
  {
    return this->languageProfile;
  }

  /*!
   * This call is used by the core to bind an existing NclDocument with this
   * plugin instance.
   * \param document an NclDocument instance
   */
  inline void
  setProject (Project *project)
  {
    QMutexLocker locker (&mutex);
    this->project = project;
  }

  /*!
   * \brief This call is used by the core to recover the Project
   * instance previously binded with this plugin instance.
   *
   * \return project aProject instance
   */
  inline Project *
  getProject ()
  {
    QMutexLocker locker (&mutex);
    return this->project;
  }

  /*!
   * \brief Return the main Widget.
   *
   * The core calls this method to retrieve the Plugin Graphical
   * Interface. This Widget is going to be wrapped into a QDockWidget
   * and then displayed for the user in the main interface
   *
   * \return QWidget - wrapping the plugin interface
   */
  virtual QWidget *
  getWidget ()
  {
    return nullptr;
  }

  /*!
   * \brief Says to plugin save its settings.
   *
   * The core calls this method to notify the plugin that the user
   * is requesting to save this document.
   * Some plugins need to save special settings for a particular
   * document, in case that document is re-open in a different
   * session.
   *
   *   \return bool - true if the save was successfull,
   *                  false otherwise
   */
  virtual bool
  saveSubsession ()
  {
    return true;
  }

public slots:
  /*!
   * \brief Initialize the plugin.
   *
   * This call is used after the plugin is connected with core.
   * Just after the init return the plugin is able to receive messages from
   * core.
   * Messages send to core befor this call are IGNORED.
   */
  virtual void init () /* = 0; */ {}

  /*!
   * \brief Calls the plugin to update its internal model from model.
   *
   * This call is invoked by the core in two situations:
   *     - When the user resquest an update to all plugins
   *     - When the plugin is loaded in runtime
   *
   * In the second case, the plugin is loaded, but did no receive
   * the previous modifications, so it has to be forceed to reload
   * the Document*.
   */
  virtual void updateFromModel () /*= 0*/ {}

  /*!
   * \brief This is called by the core when a new Entity is added.
   *
   * This call is invoked by the core when a new Entity (from that
   * particular types the plugin is listening) is added in the model.
   */
  virtual void onEntityAdded (const QString &pluginID, Entity *entity) /*= 0*/
  {
    Q_UNUSED (pluginID);
    Q_UNUSED (entity);
  }

  /*!
   * \brief This is called by the core when a new Comment is added.
   *
   * This call is invoked by the core when a new Comment is added in the model.
   */
  virtual void onCommentAdded (const QString &pluginID,
                               Comment *entity) /*= 0*/
  {
    Q_UNUSED (pluginID);
    Q_UNUSED (entity);
  }

  /*!
   * \brief Called by the core when an Entity is changed.
   *
   * \param pluginID the plugin instance id that first called the changes to
   *     the entity.
   * \param entity The entity that was modified.
   */
  virtual void onEntityChanged (const QString &pluginID,
                                Entity *entity) /*= 0*/
  {
    Q_UNUSED (pluginID);
    Q_UNUSED (entity);
  }

  /*!
   * \brief Called by the core before the Entity is removed.
   *
   * \deprecated.
   */
  // virtual void onEntityAboutToRemove(Entity *) = 0;

  /*!
   * \brief Called by the core after the entity entityID is removed.
   *
   * \param pluginID the plugin instance id that first called the changes to
   *     the entity.
   * \param entityID the entity's identifier that was removed.
   */
  virtual void onEntityRemoved (const QString &pluginID,
                                const QString &entityID) /*= 0*/
  {
    Q_UNUSED (pluginID);
    Q_UNUSED (entityID);
  }

  /*!
   * \brief This is called by the core when some error triggered by this
   *      plugin instance occurs.
   *
   * \param error A description of the error.
   */
  virtual void errorMessage (const QString &error) /*= 0*/
  {
    Q_UNUSED (error);
  }

signals:
  /*!
   * \brief This message can be used to add a new entity in the model.
   *
   * \param type
   * \param parentEntityId
   * \param atts
   */
  void addEntity (const QString &type, const QString &parentEntityId,
                  const QMap<QString, QString> &atts);

  /*!
   * \brief This message can be used to add a new comment in the model.
   *
   * \param type
   * \param parentEntityId
   * \param atts
   * \param force
   */
  void addComment (const QString &comment, const QString &parentEntityId);

  /*!
   * \brief This message allows to add an Entity (and its children,
   * recursively)
   *  through passing an XML content that has the content of this Entity.
   *
   * \param entity_content
   * \param parentId
   * \param force
   */
  void addContent (const QString &entity_content, const QString &parentId,
                   Data::Format format, bool force);

  /*!
   * \brief This message can be used to edit the attributes of an Entity.
   *
   * \param entity
   * \param atts
   * \param force
   */
  void setAttributes (Entity *entity, const QMap<QString, QString> &atts);
  /*!
   * \brief This message can be used to ask the core to remove an Entity.
   *
   * \param entity The pointer to the Entity must be removed.
   * \param force It should be true if the user want to remove that entity even
   *  the internal model will not be consistent.
   */
  void removeEntity (Entity *entity);

  /*!
   * \brief Say the core that the plugin is interested in just the entities of
   *  the type passed as parameter.
   *
   * \param entityTypes a list of strings that the plugin want to listen.
   */
  void setListenFilter (const QStringList &entityTypes);

  /*!
   * \brief Can be used to extend the current messages of Core.
   *
   * This message is send to all plugins without any validation.
   * The content of the message can be any QObject and are useful to extend
   * the default core message.
   *
   * Always, the signature of the method msg implemented by an receiver must
   * be an public slot like:
   *      void msg(QString pluginID, void *obj)
   *
   * The core will call this msg method to each plug-in that implements it when
   * sendBroadcastMethod is called.
   */
  void sendBroadcastMessage (const char *msg, void *obj);

  /*!
   * \brief Each plugin is able to save its specific data by calling this
   *  message.
   *
   * Important: When a plugin send this message, the previous plug-in data is
   *  overwrited.
   *
   * \param data A byte array containing the data that plugin wants to save.
   */
  void setPluginData (const QByteArray &data);

  /*!
   * \brief TODO
   *
   * \todo This could be done with setBroadcastMessage?!
   */
  void setCurrentProjectAsDirty ();

protected:
  //! The Project binded with this particular plugin instance
  Project *project;

  /*!
   * \brief Each plugin instance also receives a unique instance ID (given by
   *  the core).
   */
  QString pluginInstanceID;
  QMutex mutex;
  ILanguageProfile *languageProfile;
};

CPR_CORE_END_NAMESPACE

#endif // IPLUGIN_H
