/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "qnstarea.h"

QnstArea::QnstArea(QnstNode* parent)
    : QnstInterface(parent)
{
    setnstType(Qnst::Area);
}

QnstArea::~QnstArea()
{

}

void QnstArea::draw(QPainter* painter)
{
    painter->setBrush(QBrush("#EEEEEE"));

    if (isSelected()){

        painter->setPen(QPen(QBrush(Qt::black), 0, Qt::DashLine));
        painter->drawEllipse(4, 4, getWidth()-1, getHeight()-1);
    }else{
        painter->drawEllipse(4, 4, getWidth()-1, getHeight()-1);
    }

    if (isMoving()){
        if (parentItem() != NULL){
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(QBrush(Qt::black), 0));// 0px = cosmetic border

            painter->drawEllipse(getMoveLeft()+4-getLeft(),
                                 getMoveTop()+4-getTop(),
                                 getWidth()-1,
                                 getHeight()-1);
        }else{
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(QBrush(Qt::black), 0));// 0px = cosmetic border

            painter->drawEllipse(getMoveLeft()+4-getLeft(),
                              getMoveTop()+4-getTop(),
                              getWidth()-1,
                              getHeight()-1);
        }

    }else if (isResizing()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawEllipse(getResizeLeft()+4-getLeft(),
                             getResizeTop()+4-getTop(),
                             getResizeWidth()-1,
                             getResizeHeight()-1);
    }
}

void QnstArea::delineate(QPainterPath* painter) const
{
    painter->addEllipse(4, 4, getWidth()-1, getHeight()-1);
}

