#include "modules/MessageControl.h"


namespace composer {
    namespace core {
        namespace module {

MessageControl::MessageControl(Document *doc) {
    this->doc = doc;
    /* qDebug() <<  "MessageControl::MessageControl("
            <<  doc->getAttribute("id") << ")";*/
}

MessageControl::~MessageControl() {

}

void MessageControl::onAddEntity( QString type,
               QString parentEntityId, QMap<QString,QString>& atts,
               bool force) {

    /* Cast to IPlugin to make sure it's a plugin */
    IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());
    IDocumentParser *parser = qobject_cast<IDocumentParser*>
                              (QObject::sender());
    QString ID;
    if(plugin) ID = plugin->getPluginInstanceID();
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
        if (plugin) plugin->errorMessage(e.what());
        else if (parser) parser->onEntityAddError(e.what());
        delete ent;
        ent = NULL;
        return;
    }
}

void MessageControl::onEditEntity(Entity *entity,
                                  QMap<QString,QString>& atts, bool force) {
    IPlugin *plugin = qobject_cast<IPlugin *>
                                                    (QObject::sender());
    if(plugin) {
        QString pluginID = plugin->getPluginInstanceID();
        try {
            //TODO - call validator to check
           entity->setAtrributes(atts);
           emit entityChanged(pluginID,entity);
        }catch(exception e){
            plugin->errorMessage(e.what());
            return;
        }
    } else {
        //TODO -- erro on casting
        return;
    }

}

void MessageControl::onRemoveEntity( Entity *entity,
                                     bool force) {

    IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());
    if(plugin) {
        QString pluginID = plugin->getPluginInstanceID();
        try {
            QString _id = entity->getUniqueId();
            emit aboutToRemoveEntity(entity);
            /* remember to change, the append should come from the plugin*/
            doc->removeEntity(entity,true);
            emit entityRemoved(pluginID,_id);
        }catch(exception e){
            plugin->errorMessage(e.what());
            return;
        }
    } else {
        //TODO -- erro on casting
        return;
    }

}
        }
    }
}//end namespace
