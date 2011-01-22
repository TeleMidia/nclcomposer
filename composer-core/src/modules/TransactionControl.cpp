#include "include/modules/TransactionControl.h"


namespace composer {
namespace core {
namespace module {

    TransactionControl::TransactionControl(Document *doc) {
        this->doc = doc;
        qDebug() <<  "TransactionControl::TransactionControl("
                <<  doc->getAttribute("id") << ")";
    }

    TransactionControl::~TransactionControl() {

    }

    void TransactionControl::onAddEntity( QString type,
                   QString parentEntityId, QMap<QString,QString>& atts,
                   bool force) {

        /* Cast to IPlugin to make sure it's a plugin */
        IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());
        IDocumentParser *parser = qobject_cast<IDocumentParser*>
                                  (QObject::sender());
        QString ID;
        if(plugin) ID = plugin->getPluginID();
        else if (parser) ID = parser->getParserName();

        Entity *ent = NULL;

        try
        {
            ent = new Entity(atts);
            ent->setType(type);
            //TODO - calll validator to check
            doc->addEntity(ent,parentEntityId);
            emit entityAdded(ID,ent);

        } catch(exception& e){
            if (plugin) plugin->onEntityAddError(e.what());
            else if (parser) parser->onEntityAddError(e.what());
            delete ent;
            ent = NULL;
            return;
        }
    }



    void TransactionControl::onEditEntity(Entity *entity,
                                      QMap<QString,QString>& atts, bool force) {
        IPlugin *plugin = qobject_cast<IPlugin *>
                                                        (QObject::sender());
        if(plugin) {
            QString pluginID = plugin->getPluginID();
            try {
                //TODO - call validator to check
               entity->setAtrributes(atts);
               emit entityChanged(pluginID,entity);
            }catch(exception e){
                plugin->onEntityAddError(e.what());
                return;
            }
        } else {
            //TODO -- erro on casting
            return;
        }

    }

    void TransactionControl::onRemoveEntity( Entity *entity,
                                         bool force) {

        IPlugin *plugin = qobject_cast<IPlugin *>
                                                        (QObject::sender());
        if(plugin) {
            QString pluginID = plugin->getPluginID();
            try {
                QString _id = entity->getUniqueId();
                emit aboutToRemoveEntity(entity);
                /* remember to change, the append should come from the plugin*/
                doc->removeEntity(entity,true);
                emit entityRemoved(pluginID,_id);
            }catch(exception e){
                plugin->onEntityRemoveError(e.what());
                return;
            }
        } else {
            //TODO -- erro on casting
            return;
        }

    }

}
}
}
