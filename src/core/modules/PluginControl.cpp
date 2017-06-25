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

#include "modules/PluginControl.h"

#include <QUuid>
#include <QMetaObject>
#include <QMetaMethod>
#include <QApplication>

#include <QJsonObject>

CPR_CORE_BEGIN_NAMESPACE

INIT_SINGLETON(PluginControl)

PluginControl::PluginControl()
{
  //Register MetaType allowing to be used by invoke
  qRegisterMetaType<void *>("const void *");
}

PluginControl::~PluginControl()
{
  //FIXME: BUG WHEN CLOSING ON WINDOWS
  QMultiHash<Project*,IPlugin*>::iterator itInst;
  QHash<QString,IPluginFactory*>::iterator itFac;

  IPlugin *inst = nullptr;
  IPluginFactory *fac = nullptr;

  for (itInst = _pluginInstances.begin();
       itInst != _pluginInstances.end(); ++itInst)
  {
    QList<IPlugin*> instances = _pluginInstances.values(itInst.key());
    QList<IPlugin*>::iterator it;

    for (it = instances.begin(); it != instances.end(); ++it)
    {
      inst = *it;
      fac = _factoryByPlugin.value(inst);
      _factoryByPlugin.remove(inst);
      fac->releasePluginInstance(inst);
      _pluginInstances.remove(itInst.key(),inst);
    }
  }

  for (itFac = _pluginFactories.begin() ; itFac != _pluginFactories.end();
       ++itFac)
  {
    fac = itFac.value();
    delete fac;
    fac = nullptr;
  }

  _pluginFactories.clear();
  _pluginInstances.clear();
  _pluginsByType.clear();
  _factoryByPlugin.clear();
  _messageControls.clear();
}

IPluginFactory* PluginControl::loadPlugin(const QString &fileName)
{
  qCDebug(CPR_CORE) << "Trying to load plugin " << fileName;
  IPluginFactory *pluginFactory = nullptr;
  QPluginLoader loader(fileName);
  QObject *plugin = loader.instance();

  if (plugin)
  {
    pluginFactory = qobject_cast<IPluginFactory*> (plugin);
    if (pluginFactory)
    {
      qCDebug(CPR_CORE) << fileName << " loaded --- "
                          << "pluginFactory = " << pluginFactory;

      QString pluginID = pluginFactory->id();
      if (!_pluginFactories.contains(pluginID))
      {
        _pluginFactories[pluginID] = pluginFactory;
        QList<LanguageType> types = pluginFactory->getSupportedLanguages();

        QList<LanguageType>::iterator it;

        for (it = types.begin() ; it!= types.end(); ++it)
        {
          _pluginsByType.insert(*it, pluginFactory->id());
        }

        qCDebug(CPR_CORE) << loader.metaData();
        QJsonObject metadata = loader.metaData().value("MetaData").toObject();
        metadata["path"] = fileName;
        pluginFactory->setMetadata(metadata);
      }
    }
  }//end load OK
  else
  {
    qCWarning(CPR_CORE) << "PluginControl::loadPlugins failed to load"
                          << "(" << fileName << ")" << " -- "
                          << loader.errorString();
  }

  return pluginFactory;
}

/*bool PluginControl::unLoadPlugin(QString fileName)
{
  // \todo: Verify if there is some instance of the IPlugin
  QPluginLoader loader(fileName);
  return loader.unload();
}*/

void PluginControl::loadPlugins()
{
  GlobalSettings settings;
  QStringList extPaths = settings.getExtensionsPaths();
  // add all the paths to LibraryPath, i.e., plugins are allowed to install
  // dll dependencies in the extensions path.
  for(const QString &extDir: extPaths)
    QApplication::addLibraryPath(extDir + "/");

  // foreach path where extensions can be installed, try to load plugins.
  for(const QString &extDir: extPaths)
    PluginControl::getInstance()->loadPlugins(extDir);
}

void PluginControl::loadPlugins(const QString &pluginsDirPath)
{
  qCDebug(CPR_CORE) << "Trying to load plugins from " << pluginsDirPath;
  QDir pluginsDir = QDir(pluginsDirPath);
  pluginsDir.setFilter(QDir::Files | QDir::NoSymLinks);

  if(!pluginsDir.exists())
  {
    qCDebug(CPR_CORE) << pluginsDirPath << " directory does not exist.";
    return;
  }

  QStringList filter;
  filter.append("*.so");
  filter.append("*.dylib");
  filter.append("*.dll");
  pluginsDir.setNameFilters(filter);

  for (const QString& fileName: pluginsDir.entryList(QDir::Files
                                                  | QDir::NoSymLinks))
  {
    loadPlugin(pluginsDir.absoluteFilePath(fileName));
  }

} //end function

void PluginControl::launchProject(Project *project)
{
  MessageControl *msgControl;
  IPluginFactory *factory;
  LanguageType type = project->getProjectType();

  msgControl = new MessageControl(project);
  _messageControls[project] = msgControl;

  QList<QString>::iterator it;
  QList<QString> plugIDs = _pluginsByType.values(type);
  for (it = plugIDs.begin(); it != plugIDs.end(); ++it)
  {
    factory        = _pluginFactories[*it];

    GlobalSettings settings;
    settings.beginGroup("loadPlugins");
    if(!settings.contains(*it) || settings.value(*it).toBool() == true)
    {
      launchNewPlugin(factory, project);
    }
    settings.endGroup();
  }
}

