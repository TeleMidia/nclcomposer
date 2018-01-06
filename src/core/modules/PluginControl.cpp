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

#include "modules/PluginControl.h"

#include <QApplication>
#include <QMetaMethod>
#include <QMetaObject>
#include <QUuid>

#include <QJsonObject>

CPR_CORE_BEGIN_NAMESPACE

INIT_SINGLETON (PluginControl)

PluginControl::PluginControl ()
{
  // Register MetaType allowing to be used by invoke
  qRegisterMetaType<void *> ("const void *");
}

PluginControl::~PluginControl ()
{
  // \fixme bug when closing on windows
  for (Project *prj : _pluginInstances.keys () )
  {
    for (IPlugin *inst : _pluginInstances.values (prj))
    {
      IPluginFactory *fac = _factoryByPlugin.value (inst);
      _factoryByPlugin.remove (inst);
      fac->releasePluginInstance (inst);
      _pluginInstances.remove (prj, inst);
    }
  }

  for (IPluginFactory *fac : _pluginFactories.values())
  {
    delete fac;
  }

  _pluginFactories.clear ();
  _pluginInstances.clear ();
  _pluginsByType.clear ();
  _factoryByPlugin.clear ();
  _messageControls.clear ();
}

IPluginFactory *
PluginControl::loadPlugin (const QString &fileName)
{
  qCDebug (CPR_CORE) << "Trying to load plugin " << fileName;
  IPluginFactory *pluginFactory = nullptr;
  QPluginLoader loader (fileName);
  QObject *plugin = loader.instance ();

  if (plugin)
  {
    pluginFactory = qobject_cast<IPluginFactory *> (plugin);
    if (pluginFactory)
    {
      qCDebug (CPR_CORE) << fileName << " loaded --- "
                         << "pluginFactory = " << pluginFactory;

      QString pluginID = pluginFactory->id ();
      if (!_pluginFactories.contains (pluginID))
      {
        _pluginFactories[pluginID] = pluginFactory;
        QList<LanguageType> types = pluginFactory->getSupportedLanguages ();

        for (auto it = types.begin (); it != types.end (); ++it)
        {
          _pluginsByType.insert (*it, pluginFactory->id ());
        }

        qCDebug (CPR_CORE) << loader.metaData ();
        QJsonObject metadata
            = loader.metaData ().value ("MetaData").toObject ();
        metadata["path"] = fileName;
        pluginFactory->setMetadata (metadata);
      }
    }
  } // end load OK
  else
  {
    qCWarning (CPR_CORE) << "PluginControl::loadPlugins failed to load"
                         << "(" << fileName << ")"
                         << " -- " << loader.errorString ();
  }

  return pluginFactory;
}

/*bool PluginControl::unLoadPlugin(QString fileName)
{
  // \todo: Verify if there is some instance of the IPlugin
  QPluginLoader loader(fileName);
  return loader.unload();
}*/

void
PluginControl::loadPlugins ()
{
  GlobalSettings stngs;
  QStringList extPaths = stngs.getExtensionsPaths ();
  // add all the paths to LibraryPath, i.e., plugins are allowed to install
  // dll dependencies in the extensions path.
  for (const QString &extDir : extPaths)
    QApplication::addLibraryPath (extDir + "/");

  // foreach path where extensions can be installed, try to load plugins.
  for (const QString &extDir : extPaths)
    PluginControl::instance ()->loadPlugins (extDir);
}

void
PluginControl::loadPlugins (const QString &pluginsDirPath)
{
  qCDebug (CPR_CORE) << "Trying to load plugins from " << pluginsDirPath;
  QDir pluginsDir = QDir (pluginsDirPath);
  pluginsDir.setFilter (QDir::Files | QDir::NoSymLinks);

  if (!pluginsDir.exists ())
  {
    qCDebug (CPR_CORE) << pluginsDirPath << " directory does not exist.";
    return;
  }

  QStringList filter;
  filter.append ("*.so");
  filter.append ("*.dylib");
  filter.append ("*.dll");
  pluginsDir.setNameFilters (filter);

  for (const QString &fileName :
       pluginsDir.entryList (QDir::Files | QDir::NoSymLinks))
  {
    loadPlugin (pluginsDir.absoluteFilePath (fileName));
  }

} // end function

void
PluginControl::launchProject (Project *project)
{
  IPluginFactory *factory;
  LanguageType type = project->projectType ();

  MessageControl *msgControl = new MessageControl (project);
  _messageControls[project] = msgControl;

  for (const QString &plgId : _pluginsByType.values (type))
  {
    factory = _pluginFactories[plgId];

    GlobalSettings stngs;
    stngs.beginGroup ("loadPlugins");
    if (!stngs.contains (plgId) || stngs.value (plgId).toBool () == true)
    {
      launchNewPlugin (factory, project);
    }
    stngs.endGroup ();
  }
}

