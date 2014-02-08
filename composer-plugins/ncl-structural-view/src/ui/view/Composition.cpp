#include "Composition.h"

#include "Media.h"
#include "Util.h"

#include <QDebug>

QnstComposition::QnstComposition(QnstEntity* parent)
  : QnstNode(parent)
{
  //setnstSubtype(Qnst::Composition);

  setColor("#EEEEEE");

  setAcceptDrops(true);


  lastw = 0;
  lasth = 0;

  collapsed = false;

  tmp = NULL;

  //tmp = new QnstGraphicsComposition();
  //tmp->hide();

  /* Default size and position */
  if(parent)
  {
    setTop(parent->getHeight()/2 - DEFAULT_CONTEXT_HEIGHT/2);
    setLeft(parent->getWidth()/2 - DEFAULT_CONTEXT_WIDTH/2);
  }
  else
  {
    setTop(0);
    setLeft(0);
  }

  setWidth(DEFAULT_CONTEXT_WIDTH);
  setHeight(DEFAULT_CONTEXT_HEIGHT);


  createObjects();
  createConnections();

  // By default composition will not be able to show mouse hover highlight
  setMouseHoverHighlight(false);
}

QnstComposition::~QnstComposition()
{

}

void QnstComposition::createObjects()
{
  menu = new QnstMenu();
  menu->actionDelete->setEnabled(true);

  menu->actionExport->setEnabled(true);

  menu->menuInsert->setEnabled(true);

  menu->actionAddMedia->setEnabled(true);
  menu->actionAddContext->setEnabled(true);
  menu->actionAddSwitch->setEnabled(true);
  menu->actionAddPort->setEnabled(true);
  menu->actionAddProperty->setEnabled(true);
  menu->actionAddAggregator->setEnabled(true);

  menu->menuAdjust->setEnabled(true);
  menu->actionAuto->setEnabled(true);
}

void QnstComposition::createConnections()
{
  // connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
  // connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));


  connect(menu, SIGNAL(menuAddEntityTriggered(QnstSubtype)),SLOT(newChild(QnstSubtype)));

  connect(menu, SIGNAL(autoRequested()), SLOT(adjustWithSpring()));
}

QString QnstComposition::getColor() const
{
  return color;
}

void QnstComposition::setColor(QString color)
{
  this->color = color;
}

void QnstComposition::setCollapsed(bool collapsed)
{
  this->collapsed = collapsed;
}

bool QnstComposition::isCollapsed()
{
  return collapsed;
}

void QnstComposition::setnstSubtype(const QnstSubtype subtype)
{
  if (subtype == Qnst::Context){
    setColor("#F4E4CC");
  }else if (subtype == Qnst::Switch){
    setColor("#C6E2FF");
    menu->actionAddSwitchPort->setEnabled(true);
    menu->actionAddPort->setEnabled(false);
  }else if (subtype == Qnst::Body){
    setColor("#EEEEEE");
  }

  QnstNode::setnstSubtype(subtype);

  updateToolTip();
}

void QnstComposition::updateToolTip()
{
  QString tip = "";
  QString name = (getnstProperty(":nst:id") != "" ? getnstProperty(":nst:id") : "?");

  if (getnstSubtype() == Qnst::Context)
  {
    tip += "Context ("+name+")";
  }
  else if (getnstSubtype() == Qnst::Switch)
  {
    tip += "Switch ("+name+")";
  }
  else if (getnstSubtype() == Qnst::Body)
  {
    tip += "Body ("+name+")";
  }
  else
  {
    tip += "Composition ("+name+")";
  }

  if (hasError)
    tip += " - Error: " + erroMsg;

  setToolTip(tip);
}

