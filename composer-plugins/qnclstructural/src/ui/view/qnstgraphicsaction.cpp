#include "qnstgraphicsaction.h"

QnstGraphicsAction::QnstGraphicsAction(QnstGraphicsEntity* parent)
    : QnstGraphicsEdge(parent)
{
    setnstType(Qnst::Action);

    setAction(Qnst::NoActionType);

    dialog = new QnstGraphicsBindDialog();

    conn = NULL;
}

QnstGraphicsAction::~QnstGraphicsAction()
{
    delete dialog;
}

QnstAction QnstGraphicsAction::getAction()
{
    return action;
}

void QnstGraphicsAction::setAction(QnstAction action)
{
    this->action = action;

    switch(action){
    case Qnst::Start:
        setActionIcon(QnstActionIcon(":/icon/start"));
        break;

    case Qnst::Stop:
        setActionIcon(QnstActionIcon(":/icon/stop"));
        break;

    case Qnst::Resume:
        setActionIcon(QnstActionIcon(":/icon/resume"));
        break;

    case Qnst::Pause:
        setActionIcon(QnstActionIcon(":/icon/pause"));
        break;

    case Qnst::Set:
        setActionIcon(QnstActionIcon(":/icon/set"));
        break;

    case Qnst::NoActionType:
        setActionIcon(QnstActionIcon(":/icon/noaction"));
        break;
    }
}

QnstActionIcon QnstGraphicsAction::getActionIcon()
{
    return actionIcon;
}

void QnstGraphicsAction::setActionIcon(QnstActionIcon actionIcon)
{
    this->actionIcon = actionIcon;
}

void QnstGraphicsAction::adjust()
{
    if (getEntityA() != NULL && getEntityB() != NULL){
        QLineF line = QLineF(QPointF(getEntityA()->getLeft() + getEntityA()->getWidth()/2,
                                     getEntityA()->getTop() + getEntityA()->getHeight()/2),
                             QPointF(getEntityB()->getLeft() + getEntityB()->getWidth()/2,
                                     getEntityB()->getTop() + getEntityB()->getHeight()/2));

        if (getEntityA()->getncgType() == Qncg::Interface){
            line.setP1(getnstGraphicsParent()->mapFromItem(getEntityA()->getnstGraphicsParent(), line.p1()));
        }

        if (getEntityB()->getncgType() == Qncg::Interface){
            line.setP2(getnstGraphicsParent()->mapFromItem(getEntityB()->getnstGraphicsParent(), line.p2()));
        }

        QPointF pointa = line.p1();
        QPointF pointb = line.p2();

        aux_adjust(pointa, pointb);

        getEntityB()->setSelectable(false);

        qreal index;

        if (pointa != pointb){
            index = 1.0;

            int n = 0;

            while(getEntityB()->collidesWithItem(this)){
                index -= 0.01;

                if (getAngle() == 0)
                    pointb = line.pointAt(index);
                else
                    pointb = arcPointAt(line , index);

                aux_adjust(pointa, pointb);

                if (++n > 100){ // avoiding infinity loop
                    break;
                }
            }
        }

        getEntityB()->setSelectable(true);

        if (scene() != NULL){
            scene()->update();
        }
    }
}

void QnstGraphicsAction::aux_adjust(QPointF pointa, QPointF pointb)
{
    if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
        setTop(pointa.y()-4);
        setLeft(pointa.x()-4);
        setWidth((pointb.x()-4)-(pointa.x()-4) + 8);
        setHeight((pointb.y()-4)-(pointa.y()-4) + 8);

    }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
        setTop(pointa.y()-4);
        setLeft(pointb.x()-4);
        setWidth((pointa.x()-4)-(pointb.x()-4) + 8);
        setHeight((pointb.y()-4)-(pointa.y()-4) + 8);

    }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
        setTop(pointb.y()-4);
        setLeft((pointa.x()-4));
        setWidth((pointb.x()-4)-(pointa.x()-4) + 8);
        setHeight((pointa.y()-4)-(pointb.y()-4) + 8);

    }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
        setTop(pointb.y()-4);
        setLeft(pointb.x()-4);
        setWidth((pointa.x()-4)-(pointb.x()-4) + 8);
        setHeight((pointa.y()-4)-(pointb.y()-4) + 8);
    }
}


