#include "StructuralGraphicsComposition.h"

#include "StructuralGraphicsMedia.h"
#include "StructuralGraphicsContext.h"
#include "StructuralUtil.h"

#include <QDebug>

StructuralGraphicsComposition::StructuralGraphicsComposition(StructuralGraphicsEntity* parent)
  : StructuralGraphicsNode(parent)
{
  setnstType(Structural::Composition);

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

  // By default composition will not be able to show mouse hover highlight
  setMouseHoverHighlight(false);
}

StructuralGraphicsComposition::~StructuralGraphicsComposition()
{

}

QString StructuralGraphicsComposition::getColor() const
{
  return color;
}

void StructuralGraphicsComposition::setColor(QString color)
{
  this->color = color;
}

void StructuralGraphicsComposition::setCollapsed(bool collapsed)
{
  this->collapsed = collapsed;
}

bool StructuralGraphicsComposition::isCollapsed()
{
  return collapsed;
}

void StructuralGraphicsComposition::updateToolTip()
{
  QString tip = "";
  QString name = (getnstId() != "" ? getnstId() : "?");

  if (getnstType() == Structural::Context)
  {
    tip += "Context ("+name+")";
  }
  else if (getnstType() == Structural::Switch)
  {
    tip += "Switch ("+name+")";
  }
  else if (getnstType() == Structural::Body)
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

void StructuralGraphicsComposition::draw(QPainter* painter)
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

    painter->drawPixmap(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-16-8, QPixmap(":/icon/context"));

    drawMouseHoverHighlight(painter); // This should not be HERE!!

    painter->setPen(QPen(QBrush(Qt::black),0));

    QString localid = (getnstId() != "" ? getnstId() : "?");

    if (localid.length() > 5)
    {
      localid = getnstId().replace(3,getnstId().length()-3,"...");
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

void StructuralGraphicsComposition::delineate(QPainterPath* painter) const
{
  painter->addEllipse(4, 4, getWidth(), getHeight());
}


void StructuralGraphicsComposition::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
  foreach(QUrl url, event->mimeData()->urls())
  {
    event->acceptProposedAction();

    return;
  }
}

void StructuralGraphicsComposition::collapse()
{
  mouseDoubleClickEvent(NULL);
}

void StructuralGraphicsComposition::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  if (tmp == NULL)
  {
    tmp = new StructuralGraphicsComposition();
    tmp->hide();
  }

  if (collapsed)
  {
    setTop(getTop() - (lasth/2 - DEFAULT_MEDIA_HEIGHT/2));
    setLeft(getLeft() - (lastw/2 - DEFAULT_MEDIA_WIDTH/2));
    setWidth(lastw);
    setHeight(lasth);

    foreach(StructuralGraphicsEntity* e, getnstGraphicsEntities())
    {
      if (e->getncgType() == Qncg::Interface)
      {
        e->setTop(((e->getTop()*lasth)/DEFAULT_MEDIA_HEIGHT));
        e->setLeft(((e->getLeft()*lastw)/DEFAULT_MEDIA_WIDTH));
      }

      e->setnstGraphicsParent(this);
      e->show();
      e->adjust();

      if (e->getncgType() == Qncg::Interface)
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

    foreach(StructuralGraphicsEntity* e, getnstGraphicsEntities())
    {
      if (e->getncgType() != Qncg::Interface)
      {
        e->hide();
        e->setnstGraphicsParent(tmp);
      }
    }

    setTop(getTop() + lasth/2 - DEFAULT_MEDIA_HEIGHT/2);
    setLeft(getLeft() + lastw/2 - DEFAULT_MEDIA_WIDTH/2);
    setWidth(DEFAULT_MEDIA_WIDTH);
    setHeight(DEFAULT_MEDIA_HEIGHT);

    foreach(StructuralGraphicsEntity* e, getnstGraphicsEntities())
    {
      if (e->getncgType() == Qncg::Interface)
      {
        e->setTop(((e->getTop()*DEFAULT_MEDIA_HEIGHT)/lasth));
        e->setLeft(((e->getLeft()*DEFAULT_MEDIA_WIDTH)/lastw));

        e->adjust();
      }
    }

    setResizable(false);
  }

  if (getnstGraphicsParent() != NULL)
  {
    getnstGraphicsParent()->adjust();
  }

  setCollapsed(!collapsed);
}

void StructuralGraphicsComposition::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  foreach(QUrl url, event->mimeData()->urls())
  {
    QString filename = url.toLocalFile();
    QString suffix = QFileInfo(filename).suffix().toLower();
    event->acceptProposedAction();
    dropsrc = filename;

    createEntity(StructuralUtil::getnstTypeFromExtension(suffix));
  }
}

void StructuralGraphicsComposition::adjustWithSpring()
{
  int it = 0;

  while(it++ < SPRING_INTERATION)
  {
    int N = getnstGraphicsEntities().size();
    QVector<QPointF> next(N);

    qreal vf[N];
    qreal va[N];

    for (int I = 0; I < N; I++)
    {
      vf[I] = 0;
      va[I] = 0;
    }

    int I = 0;
    foreach (StructuralGraphicsEntity *entity, getnstGraphicsEntities())
    {
      if (entity->getncgType() == Qncg::Node)
      {
        StructuralGraphicsNode* node = (StructuralGraphicsNode*) entity;

        int attr = 1;

        qreal rforce = 0; // resultant vector magnitude
        qreal rangle = 0; // resultant vector angle (direction)

        foreach(StructuralGraphicsEdge* edge, node->getnstGraphicsEdges())
        {
          // setting entities
          StructuralGraphicsEntity* entitya;
          StructuralGraphicsEntity* entityb;

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

    foreach (StructuralGraphicsEntity *entity, getnstGraphicsEntities())
    {
      entity->setLeft(next[I].x());
      entity->setTop(next[I].y());
    }

    adjust();
  }
}

qreal StructuralGraphicsComposition::getLastW()
{
  return lastw;
}

qreal StructuralGraphicsComposition::getLastH()
{
  return lasth;
}

void StructuralGraphicsComposition::setLastW(qreal lastW)
{
  this->lastw = lastW;
}

void StructuralGraphicsComposition::setLastH(qreal lastH)
{
  this->lasth = lastH;
}

void StructuralGraphicsComposition::setProperties(const QMap<QString, QString> &props)
{
  StructuralGraphicsNode::setProperties(props);

  if (props["collapsed"] != "")
  {
    if (props["collapsed"] == "1")
    {
      if (props["width"] != "")
        setLastW(props["width"].toDouble());

      if (props["height"] != "")
        setLastH(props["height"].toDouble());

      if (props["expandHeight"] != "")
        setHeight(props["expandHeight"].toDouble());

      if (props["expandWidth"] != "")
        setWidth(props["expandWidth"].toDouble());
    }
    else
    {
      if (props["width"] != "")
        setWidth(props["width"].toDouble());

      if (props["height"] != "")
        setHeight(props["height"].toDouble());

      if (props["expandHeight"] != "")
        setLastH(props["expandHeight"].toDouble());

      if (props["expandWidth"] != "")
        setLastW(props["expandWidth"].toDouble());
    }
  }
}

void StructuralGraphicsComposition::getProperties(QMap<QString, QString> &properties)
{
  StructuralGraphicsNode::getProperties(properties);

  properties["collapsed"] = QString::number(isCollapsed());
  properties["expandWidth"] = QString::number(getWidth());
  properties["expandHeight"] = QString::number(getHeight());
}

//void QnstGraphicsComposition::setCollpsed(bool collapsed)
//{
//    this->collapsed = collapsed;
//}
