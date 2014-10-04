/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "modules/PluginControl.h"

#include <QUuid>
#include <QMetaObject>
#include <QMetaMethod>

#if QT_VERSION >= 0x050000
#include <QJsonObject>
#endif

namespace composer {
namespace core {

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

  IPlugin *inst = NULL;
  IPluginFactory *fac = NULL;

  for (itInst = pluginInstances.begin();
       itInst != pluginInstances.end(); itInst++)
  {
    QList<IPlugin*> instances = pluginInstances.values(itInst.key());
    QList<IPlugin*>::iterator it;

    for (it = instances.begin(); it != instances.end(); it++)
    {
      inst = *it;
      fac = factoryByPlugin.value(inst);
      factoryByPlugin.remove(inst);
      fac->releasePluginInstance(inst);
      pluginInstances.remove(itInst.key(),inst);
    }
  }

  for (itFac = pluginFactories.begin() ; itFac != pluginFactories.end();
       itFac++)
  {
    fac = itFac.value();
    delete fac;
    fac = NULL;
  }

  pluginFactories.clear();
  pluginInstances.clear();
  pluginsByType.clear();
  factoryByPlugin.clear();
  messageControls.clear();
}

IPluginFactory* PluginControl::loadPlugin(QString fileName)
{
  IPluginFactory *pluginFactory = NULL;
  QPluginLoader loader(fileName);
  QObject *plugin = loader.instance();

  if (plugin)
  {
    pluginFactory = qobject_cast<IPluginFactory*> (plugin);
    if (pluginFactory)
    {
      QString pluginID = pluginFactory->id();
      if (!pluginFactories.contains(pluginID))
      {
        pluginFactories[pluginID] = pluginFactory;
        QList<LanguageType> types = pluginFactory->getSupportedLanguages();

        QList<LanguageType>::iterator it;

        for (it = types.begin() ; it!= types.end(); it++)
        {
          pluginsByType.insert(*it, pluginFactory->id());
        }

#if QT_VERSION >= 0x050000
        qDebug() << loader.metaData();
        QJsonObject metadata = loader.metaData().value("MetaData").toObject();
        pluginFactory->setMetadata(metadata);
#endif
      }
    }
  }//end load OK
  else
  {
    qWarning() << "PluginControl::loadPlugins failed to load"
               << "(" << fileName << ")" << " -- " << loader.errorString();
  }

  return pluginFactory;
}

/*bool PluginControl::unLoadPlugin(QString fileName)
{
  // \todo: Verify if there is some instance of the IPlugin
  QPluginLoader loader(fileName);
  return loader.unload();
}*/

void PluginControl::loadPlugins(QString pluginsDirPath)
{
  QDir pluginsDir = QDir(pluginsDirPath);
  pluginsDir.setFilter(QDir::Files | QDir::NoSymLinks);

  if(!pluginsDir.exists())
  {
    emit notifyError(tr("The Plugin Directory (%1) does not exist").
                     arg(pluginsDirPath));
    return;
  }

  QStringList filter;
  filter.append("*.so");
  filter.append("*.dylib");
  filter.append("*.dll");
  pluginsDir.setNameFilters(filter);

  foreach (QString fileName, pluginsDir.entryList(QDir::Files
                                                  | QDir::NoSymLinks))
  {
    loadPlugin(pluginsDir.absoluteFilePath(fileName));
  } //end foreach

} //end function

void PluginControl::launchProject(Project *project)
{
  MessageControl *msgControl;
  IPluginFactory *factory;
  LanguageType type = project->getProjectType();

  msgControl = new MessageControl(project);
  messageControls[project] = msgControl;

  QList<QString>::iterator it;
  QList<QString> plugIDs = pluginsByType.values(type);
  for (it = plugIDs.begin() ; it != plugIDs.end() ;
       it++)
  {
    factory        = pluginFactories[*it];

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
  QString location  = project->getLocation();
  if (pluginInstance)
  {
    pluginInstance->setPluginInstanceID(
          factory->id() + "#" +QUuid::createUuid().toString());
    pluginInstance->setProject(project);
    launchNewPlugin(pluginInstance, messageControls[project]);

    pluginInstances.insert(project, pluginInstance);
    factoryByPlugin.insert(pluginInstance, factory);

    emit addPluginWidgetToWindow(factory, pluginInstance, project,
                                 factoryByPlugin.size());

    //TODO: CREATE A NEW FUNCTION TO UPDATE FROM SAVED CONTENT
    pluginInstance->init();
  }
  else {
    emit notifyError(tr("Could not create an instance for the"
                        "plugin (%1)").arg(factory->id()));
  }

}

void PluginControl::launchNewPlugin(IPlugin *plugin, MessageControl *mControl)
{
  /* Connect signals from the core to slots in the plugins */
  /*
    \deprecated

    Today, the message control is reponsible to call this slots directly.

    connect(mControl,SIGNAL(entityAdded(QString, Entity*)),
            plugin, SLOT(onEntityAdded(QString, Entity*)));
    connect(mControl,SIGNAL(entityChanged(QString, Entity*)),
            plugin,SLOT(onEntityChanged(QString, Entity*)));
    connect(mControl,SIGNAL(entityRemoved(QString, QString)),
            plugin,SLOT(onEntityRemoved(QString, QString)));
    */

  /* Connect signals from the plugin to slots of the core */
  connect(plugin,
          SIGNAL(addEntity(QString, QString, QMap<QString,QString>&, bool)),
          mControl,
          SLOT(onAddEntity(QString, QString, QMap<QString,QString>&, bool)),
          Qt::DirectConnection);

  connect(plugin,
          SIGNAL(setAttributes(Entity*, QMap<QString,QString>, bool)),
          mControl,
          SLOT(onEditEntity(Entity*, QMap<QString,QString>, bool)),
          Qt::DirectConnection);

  connect(plugin,
          SIGNAL(removeEntity(Entity*,bool)),
          mControl,
          SLOT(onRemoveEntity(Entity*,bool)),
          Qt::DirectConnection);

  connect(plugin,
          SIGNAL(setListenFilter(QStringList)),
          mControl,
          SLOT(setListenFilter(QStringList)),
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
  connect(plugin, SIGNAL(setPluginData(QByteArray)),
          mControl, SLOT(setPluginData(QByteArray)),
          Qt::DirectConnection);
}

void PluginControl::connectParser(IDocumentParser *parser,
                                  MessageControl *mControl)
{
  connect(parser,
          SIGNAL(addEntity(QString, QString, QMap<QString,QString>&, bool)),
          mControl,
          SLOT(onAddEntity(QString, QString, QMap<QString,QString>&, bool)));

  connect(mControl,SIGNAL(entityAdded(QString, Entity*)),
          parser, SLOT(onEntityAdded(QString, Entity*)));
}

QList<IPluginFactory*> PluginControl::getLoadedPlugins()
{
  QHash<QString,IPluginFactory*>::iterator it;
  QList<IPluginFactory*> pList;
  for (it = pluginFactories.begin() ; it != pluginFactories.end(); it++)
  {
    pList.append(it.value());
  }
  return pList;
}

bool PluginControl::releasePlugins(Project *project)
{
  if (!project) {
    qDebug() << "Project is NULL";
    return false;
  }

  if (!messageControls.contains(project))
  {
    qDebug() << "Message Control does not know the project";
    return false;
  }

  //        if (!pluginInstances.contains(project->getLocation()))
  //        {
  //          qDebug() << "There is no plugin instance for this project";
  //          return false;
  //        }

  MessageControl *t = messageControls.value(project);
  if (t)
  {
    delete t;
    t = NULL;
    messageControls.remove(project);
  }

  QList<IPlugin*> instances = pluginInstances.values(project);

  QList<IPlugin*>::iterator it;
  for (it = instances.begin(); it != instances.end(); it++)
  {
    IPlugin *inst = *it;
    inst->saveSubsession();
    IPluginFactory *fac = factoryByPlugin.value(inst);
    factoryByPlugin.remove(inst);
    fac->releasePluginInstance(inst);
  }
  pluginInstances.remove(project);

  return true;
}

void PluginControl::sendBroadcastMessage(const char* slot, void *obj)
{
  IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());

  QList<IPlugin*>::iterator it;
  QList<IPlugin*> instances = pluginInstances.values(plugin->getProject());

  QString slotName(slot);
  slotName.append("(QString,void*)");
  for (it = instances.begin(); it != instances.end(); it++)
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
  QList<IPlugin*> instances = pluginInstances.values(project);

  for (it = instances.begin(); it != instances.end(); it++)
  {
    IPlugin *inst = *it;
    inst->saveSubsession();
  }
}

MessageControl *PluginControl::getMessageControl(Project *project)
{
  return messageControls.value(project);
}

QList <IPlugin*> PluginControl::getPluginInstances(Project *project)
{
  QList<IPlugin*> instances;
  if(pluginInstances.contains(project))
    instances = pluginInstances.values(project);

  return instances;
}

} }//end namespace