void QnstGraphicsAction::draw(QPainter* painter)
{
    if (getEntityA() != NULL && getEntityB() != NULL){
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

        QLineF line = QLineF(QPointF(getEntityA()->getLeft() + getEntityA()->getWidth()/2,
                                     getEntityA()->getTop() + getEntityA()->getHeight()/2),
                             QPointF(getEntityB()->getLeft() + getEntityB()->getWidth()/2,
                                     getEntityB()->getTop() + getEntityB()->getHeight()/2));

        if (getEntityA()->getncgType() == Qncg::Interface){
            line.setP1(getnstGraphicsParent()->mapFromItem(getEntityA()->getnstGraphicsParent(), line.p1()));
        }

        if (getEntityB()->getncgType() == Qncg::Interface){
            line.setP2(getnstGraphicsParent()->mapFromItem(getEntityB()->getnstGraphicsParent(), line.p2()));
        }

        QPointF pointa = line.p1();
        QPointF pointb = line.p2();

        if (!isInvalid()){
            painter->setPen(QPen(QBrush(QColor("#000000")), 1));
        }else{
            painter->setPen(QPen(QBrush(QColor(255,0,0,200)), 1, Qt::DashLine));
        }

        if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
            if (getAngle() != 0){
               qreal drawangle = getAdjAngle();

               QLineF localline(4+4,4+4, 4+8+getWidth()-16, 4+8+getHeight()-16);

               if (drawangle < 0){
                   drawangle = -drawangle;
               }

               qreal R = localline.length()/(::sin(((drawangle/2)*PI)/180)*2);

               qreal delta = (180-drawangle)/2 + (360 - localline.angle());

               QPointF center_a(localline.p2().x() - ::cos((180-delta-drawangle)*PI/180)*R,
                                localline.p2().y() + ::sin((180-delta-drawangle)*PI/180)*R);

               QPointF center_b(localline.p1().x() + ::cos((180-delta-drawangle)*PI/180)*R,
                                localline.p1().y() - ::sin((180-delta-drawangle)*PI/180)*R);

               if (getAdjAngle() < 0){
                   painter->drawArc(center_b.x()-R,center_b.y()-R,2*R,2*R,
                                    16*((180-delta-drawangle)+180),16*drawangle);
               }else{
                   painter->drawArc(center_a.x()-R,center_a.y()-R,2*R,2*R
                                    ,16*(180-delta-drawangle),16*drawangle);
               }

            }else{
                painter->drawLine(4+4,4+4, 4+8+getWidth()-16, 4+8+getHeight()-16);
            }

            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4+getWidth()-16, 4+getHeight()-16, 16, 16, QPixmap(actionIcon));

            if (!isInvalid()){
                painter->setBrush(QBrush(QColor("#000000")));
            }else{
                painter->setBrush(QBrush(QColor(255,0,0,75)));
                painter->drawEllipse(3+getWidth()-16, 3+getHeight()-16, 18, 18);
            }

        }else if (pointa.x() > pointb.x() && pointa.y() <= pointb.y()){

            if (getAngle() != 0){
               qreal drawangle = getAdjAngle();

               QLineF localline(4+4+getWidth()-8,4+4, 4+8, 4+8+getHeight()-16);

               if (drawangle < 0){
                   drawangle = -drawangle;
               }

               qreal R = localline.length()/(::sin(((drawangle/2)*PI)/180)*2);

               qreal delta = (180-drawangle)/2 + (360 - localline.angle());

               QPointF center_a(localline.p2().x() - ::cos((180-delta-drawangle)*PI/180)*R,
                                localline.p2().y() + ::sin((180-delta-drawangle)*PI/180)*R);

               QPointF center_b(localline.p1().x() + ::cos((180-delta-drawangle)*PI/180)*R,
                                localline.p1().y() - ::sin((180-delta-drawangle)*PI/180)*R);

               if (getAdjAngle() < 0){
                   painter->drawArc(center_b.x()-R,center_b.y()-R,2*R,2*R,
                                    16*((180-delta-drawangle)+180),16*drawangle);
               }else{
                   painter->drawArc(center_a.x()-R,center_a.y()-R,2*R,2*R
                                    ,16*(180-delta-drawangle),16*drawangle);
               }

            }else{
                painter->drawLine(4+4+getWidth()-8,4+4, 4+8, 4+8+getHeight()-16);
            }

            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4, 4+getHeight()-16, 16, 16, QPixmap(actionIcon));

            if (!isInvalid()){
                painter->setBrush(QBrush(QColor("#000000")));
            }else{
                painter->setBrush(QBrush(QColor(255,0,0,75)));
                painter->drawEllipse(3, 3+getHeight()-16, 18, 18);
            }

        }else if (pointa.x() <= pointb.x() && pointa.y() > pointb.y()){
            if (getAngle() != 0){
               qreal drawangle = getAdjAngle();

               QLineF localline(4+4, 4+4+getHeight()-8, 4+8+getWidth()-16, 4+8);

               if (drawangle < 0){
                   drawangle = -drawangle;
               }

               qreal R = localline.length()/(::sin(((drawangle/2)*PI)/180)*2);

               qreal delta = (180-drawangle)/2 + (360 - localline.angle());

               QPointF center_a(localline.p2().x() - ::cos((180-delta-drawangle)*PI/180)*R,
                                localline.p2().y() + ::sin((180-delta-drawangle)*PI/180)*R);

               QPointF center_b(localline.p1().x() + ::cos((180-delta-drawangle)*PI/180)*R,
                                localline.p1().y() - ::sin((180-delta-drawangle)*PI/180)*R);

               if (getAdjAngle() < 0){
                   painter->drawArc(center_b.x()-R,center_b.y()-R,2*R,2*R,
                                    16*((180-delta-drawangle)+180),16*drawangle);
               }else{
                   painter->drawArc(center_a.x()-R,center_a.y()-R,2*R,2*R
                                    ,16*(180-delta-drawangle),16*drawangle);
               }

            }else{
                painter->drawLine(4+4, 4+4+getHeight()-8, 4+8+getWidth()-16, 4+8);
            }

            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4+getWidth()-16,4,16,16, QPixmap(actionIcon));

            if (!isInvalid()){
                painter->setBrush(QBrush(QColor("#000000")));
            }else{
                painter->setBrush(QBrush(QColor(255,0,0,75)));
                painter->drawEllipse(3+getWidth()-16,3,18,18);
            }

        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
            if (getAngle() != 0){
               qreal drawangle = getAdjAngle();

               QLineF localline(4+4+getWidth()-8, 4+4+getHeight()-8, 4+8, 4+8);

               if (drawangle < 0){
                   drawangle = -drawangle;
               }

               qreal R = localline.length()/(::sin(((drawangle/2)*PI)/180)*2);

               qreal delta = (180-drawangle)/2 + (360 - localline.angle());

               QPointF center_a(localline.p2().x() - ::cos((180-delta-drawangle)*PI/180)*R,
                                localline.p2().y() + ::sin((180-delta-drawangle)*PI/180)*R);

               QPointF center_b(localline.p1().x() + ::cos((180-delta-drawangle)*PI/180)*R,
                                localline.p1().y() - ::sin((180-delta-drawangle)*PI/180)*R);

               if (getAdjAngle() < 0){
                   painter->drawArc(center_b.x()-R,center_b.y()-R,2*R,2*R,
                                    16*((180-delta-drawangle)+180),16*drawangle);
               }else{
                   painter->drawArc(center_a.x()-R,center_a.y()-R,2*R,2*R
                                    ,16*(180-delta-drawangle),16*drawangle);
               }

            }else{
                painter->drawLine(4+4+getWidth()-8, 4+4+getHeight()-8, 4+8, 4+8);
            }

            painter->setPen(Qt::NoPen);

            painter->drawPixmap(4,4,16,16, QPixmap(actionIcon));

            if (!isInvalid()){
                painter->setBrush(QBrush(QColor("#000000")));
            }else{
                painter->setBrush(QBrush(QColor(255,0,0,75)));
                painter->drawEllipse(3,3,18,18);
            }
        }
    }
}

