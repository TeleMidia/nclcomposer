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

#ifndef PLUGINCONTROL_H
#define PLUGINCONTROL_H

#include <QString>
#include <QObject>
#include <QDir>
#include <QPluginLoader>
#include <QObject>
#include <QMultiHash>
#include <QList>
#include <QHashIterator>
#include <QDebug>

#include <util/Utilities.h>
#include "util/ComposerCore_global.h"

#include "MessageControl.h"
#include "LanguageControl.h"
#include "model/Project.h"
#include "util/Singleton.h"
#include "extensions/IDocumentParser.h"
#include "extensions/ILanguageProfile.h"
#include "extensions/IPluginFactory.h"
#include "extensions/IPlugin.h"
using namespace cpr::core;

CPR_CORE_BEGIN_NAMESPACE

/*!
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
  SINGLETON(PluginControl)

public:
  /*!
   * \brief launchNewPlugin
   *
   * \param factory
   * \param project
   */
  void launchNewPlugin(IPluginFactory *factory, Project *project);

  /*!
   * \brief Load all the plugins from the default extensions paths.
   */
  void loadPlugins();

  /*!
   * \brief
   *
   * \param pluginsDirPath
   */
  void loadPlugins(const QString &pluginsDirPath);

  /*!
   * \brief
   *
   * \param fileName
   * \return IPluginFactory *
   */
  IPluginFactory* loadPlugin(const QString &fileName);

  /*!
   * \brief
   * \return QList<IPluginFactory *>
   */
  QList<IPluginFactory*> getLoadedPlugins();

  /*!
   * \brief
   *
   * \param doc
   * \return bool
   */
  bool releasePlugins(Project *doc);

  /*!
   * \brief TODO
   *
   * \param
   * \return
   */
  MessageControl *getMessageControl(Project *project);

  /*!
   * \brief
   *
   * \param parser
   * \param mControl
   */
  void connectParser(IDocumentParser *parser, MessageControl *mControl);

  /*!
   * \brief
   */
  QList <IPlugin*> getPluginInstances(Project *project);
  /* Should be private? */

public slots:
  /*!
   * \brief
   *
   * \param doc
   */
  void launchProject(Project *doc);

  /*!
   * \brief
   *
   * \param location
   */
  void savePluginsData(Project* project);

signals:
  /*!
   * \brief
   *
   * \param documentdId
   * \param location
   */
  void newProjectLaunchedAndCreated(QString documentdId, QString location);

  /*!
   * \brief
   *
   * \param QString
   */
  void notifyError(QString);

  /*!
   * \brief addPluginWidgetToWindow
   *
   * \param pluginFactory
   * \param plugin
   * \param documentId
   */
  void addPluginWidgetToWindow( IPluginFactory *pluginFactory,
                                IPlugin *plugin,
                                QString documentId );
  /*!
   * \brief addPluginWidgetToWindow
   *
   * \param pluginFactory
   * \param plugin
   * \param project
   * \param n
   */
  void addPluginWidgetToWindow( IPluginFactory *pluginFactory,
                                IPlugin *plugin,
                                Project* project );

private:
  /*!
   * \brief Constructor.
   */
  explicit PluginControl();
  /*!
   * \brief Constructor.
   */
  virtual ~PluginControl();

  QHash <QString, IPluginFactory*> pluginFactories;
  /*!< PluginFactory by pluginID */

  QMultiHash <LanguageType, QString> pluginsByType;
  /*!< pluginID given LanguageType */

  /* TC by DocumentID */
  /* Is this right??
   * And when we have plugins that are not related to documents???
   */
  QHash <Project *, MessageControl*> messageControls; /*!< TODO */

  QMultiHash<Project *, IPlugin*> pluginInstances;
  /*!< Plugin Instance given project location */

  QMultiHash<IPlugin*, IPluginFactory*> factoryByPlugin;

  /*!< Maps each IPlugin to its corresponding IPluginFactory */
  /*!
   * \brief Launchs a new plugin and connect it with the given MessageControl.
   *
   * \param plugin the plugin instance that must be connected.
   * \param mControl
   */
  void launchNewPlugin(IPlugin *plugin, MessageControl *mControl);

private slots:
  /*!
   * \brief sendBroadcastMessage
   *
   * \param slot
   * \param payload
   */
  void sendBroadcastMessage(const char *slot, void *payload);
};

CPR_CORE_END_NAMESPACE

#endif // PLUGINCONTROL_H
