#include "../../include/modules/PluginControl.h"


namespace composer {
namespace core {
namespace module {

    PluginControl::PluginControl() {
        qDebug() << "PluginControl::PluginControl()";
        IPluginFactory *parserFac = new DocumentParserFactory();
        pluginFactories.insert(parserFac->getPluginID(), parserFac);
        //TODO - loadPlugins :D
        //loadPlugins();
    }

    PluginControl::~PluginControl() {
         QHashIterator<QString,IPluginFactory*> itFac(pluginFactories);

         IPluginFactory *atualFactory = NULL;
         IPlugin *atualInstance = NULL;
         while(itFac.hasNext()){
             itFac.next();
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

    void PluginControl::onNewDocument(QString documentId,
                                      QString location) {

        qDebug() << "PluginControl::onNewDocument(" << documentId
                 << ", " << location << ")";

        QHashIterator<QString,IPluginFactory*> it(pluginFactories);
        IPluginFactory *pluginBuilder;
        IPlugin *pluginInstance;
        TransactionControl *transControl;
        map<string,string> atts;

        if (transactionControls.contains(documentId)) {
            emit notifyError(tr("Transaction Control could not be created"
                                "for NCLDocument(%1)"));
            return;
        }

        atts["id"] = documentId.toStdString();

        /* create the NCLDocument */
        NclDocument *nclDoc = Document::getInstance()->createNclDocument(
                                               atts, location.toStdString());

        transControl = new TransactionControl(nclDoc);
        transactionControls[documentId] = transControl;

        connect(this,SIGNAL(addNcl(QString,Entity*)),
                transControl,SIGNAL(nclAdded(QString,Entity*)));

        while (it.hasNext()) {
            it.next();
            pluginBuilder  = it.value();
            pluginInstance = pluginBuilder->createPluginInstance();
            pluginInstance->setPluginID(pluginBuilder->getPluginID());
            if (pluginInstance) {
                launchNewPlugin(pluginInstance, transControl);
                pluginInstance->setNclDocument(nclDoc);
                pluginInstances.insert(it.key(),pluginInstance);
                //TODO - Pegar o widget e colocar no QDockWidget
                //Se for nulo nao eh plugin visual
            } else {
                qDebug() << "Could not create a instance for the plugin"
                         << "(" << pluginBuilder->getPluginID() << ")";
                //TODO -- erro creating instance
            }
        }
        emit addNcl("composer.core.util.DocumentParser",nclDoc);
        emit newDocumentLaunchedAndCreated(documentId,location);
    }

    void PluginControl::launchNewPlugin(IPlugin *plugin,
                                        TransactionControl *tControl) {
        qDebug() << "PluginControl::launchNewPlugin(" << plugin->getPluginID()
                 << ")";

        for (int i = 0; i < TOTALENTITIES; i++) {
            if (plugin->listenFilter(EntityType(i))) {
                switch(EntityType(i)) {
                    case NCL:
                        connect(tControl,SIGNAL(nclAdded(QString,Entity*)),
                                plugin,SLOT(onEntityAdded(QString,Entity*)));
                        qDebug() << "PluginControl::launchNewPlugin(NCL)";
                    break;
                    case REGION:
                        connectRegion(plugin, tControl);
                    break;
                    case REGIONBASE:
                        connectRegionBase(plugin, tControl);
                    break;
                }

                /* Connect signals from the plugin to slots of the core */
                connect(plugin,SIGNAL(addEntity(EntityType,string,
                                     map<string,string>&,bool)),
                        tControl,SLOT(onAddEntity(EntityType,string,
                                     map<string,string>&,bool)));
                connect(plugin,SIGNAL(editEntity(Entity*,
                                     map<string,string>&,bool)),
                        tControl,SLOT(onEditEntity(Entity*,
                                     map<string,string>&,bool)));
                connect(plugin,SIGNAL(removeEntity(Entity*,bool)),
                        tControl,SLOT(onRemoveEntity(Entity*,bool)));

            } //endif
        }//endfor
    }

    void PluginControl::connectRegion(IPlugin *plugin,
                                      TransactionControl *tControl) {


        connect(tControl,SIGNAL(regionAdded(QString,Entity*)),
                plugin, SLOT(onEntityAdded(QString,Entity*)));
        connect(tControl,SIGNAL(regionChanged(QString,Entity*)),
                plugin, SLOT(onEntityChanged(QString,Entity*)));
        connect(tControl,SIGNAL(regionRemoved(QString,string)),
                plugin, SLOT(onEntityRemoved(QString,string)));
        connect(tControl,SIGNAL(aboutToRemoveRegion(Entity*)),
                plugin, SLOT(onEntityAboutToRemove(Entity*)),Qt::BlockingQueuedConnection);
    }

    void PluginControl::connectRegionBase(IPlugin *plugin,
                                          TransactionControl *tControl) {

        connect(tControl,SIGNAL(regionBaseAdded(QString,Entity*)),
                plugin, SLOT(onEntityAdded(QString,Entity*)));
        connect(tControl,SIGNAL(regionBaseChanged(QString,Entity*)),
                plugin, SLOT(onEntityChanged(QString,Entity*)));
        connect(tControl,SIGNAL(regionBaseRemoved(QString,string)), plugin,
                SLOT(onEntityRemoved(QString,string)));
        connect(tControl,SIGNAL(aboutToRemoveRegionBase(Entity*)),
                plugin, SLOT(onEntityAboutToRemove(Entity*)),Qt::BlockingQueuedConnection);
    }

}
}
}//fim namespace composer
