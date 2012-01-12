#include "qnstgraphicsedge.h"

QnstGraphicsEdge::QnstGraphicsEdge(QnstGraphicsEntity* parent)
    : QnstGraphicsEntity(parent)
{
    setncgType(Qncg::Edge);
    setnstType(Qnst::Edge);

    setSelectable(false);
    setResizable(false);
    setMoveable(false);

    entitya = NULL;
    entityb = NULL;

    setzIndex(-1);

    entityaenabled = true;
    entitybenabled = true;
}

QnstGraphicsEdge::~QnstGraphicsEdge()
{

}

QnstGraphicsEntity* QnstGraphicsEdge::getEntityA() const
{
    return entitya;
}

void QnstGraphicsEdge::setEntityA(QnstGraphicsEntity* entity)
{
    this->entitya = entity;
}

QnstGraphicsEntity* QnstGraphicsEdge::getEntityB() const
{
    return entityb;
}

void QnstGraphicsEdge::setEntityB(QnstGraphicsEntity* entity)
{
    this->entityb = entity;
}

bool QnstGraphicsEdge::isEntityAEnabled() const
{
    return entityaenabled;
}

void QnstGraphicsEdge::setEntityAEnabled(bool enable)
{
    this->entityaenabled = enable;
}

bool QnstGraphicsEdge::isEntityBEnabled() const
{
    return entitybenabled;
}

void QnstGraphicsEdge::setEntityBEnabled(bool enable)
{
    this->entitybenabled = enable;
}

void QnstGraphicsEdge::adjust()
{
    if (entitya != NULL && entityb != NULL){
        QLineF line = QLineF(QPointF(entitya->getLeft() + entitya->getWidth()/2,
                                     entitya->getTop() + entitya->getHeight()/2),
                             QPointF(entityb->getLeft() + entityb->getWidth()/2,
                                     entityb->getTop() + entityb->getHeight()/2));

        if (getEntityA()->getncgType() == Qncg::Interface){
            line.setP1(getnstGraphicsParent()->mapFromItem(getEntityA()->getnstGraphicsParent(), line.p1()));
        }

        if (getEntityB()->getncgType() == Qncg::Interface){
            line.setP2(getnstGraphicsParent()->mapFromItem(getEntityB()->getnstGraphicsParent(), line.p2()));
        }

        QPointF pointa = line.p1();
        QPointF pointb = line.p2();

        if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
            setTop(pointa.y()-6);
            setLeft(pointa.x()-6);
            setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
            setHeight((pointb.y()-6)-(pointa.y()-6) + 12);

        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
            setTop(pointa.y()-6);
            setLeft(pointb.x()-6);
            setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
            setHeight((pointb.y()-6)-(pointa.y()-6) + 12);

        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
            setTop(pointb.y()-6);
            setLeft((pointa.x()-6));
            setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
            setHeight((pointa.y()-6)-(pointb.y()-6) + 12);

        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
            setTop(pointb.y()-6);
            setLeft(pointb.x()-6);
            setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
            setHeight((pointa.y()-6)-(pointb.y()-6) + 12);
        }

        entitya->setSelectable(false);
        entityb->setSelectable(false);

        qreal index;

        if (pointa != pointb){
            index = 1.0;

            int n = 0;

            while(entityb->collidesWithItem(this)){
                index -= 0.01;

                pointb = line.pointAt(index);

                if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
                    setTop(pointa.y()-6);
                    setLeft(pointa.x()-6);
                    setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
                    setHeight((pointb.y()-6)-(pointa.y()-6) + 12);

                }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
                    setTop(pointa.y()-6);
                    setLeft(pointb.x()-6);
                    setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
                    setHeight((pointb.y()-6)-(pointa.y()-6) + 12);

                }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
                    setTop(pointb.y()-6);
                    setLeft((pointa.x()-6));
                    setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
                    setHeight((pointa.y()-6)-(pointb.y()-6) + 12);

                }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
                    setTop(pointb.y()-6);
                    setLeft(pointb.x()-6);
                    setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
                    setHeight((pointa.y()-6)-(pointb.y()-6) + 12);
                }

                if (++n > 100){ // avoiding infinity loop
                    break;
                }
            }

            index = 0;

            n = 0;

            while(entitya->collidesWithItem(this)){
                index += 0.01;

                pointa = line.pointAt(index);

                if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
                    setTop(pointa.y()-6);
                    setLeft(pointa.x()-6);
                    setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
                    setHeight((pointb.y()-6)-(pointa.y()-6) + 12);

                }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
                    setTop(pointa.y()-6);
                    setLeft(pointb.x()-6);
                    setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
                    setHeight((pointb.y()-6)-(pointa.y()-6) + 12);

                }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
                    setTop(pointb.y()-6);
                    setLeft((pointa.x()-6));
                    setWidth((pointb.x()-6)-(pointa.x()-6) + 12);
                    setHeight((pointa.y()-6)-(pointb.y()-6) + 12);

                }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
                    setTop(pointb.y()-6);
                    setLeft(pointb.x()-6);
                    setWidth((pointa.x()-6)-(pointb.x()-6) + 12);
                    setHeight((pointa.y()-6)-(pointb.y()-6) + 12);
                }

                if (++n > 100){ // avoiding infinity loop
                    break;
                }
            }
        }

        entitya->setSelectable(true);
        entityb->setSelectable(true);

        if (scene() != NULL){
            scene()->update();
        }
    }
}

void QnstGraphicsEdge::move(QGraphicsSceneMouseEvent* event)
{
    // nothing to do
}

void QnstGraphicsEdge::resize(QGraphicsSceneMouseEvent* event)
{
    // nothing to do
}
