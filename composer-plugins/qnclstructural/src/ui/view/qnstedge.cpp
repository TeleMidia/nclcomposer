/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */

#include "math.h"

#include "qnstedge.h"

QnstEdge::QnstEdge(QncgEntity* parent)
    : QncgEdge(parent)
{
    setResizable(false);
    setSelectable(false);
    setbSpan(16);
    seteSpan(16);

    setnstType(Qnst::Link);

    setActionType(Qnst::NoAction);
    setConditionType(Qnst::NoCondition);
}

QnstEdge::~QnstEdge()
{

}

void QnstEdge::setConditionType(int contitiontype)
{
    switch(contitiontype){
        case Qnst::OnBegin:{
            conditionimage = ":/images/start";

            break;
        }

        case Qnst::OnSelection:{
            conditionimage = ":/images/selection";

            break;
        }

        case Qnst::OnEnd:{
            conditionimage = ":/images/stop";

            break;
        }

        case Qnst::OnPause:{
            conditionimage = ":/images/pause";

            break;
        }

        case Qnst::OnSet:{
            conditionimage = ":/images/set";

            break;
        }

        case Qnst::OnResume:{
            conditionimage = ":/images/resume";

            break;
        }

        case Qnst::OnAbort:{
            conditionimage = ":/images/abort";

            break;
        }

        case Qnst::NoCondition:{
            conditionimage = "";

            break;
        }
    }

    this->conditiontype = contitiontype;
}

int QnstEdge::getConditionType()
{
    return conditiontype;
}

void QnstEdge::setActionType(int actiontype)
{
    switch(actiontype){
        case Qnst::Start:{
            actionimage = ":/images/start";

            break;
        }

        case Qnst::Stop:{
            actionimage = ":/images/stop";

            break;
        }

        case Qnst::Pause:{
            actionimage = ":/images/pause";

            break;
        }

        case Qnst::Set:{
            actionimage = ":/images/set";

            break;
        }

        case Qnst::Resume:{
            actionimage = ":/images/resume";

            break;
        }

        case Qnst::Abort:{
            actionimage = ":/images/abort";

            break;
        }

        case Qnst::NoAction:{
            actionimage = "";

            break;
        }
    }


    this->actiontype = actiontype;
}

int QnstEdge::getActionType()
{
    return actiontype;
}