void QnstGraphicsAction::delineate(QPainterPath* painter) const
{
    if (getEntityA() != NULL && getEntityB() != NULL){
        QLineF line = QLineF(QPointF(getEntityA()->getLeft() + getEntityA()->getWidth()/2,
                                     getEntityA()->getTop() + getEntityA()->getHeight()/2),
                             QPointF(getEntityB()->getLeft() + getEntityB()->getWidth()/2,
                                     getEntityB()->getTop() + getEntityB()->getHeight()/2));

        if (getEntityA()->getncgType() == Qncg::Interface){
            line.setP1(getnstGraphicsParent()->mapFromItem(getEntityA()->getnstGraphicsParent(), line.p1()));
        }

        if (getEntityB()->getncgType() == Qncg::Interface){
            line.setP2(getnstGraphicsParent()->mapFromItem(getEntityB()->getnstGraphicsParent(), line.p2()));
        }

        QPointF pointa = line.p1();
        QPointF pointb = line.p2();

        if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){

            painter->addEllipse(4+getWidth()-16, 4+getHeight()-16, 16, 16);

        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){

            painter->addEllipse(4, 4+getHeight()-16, 16, 16);

        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){

            painter->addEllipse(4+getWidth()-16,4,16,16);

        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){

            painter->addEllipse(4,4,16,16);
        }
    }
}