void
PluginControl::launchNewPlugin (IPluginFactory *factory, Project *project)
{
  IPlugin *plgInst = factory->createPluginInstance ();
  if (plgInst)
  {
    plgInst->setPluginInstanceID (factory->id () + "#"
                                  + QUuid::createUuid ().toString ());
    plgInst->setProject (project);
    launchNewPlugin (plgInst, _messageControls[project]);

    _pluginInstances.insert (project, plgInst);
    _factoryByPlugin.insert (plgInst, factory);

    emit addPluginWidgetToWindow (factory, plgInst, project);

    // TODO: CREATE A NEW FUNCTION TO UPDATE FROM SAVED CONTENT
    plgInst->init ();
  }
  else
  {
    qCWarning (CPR_CORE) << "Could not create an instance for the plugin "
                         << factory->id ();
  }
}

void
PluginControl::launchNewPlugin (IPlugin *plugin, MessageControl *mControl)
{
  /* Connect signals from the plugin to slots of the core */
  connect (plugin, &IPlugin::addEntity, mControl, &MessageControl::addEntity,
           Qt::DirectConnection);

  connect (plugin, &IPlugin::addContent, mControl, &MessageControl::addContent,
           Qt::DirectConnection);

  connect (plugin, &IPlugin::addComment, mControl, &MessageControl::addComment,
           Qt::DirectConnection);

  connect (plugin, &IPlugin::setAttributes, mControl,
           &MessageControl::editEntity, Qt::DirectConnection);

  connect (plugin, &IPlugin::removeEntity, mControl,
           &MessageControl::removeEntity, Qt::DirectConnection);

  connect (plugin, &IPlugin::setListenFilter, mControl,
           &MessageControl::setListenFilter, Qt::DirectConnection);

  connect (plugin, &IPlugin::setCurrentProjectAsDirty, mControl,
           &MessageControl::setCurrentProjectAsDirty, Qt::DirectConnection);

  // broadcastMessage
  connect (plugin, &IPlugin::sendBroadcastMessage, this,
           &PluginControl::sendBroadcastMessage, Qt::DirectConnection);

  /* setPluginData */
  connect (plugin, &IPlugin::setPluginData, mControl,
           &MessageControl::setPluginData, Qt::DirectConnection);
}

void
PluginControl::connectParser (IDocumentParser *parser,
                              MessageControl *mControl)
{
  connect (parser, &IDocumentParser::addEntity, mControl,
           &MessageControl::addEntity);
}

QList<IPluginFactory *>
PluginControl::loadedPlugins ()
{
  return _pluginFactories.values ();
}

bool
PluginControl::releasePlugins (Project *project)
{
  if (!project)
  {
    qCDebug (CPR_CORE) << "Project is null.";
    return false;
  }

  if (!_messageControls.contains (project))
  {
    qCDebug (CPR_CORE) << "Message Control does not know the project.";
    return false;
  }

  MessageControl *t = _messageControls.value (project);
  if (t)
  {
    delete t;
    _messageControls.remove (project);
  }

  for (IPlugin *inst : _pluginInstances.values (project))
  {
    inst->saveSubsession ();
    IPluginFactory *fac = _factoryByPlugin.value (inst);
    _factoryByPlugin.remove (inst);
    fac->releasePluginInstance (inst);
  }
  _pluginInstances.remove (project);

  return true;
}

void
PluginControl::sendBroadcastMessage (const char *slot, void *obj)
{
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender ());

  QString slotName (slot);
  slotName.append ("(QString,void*)");
  for (IPlugin *inst : _pluginInstances.values (plugin->project ()))
  {
    int idxSlot
        = inst->metaObject ()->indexOfSlot (slotName.toStdString ().c_str ());
    if (idxSlot != -1)
    {
      QMetaMethod method = inst->metaObject ()->method (idxSlot);
      method.invoke (inst, Qt::DirectConnection,
                     Q_ARG (QString, plugin->pluginInstanceID ()),
                     Q_ARG (void *, obj));
    }
  }
}

void
PluginControl::savePluginsData (Project *project)
{
  for (IPlugin *inst : _pluginInstances.values (project))
  {
    inst->saveSubsession ();
  }
}

MessageControl *
PluginControl::messageControl (Project *project)
{
  return _messageControls.value (project);
}

QList<IPlugin *>
PluginControl::pluginInstances (Project *project)
{
  CPR_ASSERT (_pluginInstances.contains (project));
  return _pluginInstances.values (project);
}

CPR_CORE_END_NAMESPACE
