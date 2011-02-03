#include "layoutplugin.h"

#include <QDebug>

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

LayoutPlugin::LayoutPlugin()
{
    view = new LayoutView();
    view->createScene();

    window = new MainWindow();
    window->addView(view);

    items = QMap<QString, LayoutItem*>();
}

LayoutPlugin::~LayoutPlugin()
{

}

QWidget* LayoutPlugin::getWidget()
{
    return window;
}

void LayoutPlugin::onEntityAdded(QString ID, Entity* entity)
{
    // is region
    if (entity->getType() == "region"){

        qDebug() << "SETTING:" << entity->getAttribute("id");

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

        qDebug() << "top:" << top;
        qDebug() << "left:" << left;
        qDebug() << "right:" << right;
        qDebug() << "bottom:" << bottom;
        qDebug() << "width:" << width;
        qDebug() << "height:" << height;

        QString entityID = entity->getUniqueId();

        // add entity on local model
        if (!items.contains(entityID)){
//            if (entity->getParent()->getType() != "region"){
//               if (top != -1 &&
//                   left != -1 &&
//                   width != -1 &&
//                   height != -1){




                    items[entityID] = new LayoutItem();

//                    qDebug() << s->sceneRect();

//                    qDebug() << items[ID]->pos();
//                    qDebug() << items[ID]->getSize();

//                }else{
//                    qDebug() << top << left << width << height;
//                }
            }else{
                items[entityID] = new LayoutItem(
                        items[entity->getParent()->getUniqueId()]);
            }

            QGraphicsScene* s = view->scene();
            s->addItem(items[entityID]);

            items[entityID]->setpLeft(left);
            items[entityID]->setpTop(top);
            items[entityID]->setpWidth(width);
            items[entityID]->setpHeight(height);
            items[entityID]->setTitle(entity->getAttribute("id"));


            qDebug() << "ADDED:" << entity->getAttribute("id");
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

}

void LayoutPlugin::onEntityRemoveError(QString error)
{

}

}
}
}
}
}
