/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "qnstport.h"

QnstPort::QnstPort(QnstNode* parent)
    : QnstInterface(parent)
{
    setnstType(Qnst::Port);
}

QnstPort::~QnstPort()
{

}

void QnstPort::link(QGraphicsSceneMouseEvent* event)
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

                if (bentity->parentItem() != NULL &&
                    eentity->parentItem() != NULL){

                    if (bentity->parentItem() == eentity->parentItem()){
                        QnstPort* bport = (QnstPort*) bentity;
                        QnstNode* enode = (QnstNode*) eentity;

                        QnstPortReference* edge = new QnstPortReference();
                        edge->setParentItem(parentItem());
                        edge->setnBegin(bport);
                        edge->setnEnd(enode);
                        edge->adjust();

                        bport->addBeginningEdge(edge);
                        enode->addEndingEdge(edge);
                    }
                }

            }else if (bentity->getEntityType() == Qncg::Interface &&
                      eentity->getEntityType() == Qncg::Interface){

                if (bentity->parentItem() == eentity->parentItem()->parentItem()){
                    QnstPort* bport = (QnstPort*) bentity;
                    QnstPort* eport = (QnstPort*) eentity;

                    QnstPortReference* edge = new QnstPortReference();
                    edge->setParentItem(parentItem());
                    edge->setnBegin(bport);
                    edge->setnEnd(eport);
                    edge->adjust();

                    bport->addBeginningEdge(edge);
                    eport->addEndingEdge(edge);

                }else{
                    if (bentity->parentItem()->parentItem() ==
                            eentity->parentItem()->parentItem()){

                        QnstPort* bport = (QnstPort*) bentity;
                        QnstPort* eport = (QnstPort*) eentity;

                        QnstEdge* edge = new QnstEdge();
                        edge->setParentItem(parentItem());
                        edge->setnBegin(bport);
                        edge->setnEnd(eport);
                        edge->adjust();

                        bport->addBeginningEdge(edge);
                        eport->addEndingEdge(edge);
                    }
                }

            }
        }
    }
}
