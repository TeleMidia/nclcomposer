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
#include "qnstview.h"

QnstView::QnstView(QWidget* parent)
    : QGraphicsView(parent)
{
    scene = new QnstScene(this);
    setScene(scene);

    createConnection();

    selectedEntity = NULL;

    media_count = 0;
    link_count = 0;
    context_count = 0;
    switch_count = 0;
    port_count = 0;
    area_count = 0;
    property_count = 0;
}

QnstView::~QnstView()
{

}

QString QnstView::addEntity(const QString parentUID,
                  const QMap<QString, QString> attributes)
{

}

void QnstView::removeEntity(const QString entityUID)
{

}

void QnstView::changeEntity(const QString entityUID,
                  const QString parentUID,
                  const QMap<QString, QString> attributes)
{

}

void QnstView::selectEntity(const QString entityUID)
{
    qDebug() << "QnstView::selectEntity";

    if (entities.contains(entityUID)){
        QnstEntity *e = entities[entityUID];

        QncgEntity* nextSelected = NULL;

        if (e->getnstType() == Qnst::Body ||
            e->getnstType() == Qnst::Media ||
            e->getnstType() == Qnst::Switch ||
            e->getnstType() == Qnst::Context){

            nextSelected = (QnstNode*) e;
        }else if (e->getnstType() == Qnst::Port ||
                 e->getnstType() == Qnst::Property ||
                 e->getnstType() == Qnst::Area){

            nextSelected = (QnstInterface*) e;
        }

        if (nextSelected != NULL){

            if (selectedEntity != nextSelected){

                if (selectedEntity != NULL){
                    selectedEntity->setSelected(false);
                }
            }

            selectedEntity = nextSelected;
            selectedEntity->setSelected(true);

            centerOn(selectedEntity);

            scene->update();
        }
    }
}

