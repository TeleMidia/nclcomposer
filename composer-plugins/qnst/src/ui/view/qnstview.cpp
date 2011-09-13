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

            QString name = "p"+QString::number(property_count);

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

            // todo: attrs

            emit areaAdded(sUID, psUID,attrs);

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

            area_count++;

            QString name = "link"+QString::number(area_count);

            s->setName(name);

            QMap<QString, QString> attrs;

            attrs["id"] = name;

            if (s->getnBegin()->getEntityType() == Qncg::Node){
                QnstNode* n = (QnstNode*) s->getnBegin();

                attrs["bnode"] = n->getUid();
            }

            if (s->getnEnd()->getEntityType() == Qncg::Node){
                QnstNode* n = (QnstNode*) s->getnEnd();

                attrs["enode"] = n->getUid();
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


void QnstView::createConnection()
{
    connect(scene, SIGNAL(entityAdded(QnstEntity*)),
            SLOT(requestEntityAddition(QnstEntity*)));

    connect(scene, SIGNAL(entitySelected(QnstEntity*)),
            SLOT(requestEntitySelection(QnstEntity*)));
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

