#include "../../include/modules/TransactionControl.h"


namespace composer {
namespace core {
namespace module {



    TransactionControl::TransactionControl(NclDocument *nclDoc) {
        this->nclDoc = nclDoc;
        qDebug() <<  "TransactionControl::TransactionControl("
                <<  QString::fromStdString(nclDoc->getAttribute("id")) << ")";
    }

    TransactionControl::~TransactionControl() {

    }

    void TransactionControl::onAddEntity( EntityType entity,
                   string parentEntityId, map<string,string>& atts,
                   bool force) {

        /* Cast to IPlugin to make sure it's a plugin */
        IPlugin *plugin = qobject_cast<IPlugin *>
                                                        (QObject::sender());

        if(plugin) {
            try {
                addEntity(plugin->getPluginID(),
                          entity,parentEntityId,atts,force);
            }catch(exception e){
                plugin->onEntityAddError(e.what());
                return;
            }
        } else { //It's not a plugin
            qDebug() << "TransactionControl::onAddEntity " <<
                        "Trying to add a entity but is not from a plugin";
            return;
        }
    }

    void TransactionControl::onEditEntity(Entity *entity,
                                      map<string,string>& atts, bool force) {
        IPlugin *plugin = qobject_cast<IPlugin *>
                                                        (QObject::sender());
        if(plugin) {
            try {
                editEntity(plugin->getPluginID(),entity->getType(),
                           entity,atts,force);
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
            try {
                removeEntity(plugin->getPluginID(),entity->getType(),entity,force);
            }catch(exception e){
                plugin->onEntityRemoveError(e.what());
                return;
            }
        } else {
            //TODO -- erro on casting
            return;
        }

    }

    void TransactionControl::addEntity(QString ID, EntityType entity,
                                   string parentEntityId, map<string,string>& atts,
                                   bool force) {
        switch(entity) {
            case NCL:
                addNcl(ID,atts,force);
            break;
            case REGION:
                addRegion(ID,parentEntityId,atts,force);
            break;
            case REGIONBASE:
                addRegionBase(ID,atts,force);
            break;
        }

    }

    void TransactionControl::editEntity(QString ID,
                                        EntityType type,Entity *entity,
                                        map<string,string>& atts, bool force) {
        switch(type) {
            case REGION:
               editRegion(ID,(Region*)entity,atts,force);
            break;
            case REGIONBASE:
                editRegionBase(ID,(Region*)entity,atts,force);
            break;

        }

    }

    void TransactionControl::removeEntity(QString ID, EntityType type,
                                          Entity *entity, bool force) {
        switch(type) {
            case REGION:
               removeRegion(ID,(Region*)entity,force);
            break;
            case REGIONBASE:
               removeRegionBase(ID,(Region*)entity,force);
            break;

        }

    }


    void TransactionControl::addNcl(QString ID,
                                    map<string,string>& atts, bool force) {
        qDebug() << "TransactionControl::addNcl " << "creating NCL Document";
//        NclDocument *nclDoc = NULL;
//        nclDoc = Document::getInstance()->createNclDocument(atts);

        //emit nclAdded(ID, NCL,nclDoc);
    }

    void TransactionControl::addRegion(QString ID, string parentRegionId,
                                   map<string,string> &atts, bool force) {
        Region *region = NULL;
        region = Layout::getInstance()->addRegion(nclDoc,parentRegionId,atts);

        /* Everything worked well, then emit the signal */
        emit regionAdded(ID,region);
    }

    void TransactionControl::removeRegion(QString ID, Region* region,
                                          bool force) {
        emit aboutToRemoveRegion(region);
        string regionId = region->getAttribute("id");
        Layout::getInstance()->removeRegion(nclDoc,region);

        /* Everything went well emit the signal */
        emit regionRemoved(ID, regionId);
    }

    void TransactionControl::editRegion(QString ID, Region *region,
                                        map<string, string> &atts,
                                        bool force) {

        region = Layout::getInstance()->editRegion(nclDoc,region,atts);
        emit regionChanged(ID, region);

    }

    void TransactionControl::addRegionBase(QString ID, map<string, string> &atts,
                                           bool force){
        Region *regionBase = Layout::getInstance()->addRegionBase(nclDoc,atts);
        emit regionBaseAdded(ID,regionBase);

    }

    void TransactionControl::removeRegionBase(QString ID,
                                              Region* region,bool force) {
        emit aboutToRemoveRegionBase(region);
        string device = region->getAttribute("device");
        Layout::getInstance()->removeRegionBase(nclDoc,device);
        emit regionBaseRemoved(ID, device);

    }

    void TransactionControl::editRegionBase(QString ID, Region *regionBase,
                                        map<string, string> &atts, bool force) {

        regionBase = Layout::getInstance()->editRegionBase(nclDoc,regionBase,atts);
        emit regionBaseChanged(ID, regionBase);

    }

}
}
}