void QnstEdge::draw(QPainter* painter)
{
    qreal PI = 3.1415926535897932;

    QPointF p1;

    if (getpBegin().x() <= getpEnd().x() && getpBegin().y() <= getpEnd().y()){
        QSvgRenderer svgbegin(conditionimage);
        QSvgRenderer svgend(actionimage);

        QPixmap ibegin(32,32);
        QPixmap iend(32,32);

        ibegin.setMask(ibegin.mask().createMaskFromColor(QColor(255,255,255),Qt::MaskInColor));
        ibegin.fill(QColor(0,0,0,0));

        iend.setMask(iend.mask().createMaskFromColor(QColor(255,255,255),Qt::MaskInColor));
        iend.fill(QColor(0,0,0,0));

        QPainter ipbegin(&ibegin);
        QPainter ipend(&iend);

        svgbegin.render(&ipbegin,QRectF(0,0,32,32));
        svgend.render(&ipend,QRectF(0,0,32,32));

        if (getAngle() != 0){

            qreal drawangle = getAdjustedAngle();

            QLineF line(getpBegin(),getpEnd());

            if (drawangle < 0){
                drawangle = -drawangle;
            }

            qreal R = line.length()/(::sin(((drawangle/2)*PI)/180)*2);

            qreal delta = (180-drawangle)/2 + (360 - line.angle());


            QPointF rp((getWidth()-16) - ::sin((delta-90+drawangle)*PI/180)*R,
                       16 + ::sin((180-delta)*PI/180)*R);

            QPointF rpl(16 + ::sin((delta-90+drawangle)*PI/180)*R,
                        (getHeight()-16) - ::sin((180-delta)*PI/180)*R);

            if (getAdjustedAngle() < 0){
                painter->drawArc(rpl.x()-R,rpl.y()-R,2*R,2*R,
                                 16*((180-delta-drawangle)+180),16*drawangle);
            }else{
                painter->drawArc(rp.x()-R,rp.y()-R,2*R,2*R
                                 ,16*(180-delta-drawangle),16*drawangle);
            }
        }else{
            painter->drawLine(16,16,getWidth()-16,getHeight()-16);
        }

        painter->drawPixmap(0,0,32,32,ibegin);
        painter->drawPixmap(getWidth()-32,getHeight()-32,32,32,iend);

        p1.setX(getWidth()-16);
        p1.setY(getHeight()-16);

    }else if (getpBegin().x() > getpEnd().x() && getpBegin().y() < getpEnd().y()){
        painter->drawLine(getWidth()-16,16,16,getHeight()-16);

        QSvgRenderer svgbegin(conditionimage);
        QSvgRenderer svgend(actionimage);

        QPixmap ibegin(32,32);
        QPixmap iend(32,32);

        ibegin.setMask(ibegin.mask().createMaskFromColor(QColor(255,255,255),Qt::MaskInColor));
        ibegin.fill(QColor(0,0,0,0));

        iend.setMask(iend.mask().createMaskFromColor(QColor(255,255,255),Qt::MaskInColor));
        iend.fill(QColor(0,0,0,0));

        QPainter ipbegin(&ibegin);
        QPainter ipend(&iend);

        svgbegin.render(&ipbegin,QRectF(0,0,32,32));
        svgend.render(&ipend,QRectF(0,0,32,32));

        if (getAdjustedAngle() != 0){

        }else{

        }

        painter->drawPixmap(getWidth()-32,0,32,32,ibegin);
        painter->drawPixmap(0,getHeight()-32,32,32,iend);

        p1.setX(16);
        p1.setY(getHeight()-16);

    }else if (getpBegin().x() < getpEnd().x() && getpBegin().y() > getpEnd().y()){
        painter->drawLine(16,getHeight()-16,getWidth()-16,16);

        QSvgRenderer svgbegin(conditionimage);
        QSvgRenderer svgend(actionimage);

        QPixmap ibegin(32,32);
        QPixmap iend(32,32);

        ibegin.setMask(ibegin.mask().createMaskFromColor(QColor(255,255,255),Qt::MaskInColor));
        ibegin.fill(QColor(0,0,0,0));

        iend.setMask(iend.mask().createMaskFromColor(QColor(255,255,255),Qt::MaskInColor));
        iend.fill(QColor(0,0,0,0));

        QPainter ipbegin(&ibegin);
        QPainter ipend(&iend);

        svgbegin.render(&ipbegin,QRectF(0,0,32,32));
        svgend.render(&ipend,QRectF(0,0,32,32));

        if (getAdjustedAngle() != 0){

        }else{

        }

        painter->drawPixmap(0,getHeight()-32,32,32,ibegin);
        painter->drawPixmap(getWidth()-32,0,32,32,iend);

        p1.setX(getWidth()-16);
        p1.setY(16);

    }else if (getpBegin().x() > getpEnd().x() && getpBegin().y() > getpEnd().y()){
        painter->drawLine(getWidth()-16,getHeight()-16,16,16);

        QSvgRenderer svgbegin(conditionimage);
        QSvgRenderer svgend(actionimage);

        QPixmap ibegin(32,32);
        QPixmap iend(32,32);

        ibegin.setMask(ibegin.mask().createMaskFromColor(QColor(255,255,255),Qt::MaskInColor));
        ibegin.fill(QColor(0,0,0,0));

        iend.setMask(iend.mask().createMaskFromColor(QColor(255,255,255),Qt::MaskInColor));
        iend.fill(QColor(0,0,0,0));

        QPainter ipbegin(&ibegin);
        QPainter ipend(&iend);

        svgbegin.render(&ipbegin,QRectF(0,0,32,32));
        svgend.render(&ipend,QRectF(0,0,32,32));

        if (getAdjustedAngle() != 0){

        }else{

        }

        painter->drawPixmap(getWidth()-32,getHeight()-32,32,32,ibegin);
        painter->drawPixmap(0,0,32,32,iend);

        p1.setX(16);
        p1.setY(16);
    }

    QLineF myline(getpBegin(),getpEnd());

    QVector<QPointF> v;

    double angle = ::acos(myline.dx() / myline.length());

    if (getAngle() != 0){
        angle += (90 - (180-getAdjustedAngle())/2)*PI/180;
    }

    if (myline.dy() >= 0){
         angle = (PI * 2) - angle;
    }

    if (getAngle() != 0) {

    }else{
        qreal alfa = myline.angle() - 180 - 90;

        p1.setX(p1.x() - ::sin(alfa*PI/180) * 16);
        p1.setY(p1.y() - ::cos(alfa*PI/180) * 16);
    }

    QPointF p2 = p1 - QPointF(sin(angle + PI / 3) * 12,
                              cos(angle + PI / 3) * 12);
    QPointF p3 = p1 - QPointF(sin(angle + PI - PI / 3) * 12,
                              cos(angle + PI - PI / 3) * 12);

    painter->setBrush(Qt::black);
    painter->setPen(Qt::NoPen);

    v.append(p1);
    v.append(p2);
    v.append(p3);

    QPolygonF p(v);

    painter->drawPolygon(p);
}

void QnstEdge::delineate(QPainterPath* painter) const
{
    if (getpBegin().x() <= getpEnd().x() && getpBegin().y() <= getpEnd().y()){
        painter->addEllipse(0,0,32,32);
        painter->addEllipse(getWidth()-32,getHeight()-32,32,32);

    }else if (getpBegin().x() > getpEnd().x() && getpBegin().y() < getpEnd().y()){
        painter->addEllipse(getWidth()-32,0,32,32);
        painter->addEllipse(0,getHeight()-32,32,32);

    }else if (getpBegin().x() < getpEnd().x() && getpBegin().y() > getpEnd().y()){
        painter->addEllipse(0,getHeight()-32,32,32);
        painter->addEllipse(getWidth()-32,0,32,32);

    }else if (getpBegin().x() > getpEnd().x() && getpBegin().y() > getpEnd().y()){
        painter->addEllipse(getWidth()-32,getHeight()-32,32,32);
        painter->addEllipse(0,0,32,32);

    }
}