void QnstComposition::draw(QPainter* painter)
{
  if (!collapsed)
  {
    painter->setRenderHint(QPainter::Antialiasing,true);

    painter->setBrush(QColor(color));

    if (hasError)
      painter->setPen(QPen(QBrush(Qt::red), 2));
    else
      painter->setPen(QPen(QBrush(Qt::black), 0));

    painter->drawEllipse(4, 4, getWidth()-1, getHeight()-1);

    drawMouseHoverHighlight(painter); // This should not be HERE!!

    if (isMoving())
    {
      painter->setBrush(Qt::NoBrush);
      painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

      painter->drawEllipse(getMoveLeft()+4-getLeft(), getMoveTop()+4-getTop(), getWidth()-1, getHeight()-1);

    }
    else if (isResizing())
    {
      painter->setBrush(Qt::NoBrush);
      painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

      painter->drawEllipse(getResizeLeft()+4-getLeft(), getResizeTop()+4-getTop(), getResizeWidth()-1, getResizeHeight()-1);
    }
  }
  else
  {
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter->drawPixmap(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-16-8, QPixmap(QnstUtil::iconFromMediaType(getnstSubtype())));

    drawMouseHoverHighlight(painter); // This should not be HERE!!

    painter->setPen(QPen(QBrush(Qt::black),0));

    QString localid = (getnstProperty(":nst:uid") != "" ? getnstProperty(":nst:uid") : "?");

    if (localid.length() > 5)
    {
      localid = getnstProperty(":nst:uid").replace(3,getnstProperty(":nst:uid").length()-3,"...");
    }

    painter->drawText(4 + 8/2, 4 + 8/2 + getHeight()-16-8, getWidth()-8, 16, Qt::AlignCenter, localid);

    if (isMoving())
    {
      painter->setBrush(Qt::NoBrush);
      painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

      painter->setRenderHint(QPainter::Antialiasing,false);
      painter->drawRect(getMoveLeft()+4-getLeft(), getMoveTop()+4-getTop(), getWidth()-1, getHeight()-1);
    }
  }
}

void QnstComposition::delineate(QPainterPath* painter) const
{
  painter->addEllipse(4, 4, getWidth(), getHeight());
}


void QnstComposition::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
  foreach(QUrl url, event->mimeData()->urls())
  {
    event->acceptProposedAction();

    return;
  }
}

void QnstComposition::collapse()
{
  mouseDoubleClickEvent(NULL);
}

void QnstComposition::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  if (tmp == NULL)
  {
    tmp = new QnstComposition();
    tmp->hide();
  }

  if (collapsed)
  {
    setTop(getTop() - (lasth/2 - DEFAULT_MEDIA_HEIGHT/2));
    setLeft(getLeft() - (lastw/2 - DEFAULT_MEDIA_WIDTH/2));
    setWidth(lastw);
    setHeight(lasth);

    foreach(QnstEntity* e, getnstChildren())
    {
      if (e->getnstType() == Qnst::Interface)
      {
        e->setTop(((e->getTop()*lasth)/DEFAULT_MEDIA_HEIGHT));
        e->setLeft(((e->getLeft()*lastw)/DEFAULT_MEDIA_WIDTH));
      }

      e->setnstParent(this);
      e->show();
      e->adjust();

      if (e->getnstType() == Qnst::Interface)
      {
        qDebug() << e->getLeft() << e->getTop() << e->getWidth() << e->getHeight();
      }
    }

    setResizable(true);
  }
  else
  {
    tmp->setTop(getTop());
    tmp->setLeft(getLeft());
    tmp->setWidth(getWidth());
    tmp->setHeight(getHeight());

    lastw = getWidth();
    lasth = getHeight();

    foreach(QnstEntity* e, getnstChildren())
    {
      if (e->getnstType() != Qnst::Interface)
      {
        e->hide();
        e->setnstParent(tmp);
      }
    }

    setTop(getTop() + lasth/2 - DEFAULT_MEDIA_HEIGHT/2);
    setLeft(getLeft() + lastw/2 - DEFAULT_MEDIA_WIDTH/2);
    setWidth(DEFAULT_MEDIA_WIDTH);
    setHeight(DEFAULT_MEDIA_HEIGHT);

    foreach(QnstEntity* e, getnstChildren())
    {
      if (e->getnstType() == Qnst::Interface)
      {
        e->setTop(((e->getTop()*DEFAULT_MEDIA_HEIGHT)/lasth));
        e->setLeft(((e->getLeft()*DEFAULT_MEDIA_WIDTH)/lastw));

        e->adjust();
      }
    }

    setResizable(false);
  }

  if (getnstParent() != NULL)
  {
    getnstParent()->adjust();
  }

  setCollapsed(!collapsed);
}

