#include "modules/PluginControl.h"

#include <QUuid>
#include <QMetaObject>
#include <QMetaMethod>

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
     QMultiHash<QString,IPlugin*>::iterator itInst;
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

    // qDebug() << "loadingPlugin( " << fileName << ")";
    if (plugin)
    {
        pluginFactory = qobject_cast<IPluginFactory*> (plugin);
        if (pluginFactory)
        {
            QString pluginID = pluginFactory->id();
            if (!pluginFactories.contains(pluginID))
            {
                pluginFactories[pluginID] = pluginFactory;
                QList<LanguageType> types =
                        pluginFactory->getSupportedLanguages();

                QList<LanguageType>::iterator it;

                for (it = types.begin() ; it!= types.end(); it++)
                {
                    pluginsByType.insert(*it, pluginFactory->id());
                }
            }
        }
    }//end load OK
    else
    {
       qDebug() << "PluginControl::loadPlugins failed to load"
                << "(" << fileName << ")" << " -- " << loader.errorString();
    }

    return pluginFactory;
}

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
    IPlugin *pluginInstance;
    LanguageType type = project->getProjectType();
//    ILanguageProfile *profile = LanguageControl::getInstance()->
//                                getProfileFromType(type);
    QString location  = project->getLocation();

    msgControl = new MessageControl(project);
    messageControls[location] = msgControl;

    QList<QString> plugIDs = pluginsByType.values(type);
    QList<QString>::iterator it;
    for (it = plugIDs.begin() ; it != plugIDs.end() ;
         it++)
    {
        factory        = pluginFactories[*it];
        pluginInstance = factory->createPluginInstance();
        if (pluginInstance)
        {
            pluginInstance->setPluginInstanceID(
                           factory->id() + "#" +QUuid::createUuid().toString());
            pluginInstance->setProject(project);
//            pluginInstance->setLanguageProfile(profile);
            launchNewPlugin(pluginInstance, msgControl);

            pluginInstances.insert(location, pluginInstance);
            factoryByPlugin.insert(pluginInstance, factory);

            emit addPluginWidgetToWindow(factory, pluginInstance, project,
                                         factoryByPlugin.size());
            //TODO: CREATE A NEW FUNCTION TO UPDATE FROM SAVED CONTENT
            pluginInstance->init();
        }
        else {
            emit notifyError(tr("Could not create an instance for the"
                                "plugin (%1)").arg(*it));
        }
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
            plugin,SLOT(onEntityRemoved(QString, QString))); */

    /* Connect signals from the plugin to slots of the core */
    connect(plugin,
            SIGNAL(addEntity(QString, QString, QMap<QString,QString>&, bool)),
            mControl,
            SLOT(onAddEntity(QString, QString, QMap<QString,QString>&, bool)));

    connect(plugin, SIGNAL(setAttributes(Entity*, QMap<QString,QString>, bool)),
            mControl,
            SLOT(onEditEntity(Entity*, QMap<QString,QString>, bool)));

    connect(plugin,
            SIGNAL(removeEntity(Entity*,bool)),
            mControl,
            SLOT(onRemoveEntity(Entity*,bool)));

    connect(plugin,
            SIGNAL(setListenFilter(QStringList)),
            mControl,
            SLOT(setListenFilter(QStringList)));

    // broadcastMessage
    connect(plugin, SIGNAL(sendBroadcastMessage(const char*, void *)),
            this, SLOT(sendBroadcastMessage(const char*, void*)));
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
        if (!project) return false;

        if (!messageControls.contains(project->getLocation()))
            return false;

        if (!pluginInstances.contains(project->getLocation())) return false;

        MessageControl *t = messageControls.value(project->getLocation());
        if (t)
        {
            delete t;
            t = NULL;
            messageControls.remove(project->getLocation());
        }

        QList<IPlugin*> instances =
                pluginInstances.values(project->getLocation());

        QList<IPlugin*>::iterator it;
        for (it = instances.begin(); it != instances.end(); it++)
        {
           IPlugin *inst = *it;
           inst->saveSubsession();
           IPluginFactory *fac = factoryByPlugin.value(inst);
           factoryByPlugin.remove(inst);
           fac->releasePluginInstance(inst);
        }
        pluginInstances.remove(project->getLocation());

        return true;
}

void PluginControl::sendBroadcastMessage(const char* slot, void *obj)
{
    IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());

    QList<IPlugin*>::iterator it;
    QList<IPlugin*> instances =
            pluginInstances.values(plugin->getProject()->getLocation());

    QString slotName(slot);
    slotName.append("(void*)");
    for (it = instances.begin(); it != instances.end(); it++)
    {
       IPlugin *inst = *it;
       int idxSlot = inst->metaObject()
                                ->indexOfSlot( slotName.toStdString().c_str() );

       if(idxSlot != -1)
       {
           QMetaMethod method = inst->metaObject()->method(idxSlot);
           method.invoke(inst, Qt::QueuedConnection, Q_ARG(void *, obj));
       }
    }
}

void PluginControl::savePluginsData(QString location)
{
    QList<IPlugin*>::iterator it;
    QList<IPlugin*> instances = pluginInstances.values(location);

    for (it = instances.begin(); it != instances.end(); it++)
    {
       IPlugin *inst = *it;
       inst->saveSubsession();
    }
}

MessageControl *PluginControl::getMessageControl(QString location)
{
    return messageControls.value(location);
}

QList <IPlugin*> PluginControl::getPluginInstances(QString location)
{
    QList<IPlugin*> instances;
    if(pluginInstances.contains(location))
        instances = pluginInstances.values(location);

    return instances;
}

} }//end namespace
