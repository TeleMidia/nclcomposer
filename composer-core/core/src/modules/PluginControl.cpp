#include "modules/PluginControl.h"


namespace composer {
    namespace core {
        namespace module {

    PluginControl::PluginControl() {
    }

    PluginControl::~PluginControl() {
         /*QMultiHash<QString,IPlugin*>::iterator itInst;
         QHash<QString,IPluginFactory*>::iterator itFac;

         IPlugin *inst = NULL;
         IPluginFactory *fac = NULL;

         for (itInst = pluginInstances.begin() ;
              itInst != pluginInstances.end() ; itInst++)
         {
                 QList<IPlugin*> instances = pluginInstances.values
                                                            (itInst.key());
                 QList<IPlugin*>::iterator it;

                 for (it = instances.begin(); it != instances.end(); it++)
                 {
                    inst = *it;
                    fac = pluginFactories[inst->getPluginID()];
                    fac->releasePluginInstance(inst);
                    //pluginInstances.remove(itInst.key(),inst);
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
         transactionControls.clear();*/

    }

    void PluginControl::loadPlugins(QString pluginsDirPath) {
        IPluginFactory *pluginFactory = NULL;

        QDir pluginsDir = QDir(pluginsDirPath);
        pluginsDir.setFilter(QDir::Files | QDir::NoSymLinks);

        if(!pluginsDir.exists())
        {
            emit notifyError(tr("The Plugin Directory (%1) does not exist").
                             arg(pluginsDirPath));
            return;
        }
        
        foreach (QString fileName, pluginsDir.entryList(QDir::Files
                                                     | QDir::NoSymLinks)) {

             QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
             QObject *plugin = loader.instance();

             qDebug() << "loadingPlugin( " << fileName << ")";
             if (plugin)
             {
                 pluginFactory = qobject_cast<IPluginFactory*> (plugin);
                 if (pluginFactory)
                 {
                     QString pluginID = pluginFactory->getPluginID();
                     if (!pluginFactories.contains(pluginID))
                     {
                         pluginFactories[pluginID] = pluginFactory;
                         QList<LanguageType> types =
                                 pluginFactory->getSupportLanguages();
                         QList<LanguageType>::iterator it;
                         for (it = types.begin() ; it!= types.end(); it++)
                         {
                             pluginsByType.insert(*it,
                                                  pluginFactory->getPluginID());
                         }
                     } else loader.unload();
                 }
             }//fim OK load
             else {
                qDebug() << "PluginControl::loadPlugins failed to load"
                         << "(" << fileName << ")";
             }
        }//fim foreach
    }//fim function

    void PluginControl::launchDocument(Document *doc)
    {
        TransactionControl *transControl;
        IDocumentParser *parser;
        IPluginFactory *factory;
        IPlugin *pluginInstance;
        LanguageType type = doc->getDocumentType();
        ILanguageProfile *profile = LanguageControl::getInstance()->
                                    getProfileFromType(type);
        QString location  = doc->getLocation();

        transControl = new TransactionControl(doc);
        transactionControls[location] = transControl;

        /* Requests a new DocumentParser for this Document*/
        parser = profile->createDocumentParser(doc);

        QList<QString> plugIDs = pluginsByType.values(type);
        QList<QString>::iterator it;
        for (it = plugIDs.begin() ; it != plugIDs.end() ;
             it++)
        {
            factory        = pluginFactories[*it];
            pluginInstance = factory->createPluginInstance();
            if (pluginInstance)
            {
                pluginInstance->setPluginID(factory->getPluginID());
                pluginInstance->setDocument(doc);
                launchNewPlugin(pluginInstance,transControl);
                pluginInstances.insert(location,pluginInstance);
                emit addPluginWidgetToWindow(factory,pluginInstance,doc);
            }
            else {
                emit notifyError(tr("Could not create a instance for the"
                                    "plugin (%1)").arg(*it));
            }
        }
        connectParser(parser,transControl);
        parser->parseDocument();
        profile->releaseDocumentParser(parser);
        //emit newDocumentLaunchedAndCreated(doc);
    }

    void PluginControl::launchNewPlugin(IPlugin *plugin,
                                        TransactionControl *tControl) {
        qDebug() << "PluginControl::launchNewPlugin(" << plugin->getPluginID()
                 << ")";

        /* Connect signals from the core to slots in the plugins */
        connect(tControl,SIGNAL(entityAdded(QString,Entity*)),
                plugin, SLOT(onEntityAdded(QString,Entity*)));
        connect(tControl,SIGNAL(entityChanged(QString,Entity*)),
                plugin,SLOT(onEntityChanged(QString,Entity*)));
        connect(tControl,SIGNAL(entityRemoved(QString,QString)),
                plugin,SLOT(onEntityRemoved(QString,QString)));
        connect(tControl,SIGNAL(aboutToRemoveEntity(Entity*)),
                plugin, SLOT(onEntityAboutToRemove(Entity*)));

        /* Connect signals from the plugin to slots of the core */
        connect(plugin,
                SIGNAL(addEntity(QString,QString,QMap<QString,QString>&,bool)),
                tControl,
                SLOT(onAddEntity(QString,QString,QMap<QString,QString>&,bool)));
        connect(plugin,SIGNAL(editEntity(Entity*,QMap<QString,QString>&,bool)),
                tControl,
                SLOT(onEditEntity(Entity*,QMap<QString,QString>&,bool)));
        connect(plugin,SIGNAL(removeEntity(Entity*,bool)),
                tControl,
                SLOT(onRemoveEntity(Entity*,bool)));
    }

    void PluginControl::connectParser(IDocumentParser *parser,
                         TransactionControl *tControl)
    {
        connect(parser,
                SIGNAL(addEntity(QString,QString,QMap<QString,QString>&,bool)),
                tControl,
                SLOT(onAddEntity(QString,QString,QMap<QString,QString>&,bool)));

        connect(tControl,SIGNAL(entityAdded(QString,Entity*)),
                parser, SLOT(onEntityAdded(QString,Entity*)));
    }

    QList<IPluginFactory*> PluginControl::getLoadedPlugins()
    {
            QHash<QString,IPluginFactory*>::iterator it;
            QList<IPluginFactory*> pList;
            for (it = pluginFactories.begin() ; it != pluginFactories.end();
                 it++)
            {
                pList.append(it.value());
            }
            return pList;
    }

    bool PluginControl::releasePlugins(Document *doc)
    {
            if (!doc) return false;
            if (!transactionControls.contains(doc->getLocation()))
                return false;

            if (!pluginInstances.contains(doc->getLocation())) return false;

            TransactionControl *t = transactionControls.value
                                    (doc->getLocation());
            if (t)
            {
                delete t;
                t = NULL;
                transactionControls.remove(doc->getLocation());
            }

            QList<IPlugin*> instances = pluginInstances.values
                                                        (doc->getLocation());
            QList<IPlugin*>::iterator it;
            for (it = instances.begin(); it != instances.end(); it++)
            {
               IPlugin *inst = *it;
               //TODO - chamar o save antes de dar o release na instancia
               IPluginFactory *fac = pluginFactories[inst->getPluginID()];
               qDebug() << "releasePlug( " << fac->getPluginName() << " )";
               fac->releasePluginInstance(inst);
            }
            pluginInstances.remove(doc->getLocation());

            return true;
    }

        }
    }
}//end namespace composer
