#include "StructuralComposition.h"

#include "StructuralMedia.h"
#include "StructuralUtil.h"

#include <QDebug>

StructuralComposition::StructuralComposition(StructuralEntity* parent)
  : StructuralNode(parent)
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

StructuralComposition::~StructuralComposition()
{

}

void StructuralComposition::createObjects()
{
  menu->deleteAction->setEnabled(true);

  menu->snapshotAction->setEnabled(true);

  menu->insertMenu->setEnabled(true);

  menu->mediaAction->setEnabled(true);
  menu->contextAction->setEnabled(true);
  menu->switchAction->setEnabled(true);
  menu->portAction->setEnabled(true);
  menu->propertyAction->setEnabled(true);

}

void StructuralComposition::createConnections()
{
  // connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
  // connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));


//  connect(menu, SIGNAL(menuAddEntityTriggered(QnstSubtype)),SLOT(newChild(QnstSubtype)));

//  connect(menu, SIGNAL(autoRequested()), SLOT(adjustWithSpring()));
}

QString StructuralComposition::getColor() const
{
  return color;
}

void StructuralComposition::setColor(QString color)
{
  this->color = color;
}

void StructuralComposition::setCollapsed(bool collapsed)
{
  this->collapsed = collapsed;
}

bool StructuralComposition::isCollapsed()
{
  return collapsed;
}

void StructuralComposition::setLocalName(const LocalName subtype)
{
  if (subtype == Structural::Context){
    setColor("#F4E4CC");
    menu->switchPortAction->setEnabled(false);
    menu->portAction->setEnabled(true);
  }else if (subtype == Structural::Switch){
    setColor("#C6E2FF");
    menu->switchPortAction->setEnabled(true);
    menu->portAction->setEnabled(false);
  }else if (subtype == Structural::Body){
    setColor("#EEEEEE");
    menu->switchPortAction->setEnabled(false);
    menu->portAction->setEnabled(true);
  }

  StructuralNode::setLocalName(subtype);

  updateToolTip();
}

void StructuralComposition::updateToolTip()
{
  QString tip = "";
  QString name = (getLocalProperty("LOCAL:ID") != "" ? getLocalProperty("LOCAL:ID") : "?");

  if (getLocalName() == Structural::Context)
  {
    tip += "Context ("+name+")";
  }
  else if (getLocalName() == Structural::Switch)
  {
    tip += "Switch ("+name+")";
  }
  else if (getLocalName() == Structural::Body)
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

void StructuralComposition::draw(QPainter* painter)
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

    painter->drawPixmap(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-16-8, QPixmap(StructuralUtil::iconFromMediaType(getLocalName())));

    drawMouseHoverHighlight(painter); // This should not be HERE!!

    painter->setPen(QPen(QBrush(Qt::black),0));

    QString localid = (getLocalProperty(":nst:uid") != "" ? getLocalProperty(":nst:uid") : "?");

    if (localid.length() > 5)
    {
      localid = getLocalProperty(":nst:uid").replace(3,getLocalProperty(":nst:uid").length()-3,"...");
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

void StructuralComposition::delineate(QPainterPath* painter) const
{
  painter->addEllipse(4, 4, getWidth(), getHeight());
}


void StructuralComposition::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
  foreach(QUrl url, event->mimeData()->urls())
  {
    event->acceptProposedAction();

    return;
  }
}

void StructuralComposition::collapse()
{
  mouseDoubleClickEvent(NULL);
}

void StructuralComposition::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  if (tmp == NULL)
  {
    tmp = new StructuralComposition();
    tmp->hide();
  }

  if (collapsed)
  {  
    setTop(getTop() - (lasth/2 - DEFAULT_MEDIA_HEIGHT/2));
    setLeft(getLeft() - (lastw/2 - DEFAULT_MEDIA_WIDTH/2));
    setWidth(lastw);
    setHeight(lasth);

    foreach(StructuralEntity* e, getLocalChildren())
    {
      if (e->getLocalType() == Structural::Interface)
      {
        e->setTop(((e->getTop()*lasth)/DEFAULT_MEDIA_HEIGHT));
        e->setLeft(((e->getLeft()*lastw)/DEFAULT_MEDIA_WIDTH));
      }

      e->setLocalParent(this);
      e->show();
      e->adjust();

      if (e->getLocalType() == Structural::Interface)
      {
        qDebug() << e->getLeft() << e->getTop() << e->getWidth() << e->getHeight();
      }
    }

    setResizable(true);
  }
  else
  {
    tmp->setLocalProperties(getLocalProperties());

    lastw = getWidth();
    lasth = getHeight();

    foreach(StructuralEntity* e, getLocalChildren())
    {
      if (e->getLocalType() != Structural::Interface)
      {
        e->hide();
        e->setLocalParent(tmp);
        removeLocalChild(e);
      }
    }

    setTop(getTop() + lasth/2 - DEFAULT_MEDIA_HEIGHT/2);
    setLeft(getLeft() + lastw/2 - DEFAULT_MEDIA_WIDTH/2);
    setWidth(DEFAULT_MEDIA_WIDTH);
    setHeight(DEFAULT_MEDIA_HEIGHT);

    foreach(StructuralEntity* e, getLocalChildren())
    {
      if (e->getLocalType() == Structural::Interface)
      {
        e->setTop(((e->getTop()*DEFAULT_MEDIA_HEIGHT)/lasth));
        e->setLeft(((e->getLeft()*DEFAULT_MEDIA_WIDTH)/lastw));

        e->adjust();
      }
    }

    setResizable(false);
  }

  if (getLocalParent() != NULL)
  {
    getLocalParent()->adjust();
  }

  setCollapsed(!collapsed);
}

void StructuralComposition::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  foreach(QUrl url, event->mimeData()->urls())
  {
    QString filename = url.toLocalFile();
    QString suffix = QFileInfo(filename).suffix().toLower();

    event->acceptProposedAction();
    dropsrc = filename;

    newChild(StructuralUtil::getnstTypeFromExtension(suffix));
  }
}

void StructuralComposition::adjustWithSpring()
{
  int it = 0;

  while(it++ < SPRING_INTERATION)
  {
    int N = getLocalChildren().size();
    QVector<QPointF> next(N);

    qreal vf[N];
    qreal va[N];

    for (int I = 0; I < N; I++)
    {
      vf[I] = 0;
      va[I] = 0;
    }

    int I = 0;
    foreach (StructuralEntity *entity, getLocalChildren())
    {
      if (entity->getLocalType() == Structural::Node)
      {
        StructuralNode* node = (StructuralNode*) entity;

        int attr = 1;

        qreal rforce = 0; // resultant vector magnitude
        qreal rangle = 0; // resultant vector angle (direction)

        foreach(StructuralEdge* edge, node->getnstGraphicsEdges())
        {
          // setting entities
          StructuralEntity* entitya;
          StructuralEntity* entityb;

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

    foreach (StructuralEntity *entity, getLocalChildren())
    {
      entity->setLeft(next[I].x());
      entity->setTop(next[I].y());
    }

    adjust();
  }
}

qreal StructuralComposition::getLastW()
{
  return lastw;
}

qreal StructuralComposition::getLastH()
{
  return lasth;
}

void StructuralComposition::setLastW(qreal lastW)
{
  this->lastw = lastW;
}

void StructuralComposition::setLastH(qreal lastH)
{
  this->lasth = lastH;
}

//void QnstGraphicsComposition::setCollpsed(bool collapsed)
//{
//    this->collapsed = collapsed;
//}
