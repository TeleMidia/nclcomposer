#include "qnstchangecommand.h"

QnstChangeCommand::QnstChangeCommand(QnstView* view, QString uid, QMap<QString,QString> properties)
{
    this->uid = uid;
    this->view = view;

    if (view->entities.contains(uid)){
        QnstGraphicsEntity* entity = view->entities[uid];

        previous["top"] = QString::number(entity->getTop());
        previous["left"] = QString::number(entity->getLeft());
        previous["width"] = QString::number(entity->getWidth());
        previous["height"] = QString::number(entity->getHeight());
    }

    next = properties;

    ignore = true;
}

QnstChangeCommand::~QnstChangeCommand()
{

}

void QnstChangeCommand::undo()
{
    if (view->entities.contains(uid)){
        QnstGraphicsEntity* entity = view->entities[uid];

        if (next.contains("top")){
            entity->setTop(previous["top"].toDouble());
        }

        if (next.contains("left")){
            entity->setLeft(previous["left"].toDouble());
        }

        if (next.contains("width")){
            entity->setWidth(previous["width"].toDouble());
        }

        if (next.contains("height")){
            entity->setHeight(previous["height"].toDouble());
        }

        entity->adjust();
    }
}

void QnstChangeCommand::redo()
{
    if (!ignore){
        if (view->entities.contains(uid)){
            QnstGraphicsEntity* entity = view->entities[uid];

            if (next.contains("top")){
                entity->setTop(next["top"].toDouble());
            }

            if (next.contains("left")){
                entity->setLeft(next["left"].toDouble());
            }

            if (next.contains("width")){
                entity->setWidth(next["width"].toDouble());
            }

            if (next.contains("height")){
                entity->setHeight(next["height"].toDouble());
            }

            entity->adjust();
        }

    }else{
        ignore = false;
    }
}
