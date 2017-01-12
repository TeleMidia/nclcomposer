/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
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
#include "../util/ComposerCoreControl_global.h"

#include "MessageControl.h"
#include "LanguageControl.h"
using namespace composer::core;

#include "../model/Project.h"
using namespace composer::core::model;

#include "../util/Singleton.h"
using namespace composer::core::util;

#include "../extensions/IDocumentParser.h"
#include "../extensions/ILanguageProfile.h"
#include "../extensions/IPluginFactory.h"
#include "../extensions/IPlugin.h"
using namespace composer::extension;

namespace composer {
  namespace core {

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

} } //end namespace

#endif // PLUGINCONTROL_H