void QnstGraphicsAction::setNameUids(QMap<QString, QString> nameUids)
{
    this->name_uid = nameUids;
}

void QnstGraphicsAction::addParam(QString uid, QString name, QString value)
{
    params[name] = value;
    name_uid[name] = uid;

    emit bindParamUpdated(getnstUid() ,params, name_uid);
}

void QnstGraphicsAction::setParam(QString name, QString value)
{
    params[name] = value;

    emit bindParamUpdated(getnstUid() ,params, name_uid);
}

void QnstGraphicsAction::removeUId(QString uid)
{
    QString name = name_uid.key(uid);

    if (params.contains(name)){
        params.remove(name);
        name_uid.remove(name);

        emit bindParamUpdated(getnstUid() ,params, name_uid);
    }
}

void QnstGraphicsAction::removeParam(QString name)
{
    params.remove(name);
    name_uid.remove(name);

    emit bindParamUpdated(getnstUid() ,params, name_uid);
}

void QnstGraphicsAction::setConn(QnstConnector* conn)
{
    this->conn = conn;
}

void QnstGraphicsAction::setParams(QMap<QString, QString> params)
{
    this->params = params;
}

void QnstGraphicsAction::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (conn != NULL){
        QList<QString> names = conn->getParams().values();

        QMap<QString, QString> values;

        foreach(QString name, names){
            values[name] = params[name];

            if (!name_uid.contains(name)){

                name_uid[name] = QUuid::createUuid().toString();
            }
        }

        dialog->init(values);

        if (dialog->exec()){
            params = dialog->getProperties();

            foreach(QString name, params.keys()){
                QMap<QString, QString> p;

                p["name"] = name;
                p["value"] = params[name];

                emit bindParamAdded(name_uid[name], getnstUid(), p);
            }

            emit bindParamUpdated(getnstUid() ,params, name_uid);
        }
    }
}