void QnstComposition::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  foreach(QUrl url, event->mimeData()->urls())
  {
    QString filename = url.toLocalFile();
    QString suffix = QFileInfo(filename).suffix().toLower();

    event->acceptProposedAction();
    dropsrc = filename;

    newChild(QnstUtil::getnstTypeFromExtension(suffix));
  }
}

void QnstComposition::adjustWithSpring()
{
  int it = 0;

  while(it++ < SPRING_INTERATION)
  {
    int N = getnstChildren().size();
    QPointF next[N];

    qreal vf[N];
    qreal va[N];

    for (int I = 0; I < N; I++)
    {
      vf[I] = 0;
      va[I] = 0;
    }

    int I = 0;
    foreach (QnstEntity *entity, getnstChildren())
    {
      if (entity->getnstType() == Qnst::Node)
      {
        QnstNode* node = (QnstNode*) entity;

        int attr = 1;

        qreal rforce = 0; // resultant vector magnitude
        qreal rangle = 0; // resultant vector angle (direction)

        foreach(QnstEdge* edge, node->getnstGraphicsEdges())
        {
          // setting entities
          QnstEntity* entitya;
          QnstEntity* entityb;

          if (edge->getEntityA() != node)
          {
            entitya = edge->getEntityB();
            entityb = edge->getEntityA();
          }
          else
          {
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

          if (rforce == 0)
          {
            rangle = 0;
          }
          else if (fabs(fxr) > fabs(fyr))
          {
            rangle = ((180 / PI)*tanh(fyr/fxr));
          }
          else
          {
            rangle = 90 - ((180 / PI)*tanh(fxr/fyr));
          }

          if (fxr < 0)
          {
            rangle += 180;
          }
          else if (fyr < 0 && rangle < 0)
          {
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

        if (rforce == 0)
        {
          rangle = 0;
        }
        else if (fabs(fxr) > fabs(fyr))
        {
          rangle = ((180 / PI)*tanh(fyr/fxr));
        }
        else
        {
          rangle = 90 - ((180 / PI)*tanh(fxr/fyr));
        }

        if (fxr < 0)
        {
          rangle += 180;
        }
        else if (fyr < 0 && rangle < 0)
        {
          rangle += 360;
        }

        va[I] = rangle;

        qreal rx = vf[I]*cos(va[I]*(PI / 180));
        qreal ry = vf[I]*sin(va[I]*(PI / 180));

        if (rangle < 90)
        {
          next[I] = QPointF(entity->getLeft() +  rx*attr, entity->getTop() - ry*attr);
        }
        else if (rangle < 180)
        {
          next[I] = QPointF(entity->getLeft() + rx*attr, entity->getTop() - ry*attr);
        }
        else if (rangle < 270)
        {
          next[I] = QPointF(entity->getLeft() + rx*attr, entity->getTop() - ry*attr);
        }
        else
        {
          next[I] = QPointF(entity->getLeft() + rx*attr, entity->getTop() - ry*attr);
        }
      }

      I++;
    }

    foreach (QnstEntity *entity, getnstChildren())
    {
      entity->setLeft(next[I].x());
      entity->setTop(next[I].y());
    }

    adjust();
  }
}

qreal QnstComposition::getLastW()
{
  return lastw;
}

qreal QnstComposition::getLastH()
{
  return lasth;
}

void QnstComposition::setLastW(qreal lastW)
{
  this->lastw = lastW;
}

void QnstComposition::setLastH(qreal lastH)
{
  this->lasth = lastH;
}

//void QnstGraphicsComposition::setCollpsed(bool collapsed)
//{
//    this->collapsed = collapsed;
//}