void PluginControl::launchNewPlugin(IPluginFactory *factory, Project *project)
{
  IPlugin *pluginInstance = factory->createPluginInstance();
  if (pluginInstance)
  {
    pluginInstance->setPluginInstanceID(
          factory->id() + "#" + QUuid::createUuid().toString());
    pluginInstance->setProject(project);
    launchNewPlugin(pluginInstance, _messageControls[project]);

    _pluginInstances.insert(project, pluginInstance);
    _factoryByPlugin.insert(pluginInstance, factory);

    emit addPluginWidgetToWindow( factory,
                                  pluginInstance,
                                  project);

    //TODO: CREATE A NEW FUNCTION TO UPDATE FROM SAVED CONTENT
    pluginInstance->init();
  }
  else
  {
    qCWarning(CPR_CORE) << "Could not create an instance for the plugin "
                      << factory->id();
  }

}

void PluginControl::launchNewPlugin(IPlugin *plugin, MessageControl *mControl)
{
  /* Connect signals from the plugin to slots of the core */
  connect(plugin,
          SIGNAL(addEntity(const QString&, const QString&, const QMap<QString,QString>&)),
          mControl,
          SLOT(onAddEntity(const QString&, const QString&, const QMap<QString,QString>&)),
          Qt::DirectConnection);

  connect(plugin,
          SIGNAL(addComment(const QString&, const QString&)),
          mControl,
          SLOT(onAddComment(const QString&, const QString&)),
          Qt::DirectConnection);

  connect(plugin,
          SIGNAL(addEntity(const QString&, const QString&, Data::Format)),
          mControl,
          SLOT(onAddEntity(const QString&, const QString&, Data::Format)),
          Qt::DirectConnection);

  connect(plugin,
          SIGNAL(setAttributes(Entity*, const QMap<QString,QString>&)),
          mControl,
          SLOT(onEditEntity(Entity*, const QMap<QString,QString>&)),
          Qt::DirectConnection);

  connect(plugin,
          SIGNAL(removeEntity(Entity*)),
          mControl,
          SLOT(onRemoveEntity(Entity*)),
          Qt::DirectConnection);

  connect(plugin,
          SIGNAL(setListenFilter(const QStringList&)),
          mControl,
          SLOT(setListenFilter(const QStringList&)),
          Qt::DirectConnection);

  connect(plugin,
          SIGNAL(setCurrentProjectAsDirty()),
          mControl,
          SLOT(setCurrentProjectAsDirty()),
          Qt::DirectConnection);

  // broadcastMessage
  connect(plugin, SIGNAL(sendBroadcastMessage(const char*, void *)),
          this, SLOT(sendBroadcastMessage(const char*, void*)),
          Qt::DirectConnection);

  /* setPluginData */
  connect(plugin, SIGNAL(setPluginData(const QByteArray&)),
          mControl, SLOT(setPluginData(const QByteArray&)),
          Qt::DirectConnection);
}

void PluginControl::connectParser(IDocumentParser *parser,
                                  MessageControl *mControl)
{
  connect(parser,
          SIGNAL(addEntity(QString, QString, QMap<QString,QString>&)),
          mControl,
          SLOT(onAddEntity(QString, QString, QMap<QString,QString>&)));

  connect(mControl,SIGNAL(entityAdded(const QString&, Entity*)),
          parser, SLOT(onEntityAdded(const QString&, Entity*)));
}

QList<IPluginFactory*> PluginControl::getLoadedPlugins()
{
  QHash<QString,IPluginFactory*>::iterator it;
  QList<IPluginFactory*> pList;
  for (it = _pluginFactories.begin() ; it != _pluginFactories.end(); ++it)
  {
    pList.append(it.value());
  }
  return pList;
}

bool PluginControl::releasePlugins(Project *project)
{
  if (!project)
  {
    qCDebug(CPR_CORE) << "Project is null.";
    return false;
  }

  if (!_messageControls.contains(project))
  {
    qCDebug(CPR_CORE) << "Message Control does not know the project.";
    return false;
  }

  MessageControl *t = _messageControls.value(project);
  if (t)
  {
    delete t;
    t = nullptr;
    _messageControls.remove(project);
  }

  QList<IPlugin*> instances = _pluginInstances.values(project);
  QList<IPlugin*>::iterator it;
  for (it = instances.begin(); it != instances.end(); ++it)
  {
    IPlugin *inst = *it;
    inst->saveSubsession();
    IPluginFactory *fac = _factoryByPlugin.value(inst);
    _factoryByPlugin.remove(inst);
    fac->releasePluginInstance(inst);
  }
  _pluginInstances.remove(project);

  return true;
}

void PluginControl::sendBroadcastMessage(const char* slot, void *obj)
{
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());

  QList<IPlugin*>::iterator it;
  QList<IPlugin*> instances = _pluginInstances.values(plugin->getProject());

  QString slotName(slot);
  slotName.append("(QString,void*)");
  for (it = instances.begin(); it != instances.end(); ++it)
  {
    IPlugin *inst = *it;
    int idxSlot = inst->metaObject()
        ->indexOfSlot( slotName.toStdString().c_str() );
    if(idxSlot != -1)
    {
      QMetaMethod method = inst->metaObject()->method(idxSlot);
      method.invoke(inst, Qt::DirectConnection,
                    Q_ARG(QString, plugin->getPluginInstanceID()),
                    Q_ARG(void *, obj));
    }
  }
}

void PluginControl::savePluginsData(Project *project)
{
  QList<IPlugin*>::iterator it;
  QList<IPlugin*> instances = _pluginInstances.values(project);

  for (it = instances.begin(); it != instances.end(); ++it)
  {
    IPlugin *inst = *it;
    inst->saveSubsession();
  }
}

MessageControl *PluginControl::getMessageControl(Project *project)
{
  return _messageControls.value(project);
}

QList <IPlugin*> PluginControl::getPluginInstances(Project *project)
{
  QList<IPlugin*> instances;
  if(_pluginInstances.contains(project))
    instances = _pluginInstances.values(project);

  return instances;
}

CPR_CORE_END_NAMESPACE
