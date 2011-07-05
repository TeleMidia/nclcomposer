#include "qnlycomposerplugin.h"

namespace composer {
namespace plugin {
namespace layout {

QnlyComposerPlugin::QnlyComposerPlugin(QObject* parent)
{
    setParent(parent);

    createView();
    createDocument();
    createConnections();
}

QnlyComposerPlugin::~QnlyComposerPlugin()
{

}

void QnlyComposerPlugin::createView()
{
    view = new QnlyView();
}

void QnlyComposerPlugin::createDocument()
{

}

void QnlyComposerPlugin::createConnections()
{
    connect(view, SIGNAL(itemChanged(QString,QString,QMap<QString,QString>)), SLOT(changeRegion(QString,QString,QMap<QString,QString>)));
}

QWidget* QnlyComposerPlugin::getWidget()
{
    return view;
}

bool QnlyComposerPlugin::saveSubsession()
{

}

void QnlyComposerPlugin::updateFromModel()
{

}

void QnlyComposerPlugin::errorMessage(QString error)
{

}

void QnlyComposerPlugin::onEntityAdded(QString pluginID, Entity *entity)
{    
    if (entity != NULL){
        if (entity->getType() == "region"){
            addRegion(entity);

        }else if (entity->getType() == "regionBase"){

            addRegionBase(entity);
        }
    }
}

void QnlyComposerPlugin::onEntityRemoved(QString pluginID, QString entityID)
{

}

void QnlyComposerPlugin::onEntityChanged(QString pluginID, Entity *entity)
{
    if (entity != NULL){
        if (entity->getType() == "region"){
            changeRegion(entity);

        }else if (entity->getType() == "regionBase"){
            changeRegionBase(entity);
        }
    }
}

void QnlyComposerPlugin::changeSelectedEntity (void*)
{

}

void QnlyComposerPlugin::addRegion(Entity* entity)
{
    if (entity != NULL){
        if (entity->getType() == "region"){
            // setting
            QString entityUID;

            if (!entity->getUniqueId().isEmpty()){
                entityUID = entity->getUniqueId();

            }else{
                return; // cancelling addition
            }

            QString regionUID;

            regionUID = entityUID;

            QString parentregionUID;

            if (regions.contains(entity->getParentUniqueId())){
                parentregionUID = entity->getParentUniqueId();

            }else{
                parentregionUID = "";
            }

            QString regionbaseUID;

            if (regions.contains(parentregionUID)){
                regionbaseUID = relations[parentregionUID];

            }else if (regionbases.contains(entity->getParentUniqueId())){
                regionbaseUID = entity->getParentUniqueId();

            }else{
                return; // cancelling addition
            }

            QMap<QString, QString> attributes;

            if (!entity->getAttribute("id").isEmpty()){
                attributes["id"] =  entity->getAttribute("id");

            }else{
                // no default value
            }

            if (!entity->getAttribute("title").isEmpty()){
                attributes["title"] =  entity->getAttribute("title");

            }else{
                // no default value
            }

            if (!entity->getAttribute("zIndex").isEmpty()){
                attributes["zIndex"] =  entity->getAttribute("zIndex");

            }else{
                // no default value
            }

            if (!entity->getAttribute("top").isEmpty()){
                attributes["top"] =  entity->getAttribute("top");

            }else{
                // no default value
            }

            if (!entity->getAttribute("left").isEmpty()){
                attributes["left"] =  entity->getAttribute("left");

            }else{
                // no default value
            }

            if (!entity->getAttribute("bottom").isEmpty()){
                attributes["bottom"] =  entity->getAttribute("bottom");

            }else{
                // no default value
            }

            if (!entity->getAttribute("right").isEmpty()){
                attributes["right"] =  entity->getAttribute("right");

            }else{
                // no default value
            }

            if (!entity->getAttribute("width").isEmpty()){
                attributes["width"] =  entity->getAttribute("width");

            }else{
                // no default value
            }

            if (!entity->getAttribute("height").isEmpty()){
                attributes["height"] =  entity->getAttribute("height");

            }else{
                // no default value
            }

            // adding
            regions[regionUID] = entity;

            relations[regionUID] = regionbaseUID;


            view->addRegion(regionUID, parentregionUID, regionbaseUID, attributes);
        }
    }
}

void QnlyComposerPlugin::removeRegion(Entity* entity)
{
    if (entity != NULL){
        if (entity->getType() == "region"){
            // setting
            QString entityUID;

            if (entity->getUniqueId() != NULL){
                entityUID = entity->getUniqueId();

            }else{
                return; // cancelling remotion
            }

            QString regionUID;

            if (entities.contains(entityUID)){
                regionUID = entities[entityUID];

            }else{
                return; // cancelling remotion
            }

            QString regionbaseUID;

            if (relations.contains(regionUID)){
                regionbaseUID = relations[regionUID];

            }else{
                return; // cancelling remotion
            }

            view->removeRegion(regionUID, regionbaseUID);
        }
    }
}

void QnlyComposerPlugin::selectRegion(Entity *entity)
{

}

void QnlyComposerPlugin::changeRegion(Entity *entity)
{
    if (entity != NULL){
        if (entity->getType() == "region"){
            // setting
            QString entityUID;

            if (entity->getUniqueId() != NULL){
                entityUID = entity->getUniqueId();

            }else{
                return; // cancelling addition
            }

            QString regionUID;

            if (regions.contains(entityUID)){
                regionUID = entityUID;

            }else{
                return; // canceling changing
            }

            QString regionbaseUID;

            if (regionbases.contains(relations[regionUID])){
                regionbaseUID = relations[regionUID];

            }else{
                return; // canceling changing
            }

            QMap<QString, QString> attributes;

            if (entity->getAttribute("id") != NULL){
                attributes["id"] =  entity->getAttribute("id");

            }else{
                // no default value
            }

            if (entity->getAttribute("title") != NULL){
                attributes["title"] =  entity->getAttribute("title");

            }else{
                // no default value
            }

            if (entity->getAttribute("zIndex") != NULL){
                attributes["zIndex"] =  entity->getAttribute("zIndex");

            }else{
                // no default value
            }

            if (entity->getAttribute("top") != NULL){
                attributes["top"] =  entity->getAttribute("top");

            }else{
                // no default value
            }

            if (entity->getAttribute("left") != NULL){
                attributes["left"] =  entity->getAttribute("left");

            }else{
                // no default value
            }

            if (entity->getAttribute("bottom") != NULL){
                attributes["bottom"] =  entity->getAttribute("bottom");

            }else{
                // no default value
            }

            if (entity->getAttribute("right") != NULL){
                attributes["right"] =  entity->getAttribute("right");

            }else{
                // no default value
            }

            if (entity->getAttribute("width") != NULL){
                attributes["width"] =  entity->getAttribute("width");

            }else{
                // no default value
            }

            if (entity->getAttribute("height") != NULL){
                attributes["height"] =  entity->getAttribute("height");

            }else{
                // no default value
            }

            // adding
            view->changeRegion(regionUID, regionbaseUID, attributes);
        }
    }
}

void QnlyComposerPlugin::addRegion(const QString regionUID, const QString parentregionUID, const QString regionbaseUID, const QMap<QString, QString> &attributes)
{
    if (!regions.contains(regionUID) && regionbases.contains(regionbaseUID)){
        // setting
        QMap<QString, QString> standard;

        if (attributes.contains("id")){
            standard["id"] = attributes["id"];

        }else{
            // no default value
        }

        if (attributes.contains("title")){
            standard["title"] = attributes["title"];

        }else{
            // no default value
        }

        if (attributes.contains("top")){
            standard["top"] = attributes["top"];

        }else{
            // no default value
        }

        if (attributes.contains("left")){
            standard["left"] = attributes["left"];

        }else{
            // no default value
        }

        if (attributes.contains("bottom")){
            standard["bottom"] = attributes["bottom"];

        }else{
            // no default value
        }

        if (attributes.contains("right")){
            standard["right"] = attributes["right"];

        }else{
            // no default value
        }

        if (attributes.contains("width")){
            standard["width"] = attributes["width"];

        }else{
            // no default value
        }

        if (attributes.contains("height")){
            standard["height"] = attributes["height"];

        }else{
            // no default value
        }

        if (attributes.contains("zIndex")){
            standard["zIndex"] = attributes["zIndex"];

        }else{
            // no default value
        }

        // emitting
        if (parentregionUID != NULL){
            emit addEntity("region", parentregionUID, standard, false);

        }else{
            emit addEntity("region", regionbaseUID, standard, false);
        }
    }
}

void QnlyComposerPlugin::removeRegion(const QString regionUID, const QString regionbaseUID)
{
/*    if (regions.contains(regionUID) && regionbases.contains(regionbaseUID)){
        if (!entitystack.isEmpty()){
            // setting
            QString entityUID;

            entityUID = entitystack.pop();

            // removing
            regions.remove(regionUID);

            relations.remove(regionUID);

            entities.remove(entityUID);

        }else{
            // setting
            QString entityUID;

            entityUID = regions[regionUID];

            regionstack.push(regionUID);

            // removing
            regions.remove(regionUID);

            relations.remove(regionUID);

            // emitting
            emit removeEntity(getProject()->getEntityBydId(entityUID), false);
        }
    }*/
}



void QnlyComposerPlugin::selectRegion(const QString regionUID, const QString regionbaseUID)
{

}

void QnlyComposerPlugin::changeRegion(const QString regionUID, const QString regionbaseUID, const QMap<QString, QString> &attributes)
{

    /*if (regions.contains(regionUID) && regionbases.contains(regionbaseUID)){
        if (!entitystack.isEmpty()){
            // nothing to do

        }else{
            // setting
            QString entityUID;

            entityUID = entitystack.pop();
*/
            QMap<QString, QString> standard;

            if (attributes.contains("id")){
                standard["id"] = attributes["id"];

            }else{
                // no default value
            }

            if (attributes.contains("title")){
                standard["title"] = attributes["title"];

            }else{
                // no default value
            }

            if (attributes.contains("top")){
                standard["top"] = attributes["top"];

            }else{
                // no default value
            }

            if (attributes.contains("left")){
                standard["left"] = attributes["left"];

            }else{
                // no default value
            }

            if (attributes.contains("bottom")){
                standard["bottom"] = attributes["bottom"];

            }else{
                // no default value
            }

            if (attributes.contains("right")){
                standard["right"] = attributes["right"];

            }else{
                // no default value
            }

            if (attributes.contains("width")){
                standard["width"] = attributes["width"];

            }else{
                // no default value
            }

            if (attributes.contains("height")){
                standard["height"] = attributes["height"];

            }else{
                // no default value
            }

            if (attributes.contains("zIndex")){
                standard["zIndex"] = attributes["zIndex"];

            }else{
                // no default value
            }

            // emitting
            emit setAttributes(getProject()->getEntityById(regionUID), standard, false);
//        }
//    }
}

void QnlyComposerPlugin::addRegionBase(Entity *entity)
{

    if (entity != NULL){
        if (entity->getType() == "regionBase"){
            // setting
            QString entityUID;

            if (!entity->getUniqueId().isEmpty()){
                entityUID = entity->getUniqueId();

            }else{
                return; // cancelling addition
            }

            QString regionbaseUID;

            regionbaseUID = entityUID;

            QMap<QString, QString> attributes;

            if (!entity->getAttribute("id").isEmpty()){
                attributes["id"] =  entity->getAttribute("id");

            }else{
                // no default value
            }

            if (!entity->getAttribute("region").isEmpty()){
                attributes["region"] =  entity->getAttribute("region");

            }else{
                // no default value
            }

            if (!entity->getAttribute("device").isEmpty()){
                attributes["device"] =  entity->getAttribute("device");

            }else{
                // no default value
            }

            // adding
            regionbases[regionbaseUID] = entity;

            view->addRegionbase(regionbaseUID, attributes);
//            addRegionbase(regionbaseUID, attributes);
        }
    }
}

void QnlyComposerPlugin::removeRegionBase(Entity *entity)
{

}

void QnlyComposerPlugin::selectRegionBase(Entity *entity)
{

}

void QnlyComposerPlugin::changeRegionBase(Entity *entity)
{

}

void QnlyComposerPlugin::addRegionBase(const QString regionbaseUID, const QMap<QString, QString> &attributes)
{

}

void QnlyComposerPlugin::removeRegionBase(const QString regionbaseUID)
{

}

void QnlyComposerPlugin::selectRegionBase(const QString regionbaseUID)
{

}

void QnlyComposerPlugin::changeRegionBase(const QString regionbaseUID, const QMap<QString, QString> &attributes)
{

}

}
}
}
