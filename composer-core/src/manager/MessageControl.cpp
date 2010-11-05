#include "../../include/manager/MessageControl.h"

namespace manager {
    MessageControl::MessageControl() {
        QMutexLocker locker(&mutex);
        this->layoutFacade   = Layout::getInstance();
        this->documentFacade = Document::getInstance();
    }

    MessageControl::~MessageControl() {
        QMutexLocker locker(&mutex);
        Layout::releaseInstance();
        Document::releaseInstance();
    }

    void MessageControl::onCreateDocument(map<string,string>& atts) {
        NclDocument *nclDoc = documentFacade->createNclDocument(atts);
        emit documentCreated(nclDoc);
    }

    void MessageControl::onAddEntity( EntityType entity,
                   string parentEntityId, map<string,string>& atts,
                   bool force) {
        IPluginMessage *plugin = qobject_cast<IPluginMessage *>
                                                        (QObject::sender());
        if(plugin) {
            sender = plugin;
            try {
                addEntity(entity,parentEntityId,atts,force);
            }catch(exception e){
                plugin->onEntityAddError(e.what());
                return;
            }
        } else {
            qDebug() << ;
            return;
        }
    }

    void MessageControl::onEditEntity(EntityType type,Entity *entity,
                                      map<string,string>& atts, bool force) {
        IPluginMessage *plugin = qobject_cast<IPluginMessage *>
                                                        (QObject::sender());
        if(plugin) {
            setSender(plugin);
            try {
                editEntity(type,entity,atts,force);
            }catch(exception e){
                plugin->onEntityAddError(e.what());
                return;
            }
        } else {
            //TODO -- erro on casting
            return;
        }

    }

    void MessageControl::onRemoveEntity( EntityType type, Entity *entity,
                                         bool force) {

        IPluginMessage *plugin = qobject_cast<IPluginMessage *>
                                                        (QObject::sender());
        if(plugin) {
            setSender(plugin);
            try {
                removeEntity(type,entity,force);
            }catch(exception e){
                plugin->onEntityRemoveError(e.what());
                return;
            }
        } else {
            //TODO -- erro on casting
            return;
        }

    }

    void MessageControl::addEntity(EntityType entity,
                                   string parentEntityId, map<string,string>& atts,
                                   bool force) {
        switch(entity) {
            case REGION:
                addRegion(parentEntityId,atts,force);
            break;
            case REGIONBASE:
                addRegionBase(atts,force);
            break;
        }

    }

    void MessageControl::editEntity(EntityType type,Entity *entity,
                                    map<string,string>& atts, bool force) {
        switch(type) {
            case REGION:
               editRegion((Region*)entity,atts,force);
            break;
            case REGIONBASE:
                editRegionBase((Region*)entity,atts,force);
            break;

        }

    }

    void MessageControl::removeEntity(EntityType type,Entity *entity, bool force) {
        switch(type) {
            case REGION:
               removeRegion((Region*)entity,force);
            break;
            case REGIONBASE:
               removeRegionBase((Region*)entity,force);
            break;

        }

    }


    void MessageControl::addRegion(string parentRegionId,
                                   map<string,string> &atts, bool force) {
        Region *region = NULL;
        NclDocument *nclDoc = getSender()->getNclDocument();
        region = layoutFacade->addRegion(nclDoc,parentRegionId,atts);

        /* Everything worked well, then emit the signal */
        emit regionAdded(region);
    }

    void MessageControl::removeRegion(Region* region,bool force) {
        emit aboutToRemoveRegion(region);
        string regionId = region->getAttribute("id");
        NclDocument *nclDoc = getSender()->getNclDocument();
        layoutFacade->removeRegion(nclDoc,region);

        /* Everything went well emit the signal */
        emit regionRemoved(regionId);
    }

    void MessageControl::editRegion(Region *region, map<string, string> &atts,
                                    bool force) {

        NclDocument *nclDoc = getSender()->getNclDocument();
        region = layoutFacade->editRegion(nclDoc,region,atts);
        emit regionChanged(region);

    }

    void MessageControl::addRegionBase(map<string, string> &atts,
                                       bool force){
        NclDocument *nclDoc = getSender()->getNclDocument();
        Region *regionBase = layoutFacade->addRegionBase(nclDoc,atts);
        emit regionBaseAdded(regionBase);

    }

    void MessageControl::removeRegionBase(Region* region,bool force) {
        emit aboutToRemoveRegionBase(region);
        string device = region->getAttribute("device");
        NclDocument *nclDoc = getSender()->getNclDocument();
        layoutFacade->removeRegionBase(nclDoc,device);
        emit regionBaseRemoved(device);

    }

    void MessageControl::editRegionBase(Region *regionBase,
                                        map<string, string> &atts, bool force) {
        NclDocument *nclDoc = getSender()->getNclDocument();
        regionBase = layoutFacade->editRegionBase(nclDoc,regionBase,atts);
        emit regionBaseChanged(regionBase);

    }
}