void QnstView::requestEntityAddition(QnstEntity* e)
{
    entities[e->getUid()] = e;

    switch(e->getnstType()){
        case Qnst::Body:{
            QnstBody* b = (QnstBody*) e;

            QString pbUID = "";

            if (b->getnstParent() != NULL){
                QnstEntity* pe = b->getnstParent();

                pbUID = pe->getUid();
            }

            QString bUID = b->getUid();


            QMap<QString, QString> attrs;

            // todo: attrs

            emit bodyAdded(bUID, pbUID,attrs);

            centerOn(b);

            break;
        }

        case Qnst::Context:{
            QnstContext* c = (QnstContext*) e;

            QString pcUID = "";

            if (c->getnstParent() != NULL){
                QnstEntity* pe = c->getnstParent();

                qDebug() << pe->getUid();

                pcUID = pe->getUid();
            }

            QString cUID = c->getUid();

            context_count++;

            QString name = "ctx"+QString::number(context_count);

            c->setName(name);

            QMap<QString, QString> attrs;

            attrs["id"] = name;

            // todo: attrs

            emit contextAdded(cUID, pcUID,attrs);

            break;
        }

        case Qnst::Media:{
            QnstMedia* m = (QnstMedia*) e;

            QString pmUID = "";

            if (m->getnstParent() != NULL){
                QnstEntity* pe = m->getnstParent();

                pmUID = pe->getUid();
            }

            QString mUID = m->getUid();

            media_count++;

            QString name = "m"+QString::number(media_count);

            m->setName(name);

            QMap<QString, QString> attrs;

            attrs["id"] = name;

            // todo: attrs

            emit mediaAdded(mUID, pmUID,attrs);

            break;
        }

        case Qnst::Switch:{
            QnstSwitch* s = (QnstSwitch*) e;

            QString psUID = "";

            if (s->getnstParent() != NULL){
                QnstEntity* pe = s->getnstParent();

                psUID = pe->getUid();
            }

            QString sUID = s->getUid();

            switch_count++;

            QString name = "swt"+QString::number(switch_count);

            s->setName(name);

            QMap<QString, QString> attrs;

            attrs["id"] = name;

            // todo: attrs

            emit switchAdded(sUID, psUID,attrs);

            break;
        }

        case Qnst::Port:{
            QnstPort* s = (QnstPort*) e;

            QString psUID = "";

            if (s->getnstParent() != NULL){
                QnstEntity* pe = s->getnstParent();

                psUID = pe->getUid();
            }

            QString sUID = s->getUid();

            port_count++;

            QString name = "p"+QString::number(port_count);

            s->setName(name);

            QMap<QString, QString> attrs;

            attrs["id"] = name;

            // todo: attrs

            emit portAdded(sUID, psUID,attrs);

            break;
        }

        case Qnst::Property:{
            QnstProperty* s = (QnstProperty*) e;

            QString psUID = "";

            if (s->getnstParent() != NULL){
                QnstEntity* pe = s->getnstParent();

                psUID = pe->getUid();
            }

            QString sUID = s->getUid();

            property_count++;

            QString name = "prop"+QString::number(property_count);

            s->setName(name);

            QMap<QString, QString> attrs;

            // todo: attrs

            emit propertyAdded(sUID, psUID,attrs);

            break;
        }

        case Qnst::Area:{
            QnstArea* s = (QnstArea*) e;

            QString psUID = "";

            if (s->getnstParent() != NULL){
                QnstEntity* pe = s->getnstParent();

                psUID = pe->getUid();
            }

            QString sUID = s->getUid();

            area_count++;

            QString name = "a"+QString::number(area_count);

            s->setName(name);

            QMap<QString, QString> attrs;

            attrs["id"] = name;

            emit areaAdded(sUID, psUID,attrs);

            break;
        }

        case Qnst::Refer:{
            QnstEdge* s = (QnstEdge*) e;

            QString psUID = "";



            if (s->getnstParent() != NULL){
                QnstEntity* pe = s->getnstParent();

                psUID = pe->getUid();
            }

            QString sUID = s->getUid();

            link_count++;

            QString name = "link"+QString::number(link_count);

            s->setName(name);

            QMap<QString, QString> attrs;

            attrs["id"] = name;

            if (e->getnstType() == Qnst::Refer){
                attrs["type"] = "refer";
            }else{
                attrs["type"] = "link";
            }

            if (s->getnBegin()->getEntityType() == Qncg::Node){
                QnstNode* n = (QnstNode*) s->getnBegin();

                attrs["bnode"] = n->getUid();
            }

            if (s->getnEnd()->getEntityType() == Qncg::Node){
                QnstNode* n = (QnstNode*) s->getnEnd();

                attrs["enode"] = n->getUid();
            }

            if (s->getnBegin()->getEntityType() == Qncg::Interface){
                QnstInterface* n = (QnstInterface*) s->getnBegin();

                attrs["bnode"] = n->getnstParent()->getUid();
                attrs["binterface"] = n->getUid();
            }

            if (s->getnEnd()->getEntityType() == Qncg::Interface){
                QnstInterface* n = (QnstInterface*) s->getnEnd();

                attrs["enode"] = n->getnstParent()->getUid();
                attrs["einterface"] = n->getUid();
            }

            // todo: attrs
            switch(s->getConditionType()){
                case Qnst::OnBegin:{
                    attrs["condition"] = "onBegin";

                    break;
                }

                case Qnst::OnSelection:{
                    attrs["condition"] = "onSelection";

                    break;
                }

                case Qnst::OnEnd:{
                    attrs["condition"] = "onEnd";

                    break;
                }

                case Qnst::OnPause:{
                    attrs["condition"] = "onPause";

                    break;
                }

                case Qnst::OnSet:{
                    attrs["condition"] = "onSet";

                    break;
                }

                case Qnst::OnResume:{
                    attrs["condition"] = "onResume";

                    break;
                }

                case Qnst::OnAbort:{
                    attrs["condition"] = "onAbort";

                    break;
                }

                case Qnst::OnKeySelection:{
                    attrs["condition"] = "onKeySelection";
                    attrs["key"] = "";

                    break;
                }
            }

            switch(s->getActionType()){
                case Qnst::Start:{
                    attrs["action"] = "start";

                    break;
                }

                case Qnst::Stop:{
                    attrs["action"] = "stop";

                    break;
                }

                case Qnst::Pause:{
                    attrs["action"] = "pause";

                    break;
                }

                case Qnst::Set:{
                    attrs["action"] = "set";
                    attrs["value"] = "";

                    break;
                }

                case Qnst::Resume:{
                    attrs["action"] = "resume";

                    break;
                }

                case Qnst::Abort:{
                    attrs["action"] = "abort";

                    break;
                }

                case Qnst::StartDelay:{
                    attrs["action"] = "startDelay";
                    attrs["delay"] = "";

                    break;
                }
            }

            emit linkAdded(sUID, psUID,attrs);

            break;
        }

        case Qnst::Link:{
            QnstEdge* s = (QnstEdge*) e;

            QString psUID = "";



            if (s->getnstParent() != NULL){
                QnstEntity* pe = s->getnstParent();

                psUID = pe->getUid();
            }

            QString sUID = s->getUid();

            link_count++;

            QString name = "link"+QString::number(link_count);

            s->setName(name);

            QMap<QString, QString> attrs;

            attrs["id"] = name;

            if (e->getnstType() == Qnst::Refer){
                attrs["type"] = "refer";
            }else{
                attrs["type"] = "link";
            }

            if (s->getnBegin()->getEntityType() == Qncg::Node){
                QnstNode* n = (QnstNode*) s->getnBegin();

                attrs["bnode"] = n->getUid();
            }

            if (s->getnEnd()->getEntityType() == Qncg::Node){
                QnstNode* n = (QnstNode*) s->getnEnd();

                attrs["enode"] = n->getUid();
            }

            if (s->getnBegin()->getEntityType() == Qncg::Interface){
                QnstInterface* n = (QnstInterface*) s->getnBegin();

                attrs["bnode"] = n->getnstParent()->getUid();
                attrs["binterface"] = n->getUid();
            }

            if (s->getnEnd()->getEntityType() == Qncg::Interface){
                QnstInterface* n = (QnstInterface*) s->getnEnd();

                attrs["enode"] = n->getnstParent()->getUid();
                attrs["einterface"] = n->getUid();
            }

            // todo: attrs
            switch(s->getConditionType()){
                case Qnst::OnBegin:{
                    attrs["condition"] = "onBegin";

                    break;
                }

                case Qnst::OnSelection:{
                    attrs["condition"] = "onSelection";

                    break;
                }

                case Qnst::OnEnd:{
                    attrs["condition"] = "onEnd";

                    break;
                }

                case Qnst::OnPause:{
                    attrs["condition"] = "onPause";

                    break;
                }

                case Qnst::OnSet:{
                    attrs["condition"] = "onSet";

                    break;
                }

                case Qnst::OnResume:{
                    attrs["condition"] = "onResume";

                    break;
                }

                case Qnst::OnAbort:{
                    attrs["condition"] = "onAbort";

                    break;
                }

                case Qnst::OnKeySelection:{
                    attrs["condition"] = "onKeySelection";
                    attrs["key"] = "";

                    break;
                }
            }

            switch(s->getActionType()){
                case Qnst::Start:{
                    attrs["action"] = "start";

                    break;
                }

                case Qnst::Stop:{
                    attrs["action"] = "stop";

                    break;
                }

                case Qnst::Pause:{
                    attrs["action"] = "pause";

                    break;
                }

                case Qnst::Set:{
                    attrs["action"] = "set";
                    attrs["value"] = "";

                    break;
                }

                case Qnst::Resume:{
                    attrs["action"] = "resume";

                    break;
                }

                case Qnst::Abort:{
                    attrs["action"] = "abort";

                    break;
                }

                case Qnst::StartDelay:{
                    attrs["action"] = "startDelay";
                    attrs["delay"] = "";

                    break;
                }
            }

            emit linkAdded(sUID, psUID,attrs);

            break;
        }
    }
}

