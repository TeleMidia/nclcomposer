#include "qnstgraphicscomposition.h"

#include "qnstgraphicshtml.h"
#include "qnstgraphicsncl.h"
#include "qnstgraphicsimage.h"
#include "qnstgraphicsaudio.h"
#include "qnstgraphicstext.h"
#include "qnstgraphicsvideo.h"
#include "qnstgraphicsscript.h"
#include "qnstgraphicssettings.h"
#include "qnstgraphicsmedia.h"
#include "qnstgraphicscontext.h"

#include <QDebug>

QnstGraphicsComposition::QnstGraphicsComposition(QnstGraphicsNode* parent)
    : QnstGraphicsNode(parent)
{
    setnstType(Qnst::Compostion);

    setColor("#EEEEEE");

    setAcceptDrops(true);

    lastw = 0;
    lasth = 0;

    collapsed = false;

    tmp = NULL;

//    tmp = new QnstGraphicsComposition();
//    tmp->hide();
}

QnstGraphicsComposition::~QnstGraphicsComposition()
{

}

QString QnstGraphicsComposition::getColor() const
{
    return color;
}

void QnstGraphicsComposition::setColor(QString color)
{
    this->color = color;
}

void QnstGraphicsComposition::setCollapsed(bool collapsed)
{
    this->collapsed = collapsed;
}

bool QnstGraphicsComposition::isCollapsed()
{
    return collapsed;
}

void QnstGraphicsComposition::setnstId(QString id)
{
    QnstGraphicsNode::setnstId(id);

    QString tip = "";
    QString name = (getnstId() != "" ? getnstId() : "?");

    if (getnstType() == Qnst::Context){
        tip += "Context ("+name+")";

    }else if (getnstType() == Qnst::Switch){
        tip += "Switch ("+name+")";

    }else if (getnstType() == Qnst::Body){
        tip += "Body ("+name+")";

    }else{
        tip += "Composition ("+name+")";
    }

    setToolTip(tip);
}

bool QnstGraphicsComposition::createEntity(Qnst::EntityType type)
{
  QnstGraphicsEntity *entity;

  switch(type)
  {
  case Qnst::Html:
    entity = new QnstGraphicsHTML(this);
    break;

  case Qnst::NCL:
    entity = new QnstGraphicsNCL(this);
    break;

  case Qnst::Image:
    entity = new QnstGraphicsImage(this);
    break;

  case Qnst::Audio:
    entity = new QnstGraphicsAudio(this);
    break;

  case Qnst::Text:
    entity = new QnstGraphicsText(this);
    break;

  case Qnst::Video:
    entity = new QnstGraphicsVideo(this);
    break;

  case Qnst::Script:
    entity = new QnstGraphicsScript(this);
    break;

  case Qnst::Settings:
    entity = new QnstGraphicsSettings(this);
    break;

  case Qnst::Media:
    entity = new QnstGraphicsMedia(this);
    break;

  case Qnst::Context:
    entity = new QnstGraphicsContext(this);
    break;

  case Qnst::Switch:
    entity = new QnstGraphicsSwitch(this);
    break;


  default:
    entity = NULL;
  }

  if(entity == NULL) return false;


  QnstGraphicsMedia *content  = dynamic_cast<QnstGraphicsMedia*>(entity);
  qDebug() << (bool) content;

  if(content != NULL) // If the Entity is a Media content
  {
    content->setTop(getHeight()/2 - 48/2);
    content->setLeft(getWidth()/2 - 48/2);
    content->setWidth(48);
    content->setHeight(64);
    content->adjust();

    if (dropsrc != "") //if it is a drop we will keep the baseName as id
    {
      content->setSource(dropsrc);
      QFileInfo file = QFileInfo(dropsrc);
      QString nstId = file.baseName();
      entity->setnstId(nstId);
      dropsrc = "";
    }
  }
  else
  {
    QnstGraphicsComposition *composition =
        dynamic_cast<QnstGraphicsComposition*>(entity);

    qDebug() << (bool) composition;

    //If the Entity is a Composition (i.e. Body, Context or Switch)
    if(composition != NULL)
    {
      composition->setTop(getHeight()/2 - 200/2);
      composition->setLeft(getWidth()/2 - 250/2);
      composition->setWidth(250);
      composition->setHeight(200);
      composition->adjust();

      composition->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());
    }
  }

  addnstGraphicsEntity(entity);

  emit entityAdded(entity);
}


