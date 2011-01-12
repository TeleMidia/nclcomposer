#include "../../include/modules/PluginControl.h"


namespace composer {
namespace core {
namespace module {

    PluginControl::PluginControl() {

    }

    PluginControl::~PluginControl() {
         QHashIterator<QString,IPluginFactory*> itFac(pluginFactories);

         IPluginFactory *atualFactory = NULL;
         IPlugin *atualInstance = NULL;
         while(itFac.hasNext()){
             atualFactory = itFac.value();
             QList<IPlugin*> instances = pluginInstances.values
                                                        (itFac.key());
             QListIterator<IPlugin*> itInst(instances);
             while(itInst.hasNext()) {
                 atualInstance = itInst.next();
                 atualFactory->releasePluginInstance(atualInstance);
             }
             itFac.next();
             delete atualFactory;
         }
         pluginFactories.clear();
         pluginInstances.clear();
    }

    void PluginControl::loadPlugins(QString pluginsDirPath) {
        IPluginFactory *pluginFactory = NULL;

        QDir pluginsDir = QDir(pluginsDirPath);
        
        foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
                 QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
                 QObject *plugin = loader.instance();
                 if (plugin) {
                     pluginFactory = qobject_cast<IPluginFactory*> (plugin);
                     if (pluginFactory) {
                         QString pluginID = pluginFactory->getPluginID();
                         if (pluginFactories.contains(pluginID))
                             qDebug() << "PluginControl::loadPlugins" <<
                                     "Plugin with ID (" << pluginID <<
                                     ") already exists";
                         pluginFactories.insert(pluginID,pluginFactory);
                     } else {
                         //TODO - erro loading plugin
                     }
                 }
        }
    }

    void PluginControl::onNewDocument(NclDocument *nclDoc) {
        QHashIterator<QString,IPluginFactory*> it(pluginFactories);
        IPluginFactory *pluginBuilder;
        IPlugin *pluginInstance;
        while (it.hasNext()) {
            pluginBuilder  = it.value();
            pluginInstance = pluginBuilder->createPluginInstance();
            if (pluginInstance) {
                launchNewPlugin(pluginInstance);
                pluginInstance->setNclDocument(nclDoc);
                pluginInstances.insert(it.key(),pluginInstance);
                //TODO - Pegar o widget e colocar no QDockWidget
            } else {
                //TODO -- erro creating instance
            }
        }
    }

    void PluginControl::launchNewPlugin(IPlugin *plugin) {
        MessageControl *messageControl = MessageControl::getInstance();

        for (int i = 0; i < TOTALENTITIES; i++) {
            if (plugin->listenFilter(EntityType(i))) {
                switch(EntityType(i)) {
                    case REGION:
                        connectRegion(plugin);
                    break;
                    case REGIONBASE:
                        connectRegionBase(plugin);
                    break;
                }

                /* Connect signals from the plugin to slots of the core */
                connect(plugin,SIGNAL(addEntity(EntityType,string,
                                     map<string,string>&,bool)),
                        messageControl,SLOT(onAddEntity(EntityType,string,
                                     map<string,string>&,bool)));
                connect(plugin,SIGNAL(editEntity(EntityType,Entity*,
                                     map<string,string>&,bool)),
                        messageControl,SLOT(onEditEntity(EntityType,Entity*,
                                     map<string,string>&,bool)));
                connect(plugin,SIGNAL(removeEntity(Entity*,bool)),
                        messageControl,SLOT(onRemoveEntity(Entity*,bool)));

            } //endif
        }//endfor
    }

    void PluginControl::connectRegion(IPlugin *plugin) {
        MessageControl *messageControl = MessageControl::getInstance();

        connect(messageControl,SIGNAL(regionAdded(Region*)),plugin,
                SLOT(onEntityAdded(Entity*))); 
        connect(messageControl,SIGNAL(regionChanged(Region*)), plugin,
                SLOT(onEntityChanged(Entity*))); 
        connect(messageControl,SIGNAL(regionRemoved(string)),plugin,
                SLOT(onEntityRemoved(string)));
        connect(messageControl,SIGNAL(aboutToRemoveRegion(Region*)),plugin,
                SLOT(onEntityAboutToRemove(Entity*)),Qt::BlockingQueuedConnection);
    }

    void PluginControl::connectRegionBase(IPlugin *plugin) {
        MessageControl *messageControl = MessageControl::getInstance();

        connect(messageControl,SIGNAL(regionBaseAdded(Region*)), plugin,
                SLOT(onEntityAdded(Entity*)));
        connect(messageControl,SIGNAL(regionBaseChanged(Region*)), plugin,
                SLOT(onEntityChanged(Entity*)));
        connect(messageControl,SIGNAL(regionBaseRemoved(string)), plugin,
                SLOT(onEntityRemoved(string)));
        connect(messageControl,SIGNAL(aboutToRemoveRegionBase(Region*)),plugin,
                SLOT(onEntityAboutToRemove(Entity*)),Qt::BlockingQueuedConnection);
    }

}
}
}//fim namespace composer