void QnstView::requestEntitySelection(QnstEntity* e)
{
    QncgEntity* nextSelected = NULL;

    if (e->getnstType() == Qnst::Body ||
        e->getnstType() == Qnst::Media ||
        e->getnstType() == Qnst::Switch ||
        e->getnstType() == Qnst::Context){

        nextSelected = (QnstNode*) e;
    }else if (e->getnstType() == Qnst::Port ||
             e->getnstType() == Qnst::Property ||
             e->getnstType() == Qnst::Area){

         nextSelected = (QnstInterface*) e;
     }

    if (selectedEntity != nextSelected){

        if (selectedEntity != NULL){
            selectedEntity->setSelected(false);
        }
    }

    selectedEntity = nextSelected;

    emit entitySelected(e->getUid());
}


void QnstView::requestEntityRemotion(QnstEntity* e)
{
    entities.remove(e->getUid());

    if (e->getnstType() != Qnst::Link){
        QncgEntity* entity;

        if (e->getnstType() == Qnst::Body ||
            e->getnstType() == Qnst::Media ||
            e->getnstType() == Qnst::Switch ||
            e->getnstType() == Qnst::Context){

            entity = (QnstNode*) e;

            foreach(QncgEdge* edge, ((QnstNode*) e)->getBeginningEdges()){
                ((QnstEdge*) edge)->deleteEntity();
            }

            foreach(QncgEdge* edge, ((QnstNode*) e)->getEndingEdges()){
                ((QnstEdge*) edge)->deleteEntity();
            }

        }else if (e->getnstType() == Qnst::Port ||
                  e->getnstType() == Qnst::Property ||
                  e->getnstType() == Qnst::Area){

            entity = (QnstInterface*) e;

            foreach(QncgEdge* edge, ((QnstInterface*) e)->getBeginningEdges()){
                ((QnstEdge*) edge)->deleteEntity();
            }

            foreach(QncgEdge* edge, ((QnstInterface*) e)->getEndingEdges()){
                ((QnstEdge*) edge)->deleteEntity();
            }
        }

        foreach (QGraphicsItem* c, entity->childItems()){
            QncgEntity* cncg = (QncgEntity*) c;

            if (cncg->getEntityType() == Qncg::Node){
                requestEntityRemotion((QnstNode*) cncg);

            }else if (cncg->getEntityType() == Qncg::Interface){
                requestEntityRemotion((QnstInterface*) cncg);
            }
        }
    }

    emit entityRemoved(e->getUid());
}

void QnstView::createConnection()
{
    connect(scene, SIGNAL(entityAdded(QnstEntity*)),
            SLOT(requestEntityAddition(QnstEntity*)));

    connect(scene, SIGNAL(entitySelected(QnstEntity*)),
            SLOT(requestEntitySelection(QnstEntity*)));

    connect(scene, SIGNAL(entityRemoved(QnstEntity*)),
            SLOT(requestEntityRemotion(QnstEntity*)));
}

//void QnstView::wheelEvent( QWheelEvent * event )
//{
//    int numDegrees = event->delta() / 8;

//    if (numDegrees > 0){
//        scale(1.1,1.1);
//    }else{
//        scale(0.9,0.9);
//    }

////    if (event->orientation() == Qt::Horizontal) {
////        scrollHorizontally(numSteps);
////    } else {

////        scrollVertically(numSteps);
////    }
//    event->accept();
//}