void QnstArea::link(QGraphicsSceneMouseEvent* event)
{
    QList<QGraphicsItem *> bitems = scene()->items(getSceneEdge()->line().p1());

    if (bitems.count() && bitems.first() == getSceneEdge()){
        bitems.removeFirst();
    }

    QList<QGraphicsItem *> eitems = scene()->items(getSceneEdge()->line().p2());

    if (eitems.count() && eitems.first() == getSceneEdge()){
        eitems.removeFirst();
    }

    if (bitems.count() && eitems.count()){
        QncgEntity* bentity = (QncgEntity*) bitems.first();
        QncgEntity* eentity = (QncgEntity*) eitems.first();

        if (bentity != eentity){
            if (bentity->getEntityType() == Qncg::Interface &&
                eentity->getEntityType() == Qncg::Node){

                if (bentity->parentItem()->parentItem() != NULL &&
                    eentity->parentItem() != NULL){

                    if (bentity->parentItem()->parentItem() ==
                        eentity->parentItem()){

                        QnstArea* barea = (QnstArea*) bentity;
                        QnstNode* enode = (QnstNode*) eentity;

                        QDialog* d = new QDialog();
                        d->setWindowModality(Qt::ApplicationModal);

                        Ui::QnstDBind dbind;
                        dbind.setupUi(d);

                        d->exec();
                        if (d->result() == QDialog::Accepted){

                            QnstEdge* edge = new QnstEdge();
                            edge->setParentItem(parentItem()->parentItem());
                            edge->setnstParent(getnstParent()->getnstParent());
                            edge->setnBegin(barea);
                            edge->setnEnd(enode);

                            int inv = -1;

                            qreal step = 45;

                            qreal angle = 0;

                            if (getAngles().contains(enode->getUid())){
                                while (getAngles().value(enode->getUid()).contains(angle)){

                                    if (inv > 0){
                                        angle *= -1;
                                        angle += step;
                                    }else{
                                        angle *= inv;
                                    }

                                    inv *= -1;
                                }

                                addAngle(enode->getUid(), angle);

                                enode->addAngle(getUid(),-angle);

                            }else{
                                addAngle(enode->getUid(), angle);

                                enode->addAngle(getUid(), angle);
                            }

                            edge->setAngle(angle);

                            if (dbind.ccondition->currentText() == "none"){
                                edge->setConditionType(Qnst::NoCondition);

                            }else if (dbind.ccondition->currentText() == "onBegin"){
                                edge->setConditionType(Qnst::OnBegin);

                            }else if (dbind.ccondition->currentText() == "onEnd"){
                                edge->setConditionType(Qnst::OnEnd);

                            }else if (dbind.ccondition->currentText() == "onPause"){
                                edge->setConditionType(Qnst::OnPause);

                            }else if (dbind.ccondition->currentText() == "onResume"){
                                edge->setConditionType(Qnst::OnResume);

                            }else if (dbind.ccondition->currentText() == "onSelection"){
                                edge->setConditionType(Qnst::OnSelection);

                            }else if (dbind.ccondition->currentText() == "onSet"){
                                edge->setConditionType(Qnst::OnSet);

                            }else if (dbind.ccondition->currentText() == "onAbort"){
                                edge->setConditionType(Qnst::OnAbort);

                            }else if (dbind.ccondition->currentText() == "onKeySelection"){
                                edge->setConditionType(Qnst::OnKeySelection);
                            }


                            if (dbind.caction->currentText() == "none"){
                                edge->setActionType(Qnst::NoAction);

                            }else if (dbind.caction->currentText() == "start"){
                                edge->setActionType(Qnst::Start);

                            }else if (dbind.caction->currentText() == "stop"){
                                edge->setActionType(Qnst::Stop);

                            }else if (dbind.caction->currentText() == "pause"){
                                edge->setActionType(Qnst::Pause);

                            }else if (dbind.caction->currentText() == "resume"){
                                edge->setActionType(Qnst::Resume);

                            }else if (dbind.caction->currentText() == "abort"){
                                edge->setActionType(Qnst::Abort);

                            }else if (dbind.caction->currentText() == "set"){
                                edge->setActionType(Qnst::Set);

                            }else if (dbind.caction->currentText() == "startDelay"){
                                edge->setActionType(Qnst::StartDelay);
                            }

                            edge->adjust();

                            delete d;

                            barea->addBeginningEdge(edge);
                            enode->addEndingEdge(edge);

                            emit entityAdded(edge);

                        }
                    }
                }

            }else if (bentity->getEntityType() == Qncg::Interface &&
                      eentity->getEntityType() == Qncg::Interface){

                if (bentity->parentItem()->parentItem() != NULL &&
                    eentity->parentItem()->parentItem() != NULL){

                    if (bentity->parentItem()->parentItem() ==
                        eentity->parentItem()->parentItem()){

                        QnstArea* barea = (QnstArea*) bentity;
                        QnstInterface* einterface = (QnstInterface*) eentity;

                        QDialog* d = new QDialog();
                        d->setWindowModality(Qt::ApplicationModal);

                        Ui::QnstDBind dbind;
                        dbind.setupUi(d);

                        d->exec();
                        if (d->result() == QDialog::Accepted){

                            QnstEdge* edge = new QnstEdge();
                            edge->setParentItem(parentItem()->parentItem());
                            edge->setnstParent(getnstParent()->getnstParent());
                            edge->setnBegin(barea);
                            edge->setnEnd(einterface);

                            int inv = -1;

                            qreal step = 45;

                            qreal angle = 0;

                            if (getAngles().contains(einterface->getUid())){
                                while (getAngles().value(einterface->getUid()).contains(angle)){

                                    if (inv > 0){
                                        angle *= -1;
                                        angle += step;
                                    }else{
                                        angle *= inv;
                                    }

                                    inv *= -1;
                                }

                                addAngle(einterface->getUid(), angle);

                                einterface->addAngle(getUid(),-angle);

                            }else{
                                addAngle(einterface->getUid(), angle);

                                einterface->addAngle(getUid(), angle);
                            }

                            edge->setAngle(angle);

                            if (dbind.ccondition->currentText() == "none"){
                                edge->setConditionType(Qnst::NoCondition);

                            }else if (dbind.ccondition->currentText() == "onBegin"){
                                edge->setConditionType(Qnst::OnBegin);

                            }else if (dbind.ccondition->currentText() == "onEnd"){
                                edge->setConditionType(Qnst::OnEnd);

                            }else if (dbind.ccondition->currentText() == "onPause"){
                                edge->setConditionType(Qnst::OnPause);

                            }else if (dbind.ccondition->currentText() == "onResume"){
                                edge->setConditionType(Qnst::OnResume);

                            }else if (dbind.ccondition->currentText() == "onSelection"){
                                edge->setConditionType(Qnst::OnSelection);

                            }else if (dbind.ccondition->currentText() == "onSet"){
                                edge->setConditionType(Qnst::OnSet);

                            }else if (dbind.ccondition->currentText() == "onAbort"){
                                edge->setConditionType(Qnst::OnAbort);

                            }else if (dbind.ccondition->currentText() == "onKeySelection"){
                                edge->setConditionType(Qnst::OnKeySelection);
                            }


                            if (dbind.caction->currentText() == "none"){
                                edge->setActionType(Qnst::NoAction);

                            }else if (dbind.caction->currentText() == "start"){
                                edge->setActionType(Qnst::Start);

                            }else if (dbind.caction->currentText() == "stop"){
                                edge->setActionType(Qnst::Stop);

                            }else if (dbind.caction->currentText() == "pause"){
                                edge->setActionType(Qnst::Pause);

                            }else if (dbind.caction->currentText() == "resume"){
                                edge->setActionType(Qnst::Resume);

                            }else if (dbind.caction->currentText() == "abort"){
                                edge->setActionType(Qnst::Abort);

                            }else if (dbind.caction->currentText() == "set"){
                                edge->setActionType(Qnst::Set);

                            }else if (dbind.caction->currentText() == "startDelay"){
                                edge->setActionType(Qnst::StartDelay);
                            }

                            edge->adjust();

                            delete d;

                            barea->addBeginningEdge(edge);
                            einterface->addEndingEdge(edge);

                            emit entityAdded(edge);

                        }
                    }
                }
            }


        }
    }
}
