#include "include/modules/PluginControl.h"


namespace composer {
namespace core {
namespace module {

    PluginControl::PluginControl() {
        qDebug() << "PluginControl::PluginControl()";
    }

    PluginControl::~PluginControl() {
         QHash<QString,IPluginFactory*>::iterator itFac;

         IPluginFactory *atualFactory = NULL;

         for (itFac = pluginFactories.begin() ; itFac != pluginFactories.end();
              itFac++)
         {
                 atualFactory = itFac.value();
                 QList<IPlugin*> instances = pluginInstances.values
                                                            (itFac.key());
                 QList<IPlugin*>::iterator itInst;

                 for (itInst = instances.begin() ; itInst != instances.end();
                      itInst++)
                 {
                     atualFactory->releasePluginInstance(*itInst);
                 }
                 delete atualFactory;
                 atualFactory = NULL;
         }

         pluginFactories.clear();
         pluginInstances.clear();
         pluginsByType.clear();

    }

    void PluginControl::loadPlugins(QString pluginsDirPath) {
        IPluginFactory *pluginFactory = NULL;

        QDir pluginsDir = QDir(pluginsDirPath);

        if(!pluginsDir.exists())
        {
            emit notifyError(tr("The Plugin Directory (%1) does not exist").
                             arg(pluginsDirPath));
            return;
        }
        
        foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
             QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
             QObject *plugin = loader.instance();

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
                     }//fim OK insertMap
                 }//fim if OK pluginFactory
             }//fim OK load
             else {
                qDebug() << "PluginControl::loadPlugins failed to load"
                         << "(" << fileName << ")";
             }
        }//fim foreach
    }//fim function

    void PluginControl::onNewDocument(QString documentId,
                                      QString location) {

        qDebug() << "PluginControl::onNewDocument(" << documentId
                 << ", " << location << ")";

        IPluginFactory *factory;
        IPlugin *pluginInstance;
        TransactionControl *transControl;
        ILanguageProfile *profile;
        IDocumentParser *parser;
        QMap<QString,QString> atts;

        if (transactionControls.contains(documentId)) {
            //TODO - mudar o para a chave da HASH levar em conta o projeto
            qDebug() << tr("Transaction Control could not be created"
                                "for Document(%1)").arg(documentId);
            return;
        }

        QString ext = location;
        ext = ext.remove(0,ext.lastIndexOf(".")+1);
        LanguageType type = Utilities::getLanguageTypeByExtension(ext);

        if(type == NONE) {
            emit notifyError(tr("No Language Profile found for (%1)").
                             arg(ext.toUpper()));
            return;
        }

        /*Requests the LanguageProfile associated with this DocumentType */
        profile = LanguageControl::getInstance()->getProfileFromType(type);
        if (!profile) {
            emit notifyError(tr("No Language Profile Extension "
                                "found for (%1)").
                             arg(ext.toUpper()));
            return;
        }

        atts["id"] = documentId;

        /* create the NCLDocument */
        Document *doc = new Document(atts);
        doc->setLocation(location);
        doc->setDocumentType(type);

        transControl = new TransactionControl(doc);
        transactionControls[documentId] = transControl;

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
                pluginInstances.insert(*it,pluginInstance);
                emit addPluginWidgetToWindow(pluginInstance,location);
            }
            else {
                qDebug() << "Could not create a instance for the plugin"
                        << "(" << *it << ")";
            }
        }
        connectParser(parser,transControl);
        parser->parseDocument();
        profile->releaseDocumentParser(parser);
        emit newDocumentLaunchedAndCreated(documentId,location);
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
                plugin, SLOT(onEntityAboutToRemove(Entity*)),
                Qt::BlockingQueuedConnection);

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



}
}
}//fim namespace composer