void QnstGraphicsComposition::draw(QPainter* painter)
{
//    if (!isSelected() && hasMouseHover() && getnstType() != Qnst::Body){
//        painter->setBrush(Qt::NoBrush);
//        painter->setPen(QPen(QBrush(QColor("#999999")), 0, Qt::DashLine)); // 0px = cosmetic border

//        painter->drawRect(4, 4, getWidth(), getHeight());
//    }

    if (!collapsed){
        painter->setRenderHint(QPainter::Antialiasing,true);

        painter->setBrush(QColor(color));
        painter->setPen(QPen(QBrush(Qt::black), 0));

        painter->drawEllipse(4, 4, getWidth()-1, getHeight()-1);

        if (isMoving()){
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

            painter->drawEllipse(getMoveLeft()+4-getLeft(), getMoveTop()+4-getTop(), getWidth()-1, getHeight()-1);

        }else if (isResizing()){
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

            painter->drawEllipse(getResizeLeft()+4-getLeft(), getResizeTop()+4-getTop(), getResizeWidth()-1, getResizeHeight()-1);
        }

    }else{
        painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

        painter->drawPixmap(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-16-8, QPixmap(":/icon/context"));

        if (!isSelected() && hasMouseHover()){
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(QBrush(QColor("#999999")), 0, Qt::DashLine)); // 0px = cosmetic border

            painter->drawRect(4, 4, getWidth(), getHeight());
        }

        painter->setPen(QPen(QBrush(Qt::black),0));

        QString localid = (getnstId() != "" ? getnstId() : "?");

        if (localid.length() > 5){
            localid = getnstId().replace(3,getnstId().length()-3,"...");
        }

        painter->drawText(4 + 8/2, 4 + 8/2 + getHeight()-16-8, getWidth()-8, 16, Qt::AlignCenter, localid);

        if (isMoving()){
            painter->setBrush(Qt::NoBrush);
            painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

            painter->setRenderHint(QPainter::Antialiasing,false);
            painter->drawRect(getMoveLeft()+4-getLeft(), getMoveTop()+4-getTop(), getWidth()-1, getHeight()-1);
        }
    }
}

void QnstGraphicsComposition::delineate(QPainterPath* painter) const
{
    painter->addEllipse(4, 4, getWidth(), getHeight());
}


void QnstGraphicsComposition::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    foreach(QUrl url, event->mimeData()->urls()){
            event->acceptProposedAction();

            return;
    }
}

void QnstGraphicsComposition::collapse()
{
    mouseDoubleClickEvent(NULL);
}

