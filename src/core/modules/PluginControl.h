/* Copyright (C) 2011-2018 PUC-Rio/TeleMídia Lab.

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

#ifndef PLUGINCONTROL_H
#define PLUGINCONTROL_H

#include <QDebug>
#include <QDir>
#include <QHashIterator>
#include <QList>
#include <QMultiHash>
#include <QObject>
#include <QObject>
#include <QPluginLoader>
#include <QString>

#include "util/ComposerCore_global.h"
#include <util/Utilities.h>

#include "LanguageControl.h"
#include "MessageControl.h"
#include "extensions/IDocumentParser.h"
#include "extensions/ILanguageProfile.h"
#include "extensions/IPlugin.h"
#include "extensions/IPluginFactory.h"
#include "model/Project.h"
#include "util/Singleton.h"
using namespace cpr::core;

CPR_CORE_BEGIN_NAMESPACE

/*!
 * \ingroup core
 * \brief Manages the NCL Composer plugins.
 *
 * It is part of the PluginControl responsability:
 *  - Find the installed plugins in the system.
 *  - Load the installed plugins.
 *  - And connect each plugin with its respective message controllers.
 */
class COMPOSERCORESHARED_EXPORT PluginControl : public QObject
{
  Q_OBJECT
  SINGLETON (PluginControl)

public:
  /*!
   * \brief launchNewPlugin
   *
   * \param factory
   * \param project
   */
  void launchNewPlugin (IPluginFactory *factory, Project *project);

  /*!
   * \brief Load all the plugins from the default extensions paths.
   */
  void loadPlugins ();

  /*!
   * \brief
   *
   * \param pluginsDirPath
   */
  void loadPlugins (const QString &pluginsDirPath);

  /*!
   * \brief
   *
   * \param fileName
   * \return IPluginFactory *
   */
  IPluginFactory *loadPlugin (const QString &fileName);

  /*!
   * \brief
   * \return QList<IPluginFactory *>
   */
  QList<IPluginFactory *> loadedPlugins ();

  /*!
   * \brief
   *
   * \param doc
   * \return bool
   */
  bool releasePlugins (Project *doc);

  /*!
   * \brief TODO
   *
   * \param
   * \return
   */
  MessageControl *messageControl (Project *project);

  /*!
   * \brief
   *
   * \param parser
   * \param mControl
   */
  void connectParser (IDocumentParser *parser, MessageControl *mControl);

  /*!
   * \brief
   */
  QList<IPlugin *> pluginInstances (Project *project);
  /* Should be private? */

public slots:
  /*!
   * \brief
   *
   * \param doc
   */
  void launchProject (Project *doc);

  /*!
   * \brief
   *
   * \param location
   */
  void savePluginsData (Project *project);

signals:
  /*!
   * \brief
   *
   * \param documentdId
   * \param location
   */
  void newProjectLaunchedAndCreated (QString docId, QString location);

  /*!
   * \brief
   *
   * \param QString
   */
  void notifyError (QString);

  /*!
   * \brief addPluginWidgetToWindow
   *
   * \param pluginFactory
   * \param plugin
   * \param documentId
   */
  void addPluginWidgetToWindow (IPluginFactory *pluginFactory, IPlugin *plugin,
                                QString documentId);
  /*!
   * \brief addPluginWidgetToWindow
   *
   * \param pluginFactory
   * \param plugin
   * \param project
   * \param n
   */
  void addPluginWidgetToWindow (IPluginFactory *pluginFactory, IPlugin *plugin,
                                Project *project);

private:
  /*!
   * \brief Constructor.
   */
  explicit PluginControl ();
  /*!
   * \brief Constructor.
   */
  virtual ~PluginControl ();

  QHash<QString, IPluginFactory *> _pluginFactories;
  QMultiHash<LanguageType, QString> _pluginsByType;
  QHash<Project *, MessageControl *> _messageControls;
  QMultiHash<Project *, IPlugin *> _pluginInstances;
  QMultiHash<IPlugin *, IPluginFactory *> _factoryByPlugin;

  /*!< Maps each IPlugin to its corresponding IPluginFactory */
  /*!
   * \brief Launchs a new plugin and connect it with the given MessageControl.
   *
   * \param plugin the plugin instance that must be connected.
   * \param mControl
   */
  void launchNewPlugin (IPlugin *plugin, MessageControl *mControl);

private slots:
  /*!
   * \brief sendBroadcastMessage
   *
   * \param slot
   * \param payload
   */
  void sendBroadcastMessage (const char *slot, void *payload);
};

CPR_CORE_END_NAMESPACE

#endif // PLUGINCONTROL_H
