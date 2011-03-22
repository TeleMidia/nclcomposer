#include "layoutplugin.h"

#include <QDebug>

#include <math.h>

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

LayoutPlugin::LayoutPlugin()
{
    view = new LayoutView();
    scene = view->createScene();

    connect(scene,SIGNAL(itemAdded(LayoutItem*)), SLOT(addRegion(LayoutItem*)));
    connect(scene,SIGNAL(itemRemoved(LayoutItem*)), SLOT(removeRegion(LayoutItem*)));

    window = new MainWindow();
    window->addView(view);

//    idety = QMap<QString, Entity*>();
    items = QMap<QString, LayoutItem*>();
    items2 = QMap<LayoutItem*, QString>();
}

LayoutPlugin::~LayoutPlugin()
{

}

void LayoutPlugin::removeRegion(LayoutItem* item)
{
    removeEntity(getDocument()->getEntityBydId(items2[item]),false);
}

void LayoutPlugin::addRegion(LayoutItem* item)
{
    qDebug() << "LayoutPlugin::addRegion";

    if (item->parentItem() != NULL){
        LayoutItem* p = (LayoutItem*) item->parentItem();

        lasti = item;

        QMap<QString, QString> att;
        att["top"] = QString::number(ceil(item->getpTop()*100)) + "%";
        att["left"] = QString::number(ceil(item->getpTop()*100)) + "%";
        att["width"] = QString::number(ceil(item->getpTop()*100)) + "%";
        att["bottom"] = QString::number(ceil(item->getpTop()*100)) + "%";

        addEntity("region", items2[p],att, false);
    }

//    void addEntity( QString type, QString parentEntityId,
//                           QMap<QString,QString>& atts, bool force);
}

QWidget* LayoutPlugin::getWidget()
{
    return window;
}

void LayoutPlugin::onEntityAdded(QString ID, Entity* entity)
{
    if (getPluginID() != ID){

    // is region
    if (entity->getType() == "region"){

//        qDebug() << "SETTING:" << entity->getAttribute("id");

//        qDebug() << "region";

        // adjust param for local coordinates
        QString stop = entity->getAttribute("top");
        QString sbottom = entity->getAttribute("bottom");
        QString sleft = entity->getAttribute("left");
        QString sright = entity->getAttribute("right");
        QString swidth = entity->getAttribute("width");
        QString sheight = entity->getAttribute("height");

        qreal top = 0;
        qreal bottom  = -1;
        qreal left  = 0;
        qreal right = -1;
        qreal width = -1;
        qreal height = -1;

        if (stop.contains("px")){
            top = (stop.remove(stop.length()-2,2)).toDouble();
        }else if (stop.contains("%")){
            top = (stop.remove(stop.length()-1,1)).toDouble();
            top /= 100;
        }


        if (sbottom.contains("px")){
            bottom = (sbottom.remove(sbottom.length()-2,2)).toDouble();
        }else if (sbottom.contains("%")){
            bottom = (sbottom.remove(sbottom.length()-1,1)).toDouble();
            bottom /= 100;
        }

        if (sleft.contains("px")){
            left = (sleft.remove(sleft.length()-2,2)).toDouble();
        }else if (sleft.contains("%")){
            left = (sleft.remove(sleft.length()-1,1)).toDouble();
            left /= 100;
        }

        if (sright.contains("px")){
            right = (sright.remove(sright.length()-2,2)).toDouble();
        }else if (sright.contains("%")){
            right = (sright.remove(sright.length()-1,1)).toDouble();
            right /= 100;
        }

        if (swidth.contains("px")){
            width = (swidth.remove(swidth.length()-2,2)).toDouble();
        }else if (swidth.contains("%")){
            width = (swidth.remove(swidth.length()-1,1)).toDouble();
            width /= 100;
        }

        if (sheight.contains("px")){
            height = (sheight.remove(sheight.length()-2,2)).toDouble();
        }else if (sheight.contains("%")){
            height = (sheight.remove(sheight.length()-1,1)).toDouble();
            height /= 100;
        }

//        qDebug() << "top:" << top;
//        qDebug() << "left:" << left;
//        qDebug() << "right:" << right;
//        qDebug() << "bottom:" << bottom;
//        qDebug() << "width:" << width;
//        qDebug() << "height:" << height;

        QString entityID = entity->getUniqueId();
        QGraphicsScene* s = view->scene();
        // add entity on local model
//        if (!items.contains(entityID)){
            if (entity->getParent()->getType() != "region"){
//               if (top != -1 &&
//                   left != -1 &&
//                   width != -1 &&
//                   height != -1){




                    items[entityID] = new LayoutItem();
                    items2[items[entityID]] = entityID;
                    s->addItem(items[entityID]);

                    connect(items[entityID],SIGNAL(itemAdded(LayoutItem*)),this,SLOT(addRegion(LayoutItem*)));
                     connect(items[entityID],SIGNAL(itemRemoved(LayoutItem*)),this,SLOT(removeRegion(LayoutItem*)));


//                    qDebug() << s->sceneRect();

//                    qDebug() << items[ID]->pos();
//                    qDebug() << items[ID]->getSize();

//                }else{
//                    qDebug() << top << left << width << height;
//                }
            }else{
                items[entityID] = new LayoutItem(
                        items[entity->getParent()->getUniqueId()]);

                items2[items[entityID]] = entityID;

                connect(items[entityID],SIGNAL(itemAdded(LayoutItem*)),this,SLOT(addRegion(LayoutItem*)));
                connect(items[entityID],SIGNAL(itemRemoved(LayoutItem*)),this,SLOT(removeRegion(LayoutItem*)));

            }




            items[entityID]->setpLeft(left);
            items[entityID]->setpTop(top);
            items[entityID]->setpWidth(width);
            items[entityID]->setpHeight(height);
            items[entityID]->setTitle(entity->getAttribute("id"));


           // qDebug() << "ADDED:" << entity->getAttribute("id");
        }
    }else{
        items[entity->getUniqueId()] = lasti;
        items2[lasti] = entity->getUniqueId();
    }
}

void LayoutPlugin::onEntityAddError(QString error)
{

}

void LayoutPlugin::onEntityChanged(QString ID, Entity* entity)
{

}

void LayoutPlugin::onEntityChangeError(QString error)
{

}

void LayoutPlugin::onEntityAboutToRemove(Entity *)
{

}

void LayoutPlugin::onEntityRemoved(QString ID, QString entityID)
{
    qDebug() << "void LayoutPlugin::onEntityRemoved(QString ID, QString entityID)";

    items2.remove(items[entityID]);

    delete(items[entityID]);

    items.remove(entityID);
}

void LayoutPlugin::onEntityRemoveError(QString error)
{

}

bool LayoutPlugin::save()
{
    //TODO: All
}

void LayoutPlugin::updateFromModel()
{
    //TODO: All
}

}
}
}
}
}