void QnstGraphicsComposition::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (tmp == NULL){
        tmp = new QnstGraphicsComposition();
        tmp->hide();
    }

    if (collapsed){
        setTop(getTop() - (lasth/2 - 64/2));
        setLeft(getLeft() - (lastw/2 - 48/2));
        setWidth(lastw);
        setHeight(lasth);

        foreach(QnstGraphicsEntity* e, getnstGraphicsEntities()){
            if (e->getncgType() == Qncg::Interface){
                e->setTop(((e->getTop()*lasth)/64));
                e->setLeft(((e->getLeft()*lastw)/48));
            }

            e->setnstGraphicsParent(this);
            e->show();
            e->adjust();

            if (e->getncgType() == Qncg::Interface){
                qDebug() << e->getLeft() << e->getTop() << e->getWidth() << e->getHeight();
            }
        }

        setResizable(true);

    }else{
        tmp->setTop(getTop());
        tmp->setLeft(getLeft());
        tmp->setWidth(getWidth());
        tmp->setHeight(getHeight());

        lastw = getWidth();
        lasth = getHeight();

        foreach(QnstGraphicsEntity* e, getnstGraphicsEntities()){
            if (e->getncgType() != Qncg::Interface){
                e->hide();
                e->setnstGraphicsParent(tmp);
            }
        }

        setTop(getTop() + lasth/2 - 64/2);
        setLeft(getLeft() + lastw/2 - 48/2);
        setWidth(48);
        setHeight(64);

        foreach(QnstGraphicsEntity* e, getnstGraphicsEntities()){
            if (e->getncgType() == Qncg::Interface){
                e->setTop(((e->getTop()*64)/lasth));
                e->setLeft(((e->getLeft()*48)/lastw));

                e->adjust();
            }
        }

        setResizable(false);
    }

    if (getnstGraphicsParent() != NULL){
        getnstGraphicsParent()->adjust();
    }

    setCollapsed(!collapsed);
}

void QnstGraphicsComposition::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  foreach(QUrl url, event->mimeData()->urls())
  {
    QString filename = url.toLocalFile();
    QString suffix = QFileInfo(filename).suffix().toUpper();

    if(suffix == "PNG" ||
       suffix == "JPEG")
    {
      event->acceptProposedAction();
      dropsrc = filename;
      createEntity(Qnst::Image);
    }
    else if(suffix == "MP3" ||
            suffix == "WAV")
    {
      event->acceptProposedAction();
      dropsrc = filename;
      createEntity(Qnst::Audio);
    }
    else if(suffix == "AVI" ||
            suffix == "MPEG4" ||
            suffix == "MP4" ||
            suffix == "MPEG")
    {
      event->acceptProposedAction();
      dropsrc = filename;
      createEntity(Qnst::Video);
    }
    else if(suffix=="TXT")
    {
      event->acceptProposedAction();
      dropsrc = filename;
      createEntity(Qnst::Text);
    }
    else if(suffix=="LUA")
    {
      event->acceptProposedAction();
      dropsrc = filename;
      createEntity(Qnst::Script);
    }
    else if(suffix=="HTML" ||
            suffix=="HTM")
    {
      event->acceptProposedAction();
      dropsrc = filename;
      createEntity(Qnst::Html);
    }
    else if(suffix=="NCL")
    {
      event->acceptProposedAction();
      dropsrc = filename;
      createEntity(Qnst::NCL);
    }
  }
}

