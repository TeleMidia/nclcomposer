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

        case Qnst::OnKeySelection:{
            conditionimage = ":/images/selection";

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

        case Qnst::StartDelay:{
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

            qreal peta = line.angle() - 180 + (180-drawangle)/2;

            qreal gama = 180 - peta - drawangle;

            QPointF rpl(12 + ::cos((gama)*PI/180)*R,  (getHeight()-12) + ::sin((gama)*PI/180)*R);

            QPointF rp((getWidth()-12) - ::cos((gama)*PI/180)*R,  12 - ::sin((gama)*PI/180)*R);

            if (getAdjustedAngle() < 0){
                painter->drawArc(rpl.x()-R,rpl.y()-R,2*R,2*R,16*(peta),16*drawangle);
            }else{
                painter->drawArc(rp.x()-R,rp.y()-R,2*R,2*R,16*(peta+180),16*drawangle);
            }
        }else{
             painter->drawLine(getWidth()-16,16,16,getHeight()-16);
        }

        painter->drawPixmap(getWidth()-32,0,32,32,ibegin);
        painter->drawPixmap(0,getHeight()-32,32,32,iend);

        p1.setX(16);
        p1.setY(getHeight()-16);

    }else if (getpBegin().x() < getpEnd().x() && getpBegin().y() > getpEnd().y()){
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

            qreal peta = 180 - line.angle() - (180-drawangle)/2;

            qreal gama = peta - drawangle;

            QPointF rp(12 + ::cos((gama)*PI/180)*R,  (getHeight()-12) + ::sin((gama)*PI/180)*R);

            QPointF rpl((getWidth()-12) - ::cos((gama)*PI/180)*R,  12 - ::sin((gama)*PI/180)*R);

            if (getAdjustedAngle() < 0){
                painter->drawArc(rpl.x()-R,rpl.y()-R,2*R,2*R,16*((180-peta)+180),16*drawangle);
            }else{
                painter->drawArc(rp.x()-R,rp.y()-R,2*R,2*R,16*(180-peta),16*drawangle);
            }
        }else{
            painter->drawLine(16,getHeight()-16,getWidth()-16,16);
        }

        painter->drawPixmap(0,getHeight()-32,32,32,ibegin);
        painter->drawPixmap(getWidth()-32,0,32,32,iend);

        p1.setX(getWidth()-16);
        p1.setY(16);

    }else if (getpBegin().x() > getpEnd().x() && getpBegin().y() > getpEnd().y()){


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

            if (getAngle() < 0){
                drawangle = -drawangle;
            }

            qreal R = line.length()/(::sin(((drawangle/2)*PI)/180)*2);

            qreal angulotenso = (180 - line.angle() + (180-drawangle)/2);

            qreal gama = 180-angulotenso-drawangle;

            QPointF rpl((getWidth()-12) - ::cos((gama)*PI/180)*R,
                        (getHeight()-12) + ::sin((gama)*PI/180)*R);

            QPointF rp(12 + ::cos((gama)*PI/180)*R, 12 - ::sin((gama)*PI/180)*R);

            if (getAdjustedAngle() < 0){
                painter->drawArc(rpl.x()-R,rpl.y()-R,2*R,2*R,16*((180-angulotenso-drawangle)),16*drawangle);
            }else{
                painter->drawArc(rp.x()-R,rp.y()-R,2*R,2*R,16*((180-angulotenso-drawangle)+180),16*drawangle);
            }

        }else{
             painter->drawLine(getWidth()-16,getHeight()-16,16,16);
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
/*        qreal alfa = getAdjustedAngle();

        qreal beta = (180 - alfa)/2 + (360 - myline.angle());

        qreal R = myline.length()/(::sin(((alfa/2)*PI)/180)*2);

        qreal arc_len = alfa*PI*R/180;

        QPointF center_p(myline.p2().x() - ::sin((beta+alfa-90)*PI/180)*R,
                         myline.p2().y() + ::sin((180-beta-alfa)*PI/180)*R);

        qreal gama = 2 * ((::asin(8/R) * 180)/PI);

        qreal gama_len = gama*PI*R/180;

        qreal new_arc_len = arc_len - gama_len;

        qreal new_alfa = (180*new_arc_len)/(PI*R);

        qreal peta = (180-beta-alfa);

        QPointF new_p(center_p.x() + ::cos((new_alfa+peta)*PI/180)*R,
                      center_p.y() - ::sin((new_alfa+peta)*PI/180)*R);

        p1 = new_p;*/

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

