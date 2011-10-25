/*
 * Copyright 2011 TeleMidia/PUC-Rio.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>. 
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

    createActions();
    createMenus();
    createConnections();
}

QnstEdge::~QnstEdge()
{

}

void QnstEdge::deleteEntity()
{
    QnstEntity* bn;
    QnstEntity* en;

    if (getnBegin()->getEntityType() == Qncg::Node){
        bn = ((QnstNode*) getnBegin());
    }else{
        bn = ((QnstInterface*) getnBegin());
    }

    if (getnEnd()->getEntityType() == Qncg::Node){
        en = ((QnstNode*) getnEnd());
    }else{
        en = ((QnstInterface*) getnEnd());
    }

    if (getnBegin()->getEntityType() == Qncg::Node){
        ((QnstNode*) bn)->removeAngle(en->getUid(), getAngle());
        ((QnstNode*) bn)->removeBeginningEdge(this);
    }else{
        ((QnstInterface*) bn)->removeAngle(en->getUid(), getAngle());
        ((QnstInterface*) bn)->removeBeginningEdge(this);
    }

    if (getnEnd()->getEntityType() == Qncg::Node){
        ((QnstNode*) en)->removeAngle(bn->getUid(), -getAngle());
        ((QnstNode*) en)->removeEndingEdge(this);
    }else{
        ((QnstInterface*) en)->removeAngle(bn->getUid(), -getAngle());
        ((QnstInterface*) en)->removeEndingEdge(this);
    }

    emit entityRemoved(this);

    scene()->removeItem(this);
}

void QnstEdge::createActions()
{
    // help action
    helpAction = new QAction(this);
    helpAction->setText(tr("Help"));

    helpAction->setEnabled(true);
    helpAction->setShortcut(QKeySequence("F1"));

    // undo action
    undoAction = new QAction(this);
    undoAction->setText(tr("Undo"));

    undoAction->setEnabled(false);
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));

    // redo action
    redoAction = new QAction(this);
    redoAction->setText(tr("Redo"));

    redoAction->setEnabled(false);
    redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));

    // cut action
    cutAction = new QAction(this);
    cutAction->setText(tr("Cut"));

    cutAction->setEnabled(false);
    cutAction->setShortcut(QKeySequence("Ctrl+X"));

    // copy action
    copyAction = new QAction(this);
    copyAction->setText(tr("Copy"));

    copyAction->setEnabled(false);
    copyAction->setShortcut(QKeySequence("Ctrl+C"));

    // paste action
    pasteAction = new QAction(this);
    pasteAction->setText(tr("Paste"));

    pasteAction->setEnabled(false);
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));

    // delete action
    deleteAction = new QAction(this);
    deleteAction->setText(tr("Delete"));

    deleteAction->setEnabled(true);
    deleteAction->setShortcut(QKeySequence("Del"));

    // zoomin action
    zoominAction = new QAction(this);
    zoominAction->setText(tr("Zoom In"));

    zoominAction->setEnabled(true);
    zoominAction->setShortcut(QKeySequence("Ctrl++"));

    // zoomout action
    zoomoutAction = new QAction(this);
    zoomoutAction->setText(tr("Zoom Out"));

    zoomoutAction->setEnabled(true);
    zoomoutAction->setShortcut(QKeySequence("Ctrl+-"));

    // reset action
    zoomresetAction = new QAction(this);
    zoomresetAction->setText(tr("Reset"));

    zoomresetAction->setEnabled(true);
    zoomresetAction->setShortcut(QKeySequence("Ctrl+0"));

    // fullscreen action
    fullscreenAction = new QAction(this);
    fullscreenAction->setText(tr("Full Screen"));

    fullscreenAction->setEnabled(true);
    fullscreenAction->setShortcut(QKeySequence("Ctrl+F"));

    // export action
    exportAction = new QAction(this);
    exportAction->setText(tr("Export..."));

    exportAction->setEnabled(true);

    // bring to front action
    bringfrontAction = new QAction(this);
    bringfrontAction->setText(tr("Bring to Front"));

    bringfrontAction->setEnabled(false);
    bringfrontAction->setShortcut(QKeySequence("Shift+Ctrl+]"));

    // bring forward action
    bringforwardAction = new QAction(this);
    bringforwardAction->setText(tr("Bring Forward"));

    bringforwardAction->setEnabled(false);
    bringforwardAction->setShortcut(QKeySequence("Ctrl+]"));

    // send backward action
    sendbackwardAction = new QAction(this);
    sendbackwardAction->setText(tr("Send Backward"));

    sendbackwardAction->setEnabled(false);
    sendbackwardAction->setShortcut(QKeySequence("Ctrl+["));

    // send to back action
    sendbackAction = new QAction(this);
    sendbackAction->setText(tr("Send to Back"));

    sendbackAction->setEnabled(false);
    sendbackAction->setShortcut(QKeySequence("Shift+Ctrl+["));

    // hide action
    hideAction = new QAction(this);
    hideAction->setText(tr("Hide"));

    hideAction->setEnabled(false);

    // properties action
    propertiesAction = new QAction(this);
    propertiesAction->setText(tr("Properties"));

    propertiesAction->setEnabled(true);
}

void QnstEdge::createMenus()
{
    // view menu
    viewMenu = new QMenu();
    viewMenu->setTitle(tr("View"));

    viewMenu->setEnabled(true);

    viewMenu->addAction(zoominAction);
    viewMenu->addAction(zoomoutAction);
    viewMenu->addAction(zoomresetAction);
    viewMenu->addSeparator();
    viewMenu->addAction(fullscreenAction);

    // show menu
    showMenu = new QMenu();
    showMenu->setTitle(tr("Show"));

    showMenu->setEnabled(true);

    // arrange menu
    arrangeMenu = new QMenu();
    arrangeMenu->setTitle(tr("Arrange"));

    arrangeMenu->setEnabled(false);

    arrangeMenu->addAction(bringfrontAction);
    arrangeMenu->addAction(bringforwardAction);
    arrangeMenu->addAction(sendbackwardAction);
    arrangeMenu->addAction(sendbackAction);

    // context menu
    contextMenu = new QMenu();
    contextMenu->addAction(helpAction);
    contextMenu->addSeparator();
    contextMenu->addAction(undoAction);
    contextMenu->addAction(redoAction);
    contextMenu->addSeparator();
    contextMenu->addAction(cutAction);
    contextMenu->addAction(copyAction);
    contextMenu->addAction(pasteAction);
    contextMenu->addSeparator();
    contextMenu->addAction(deleteAction);
    contextMenu->addSeparator();
    contextMenu->addAction(exportAction);
    contextMenu->addSeparator();
    contextMenu->addMenu(viewMenu);
    contextMenu->addMenu(showMenu);
    contextMenu->addMenu(arrangeMenu);
    contextMenu->addSeparator();
    contextMenu->addAction(hideAction);
    contextMenu->addSeparator();
    contextMenu->addAction(propertiesAction);
}

void QnstEdge::createConnections()
{
    connect(deleteAction, SIGNAL(triggered()), SLOT(deleteEntity()));
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

void QnstEdge::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QncgEdge::contextMenuEvent(event);

    if (!event->isAccepted())
    {
        contextMenu->exec(event->screenPos());

        event->accept();
    }
}