void QnstGraphicsComposition::adjustWithSpring()
{
    int it = 0;

    while(it++ < SPRING_INTERATION){

        int N = getnstGraphicsEntities().size(); QPointF next[N];

        qreal vf[N];
        qreal va[N];

        for (int I = 0; I < N; I++){
            vf[I] = 0;
            va[I] = 0;
        }

        for (int I = 0; I < N; I++){
            QnstGraphicsEntity* entity = getnstGraphicsEntities().at(I);

            if (entity->getncgType() == Qncg::Node){
                QnstGraphicsNode* node = (QnstGraphicsNode*) entity;

                int attr = 1;

                qreal rforce = 0; // resultant vector magnitude
                qreal rangle = 0; // resultant vector angle (direction)

                foreach(QnstGraphicsEdge* edge, node->getnstGraphicsEdges()){

                    // setting entities
                    QnstGraphicsEntity* entitya;
                    QnstGraphicsEntity* entityb;

                    if (edge->getEntityA() != node){
                        entitya = edge->getEntityB();
                        entityb = edge->getEntityA();

                    }else{
                        continue;
                        entitya = edge->getEntityA();
                        entityb = edge->getEntityB();
                    }

                    // setting line
                    QLineF line;

                    line.setP1(QPointF(entitya->getLeft() + entitya->getWidth()/2,
                                       entitya->getTop() + entitya->getHeight()/2));

                    line.setP2(QPointF(entityb->getLeft() + entityb->getWidth()/2,
                                       entityb->getTop() + entityb->getHeight()/2));

                    qreal d = line.length();

                    // calculating attraction
                    qreal cforce; // current vector magnitude
                    qreal cangle; // current vector angle (direction)

                    attr = ((d - SPRING_LENGTH) > 0 ? 1 : -1);

                    cforce = SPRING_CONSTANT*(d - SPRING_LENGTH >= 0 ? d - SPRING_LENGTH : (d - SPRING_LENGTH)*(-1));
                    cangle = line.angle();

                    qreal fxa = rforce*cos(rangle*(PI / 180));
                    qreal fya = rforce*sin(rangle*(PI / 180));
                    qreal fxb = cforce*cos(cangle*(PI / 180));
                    qreal fyb = cforce*sin(cangle*(PI / 180));

                    qreal fxr = fxa + fxb;
                    qreal fyr = fya + fyb;

                    rforce = sqrt(pow(fxr,2) + pow(fyr,2));


                    if (rforce == 0){
                        rangle = 0;
                    }else if (fabs(fxr) > fabs(fyr)){
                        rangle = ((180 / PI)*tanh(fyr/fxr));
                    }else{
                        rangle = 90 - ((180 / PI)*tanh(fxr/fyr));
                    }

                    if (fxr < 0){
                        rangle += 180;
                    }else if (fyr < 0 && rangle < 0){
                        rangle += 360;
                    }

                }

                qreal fxa = vf[I]*cos(va[I]*(PI / 180));
                qreal fya = vf[I]*sin(va[I]*(PI / 180));
                qreal fxb = rforce*cos(rangle*(PI / 180));
                qreal fyb = rforce*sin(rangle*(PI / 180));

                qreal fxr = fxa + fxb;
                qreal fyr = fya + fyb;

                rforce = sqrt(pow(fxr,2) + pow(fyr,2));

                vf[I] = rforce*SPRING_DAMPING;

                if (rforce == 0){
                    rangle = 0;
                }else if (fabs(fxr) > fabs(fyr)){
                    rangle = ((180 / PI)*tanh(fyr/fxr));
                }else{
                    rangle = 90 - ((180 / PI)*tanh(fxr/fyr));
                }

                if (fxr < 0){
                    rangle += 180;
                }else if (fyr < 0 && rangle < 0){
                    rangle += 360;
                }

                va[I] = rangle;

                qreal rx = vf[I]*cos(va[I]*(PI / 180));
                qreal ry = vf[I]*sin(va[I]*(PI / 180));

                if (rangle < 90){
                    next[I] = QPointF(entity->getLeft() +  rx*attr, entity->getTop() - ry*attr);

                }else if (rangle < 180){
                    next[I] = QPointF(entity->getLeft() + rx*attr, entity->getTop() - ry*attr);

                }else if (rangle < 270){
                    next[I] = QPointF(entity->getLeft() + rx*attr, entity->getTop() - ry*attr);

                }else{
                    next[I] = QPointF(entity->getLeft() + rx*attr, entity->getTop() - ry*attr);
                }
            }
        }

        for (int I = 0; I < N; I++){
            QnstGraphicsEntity* entity = getnstGraphicsEntities().at(I);

            entity->setLeft(next[I].x());
            entity->setTop(next[I].y());
        }

        adjust();
    }
}

qreal QnstGraphicsComposition::getLastW()
{
    return lastw;
}

qreal QnstGraphicsComposition::getLastH()
{
    return lasth;
}

void QnstGraphicsComposition::setLastW(qreal lastW)
{
    this->lastw = lastW;
}

void QnstGraphicsComposition::setLastH(qreal lastH)
{
    this->lasth = lastH;
}

//void QnstGraphicsComposition::setCollpsed(bool collapsed)
//{
//    this->collapsed = collapsed;
//}
