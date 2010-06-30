#include "../../include/util/EntityUtil.h"

EntityUtil *EntityUtil::instance = NULL;
QMap<EntityType,QString> EntityUtil::entitiesTypes;

EntityUtil* EntityUtil::getInstance(){
    if (!instance) {
        instance = new EntityUtil();
        instance->init();
    }
    return instance;
}

void EntityUtil::init() {
    entitiesTypes[REGION] = "region";
    entitiesTypes[REGIONBASE] = "regionBase";
}

void EntityUtil::releaseInstance() {
    delete instance;
    instance = NULL;
    entitiesTypes.clear();
}



QString EntityUtil::getEntityName(EntityType entity) {
    if (entitiesTypes.contains(entity))
        return entitiesTypes[entity];
    else
        return "";
}

EntityType EntityUtil::getEntityType(QString entityName) {
    return entitiesTypes.key(entityName,NONE);
}

map<string,string> EntityUtil::toStdMap(QDomNamedNodeMap attsElement) {
    map<string,string> atts;
    for (int i = 0; i < attsElement.count(); i++) {
        QDomNode att = attsElement.item(i);
        if (att.isAttr()) {
            atts[att.nodeName().toStdString()] =
                                               att.nodeValue().toStdString();
        }
    }
    return atts;
}
