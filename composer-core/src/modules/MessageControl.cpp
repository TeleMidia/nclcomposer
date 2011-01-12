#include "../../include/modules/MessageControl.h"



namespace composer {
namespace core {
namespace module {

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

    void MessageControl::onAddEntity( EntityType entity,
                   string parentEntityId, map<string,string>& atts,
                   bool force) {

        /* Cast to IPlugin to make sure it's a plugin */

        IPlugin *plugin = qobject_cast<IPlugin *>
                                                        (QObject::sender());

        if(plugin) {
            sender = plugin;
            try {
                addEntity(entity,parentEntityId,atts,force);
            }catch(exception e){
                plugin->onEntityAddError(e.what());
                return;
            }
        } else { //It's not a plugin
            qDebug() << "MessageControl::onAddEntity " <<
                        "Trying to add a entity but is not from a plugin";
            return;
        }
    }

    void MessageControl::onEditEntity(EntityType type,Entity *entity,
                                      map<string,string>& atts, bool force) {
        IPlugin *plugin = qobject_cast<IPlugin *>
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

        IPlugin *plugin = qobject_cast<IPlugin *>
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
            case NCL:
                addNcl(atts,force);
            break;
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


    void MessageControl::addNcl(map<string,string>& atts, bool force) {
        qDebug() << "MessageControl::addNcl " << "creating NCL Document";
        NclDocument *nclDoc = NULL;
        nclDoc = documentFacade->createNclDocument(atts);

        emit nclAdded(nclDoc);
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
}
}
