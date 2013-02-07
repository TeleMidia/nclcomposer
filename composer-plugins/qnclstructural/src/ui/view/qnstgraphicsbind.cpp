#include "qnstgraphicsbind.h"

QnstGraphicsBind::QnstGraphicsBind(QnstGraphicsEntity* parent)
  : QnstGraphicsEdge(parent)
{
  // \todo We should have only Qnst::Bind
  setnstType(Qnst::Condition);

  setType(Qnst::NoBindType);

  dialog = new QnstGraphicsBindDialog();

  conn = NULL;

  link = NULL;
}

QnstGraphicsBind::~QnstGraphicsBind()
{
  delete dialog;
}

void QnstGraphicsBind::setType(Qnst::BindType type)
{
  this->type = type;

  switch(type)
  {
    //ACTIONS
    case Qnst::Start:
      setnstType(Qnst::Action);
      this->icon = ":/icon/start";
      break;

    case Qnst::Stop:
      setnstType(Qnst::Action);
      this->icon = ":/icon/stop";
      break;

    case Qnst::Resume:
      setnstType(Qnst::Action);
      this->icon = ":/icon/resume";
      break;

    case Qnst::Pause:
      setnstType(Qnst::Action);
      this->icon = ":/icon/pause";
      break;

    case Qnst::Set:
      setnstType(Qnst::Action);
      this->icon = ":/icon/set";
      break;

    //CONDITIONS
    case Qnst::onBegin:
      setnstType(Qnst::Condition);
      this->icon = ":/icon/onbegin";
      break;

    case Qnst::onEnd:
      setnstType(Qnst::Condition);
      this->icon = ":/icon/onend";
      break;

    case Qnst::onResume:
      setnstType(Qnst::Condition);
      this->icon = ":/icon/onresume";
      break;

    case Qnst::onPause:
      setnstType(Qnst::Condition);
      this->icon = ":/icon/onpause";
      break;

    case Qnst::onSelection:
      setnstType(Qnst::Condition);
      this->icon = ":/icon/onselection";
      break;

    default:
      if(getnstType() == Qnst::Action)
        this->icon = ":/icon/noaction";
      else
        this->icon = ":/icon/nocondition";
      break;
  }
}

Qnst::BindType QnstGraphicsBind::getType()
{
  return type;
}

QString QnstGraphicsBind::getIcon()
{
  return icon;
}

void QnstGraphicsBind::adjust()
{
  // if(isCondition())
  //  adjust_condition();
  // else
  adjust_action();
}

void QnstGraphicsBind::adjust_action()
{
  if (getEntityA() != NULL && getEntityB() != NULL)
  {
    QLineF line = QLineF(QPointF(getEntityA()->getLeft() + getEntityA()->getWidth()/2,
                                 getEntityA()->getTop() + getEntityA()->getHeight()/2),
                         QPointF(getEntityB()->getLeft() + getEntityB()->getWidth()/2,
                                 getEntityB()->getTop() + getEntityB()->getHeight()/2));

    if (getEntityA()->getncgType() == Qncg::Interface)
    {
      line.setP1(getnstGraphicsParent()->mapFromItem(getEntityA()->getnstGraphicsParent(), line.p1()));
    }

    if (getEntityB()->getncgType() == Qncg::Interface)
    {
      line.setP2(getnstGraphicsParent()->mapFromItem(getEntityB()->getnstGraphicsParent(), line.p2()));
    }

    QPointF pointa = line.p1();
    QPointF pointb = line.p2();

    aux_adjust(pointa, pointb);

    getEntityB()->setSelectable(false);

    qreal index;

    if (pointa != pointb)
    {
      index = 1.0;

      int n = 0;

      while(getEntityB()->collidesWithItem(this))
      {
        index -= 0.01;

        if (getAngle() == 0)
          pointb = line.pointAt(index);
        else
          pointb = arcPointAt(line , index);

        aux_adjust(pointa, pointb);

        if (++n > 100){ // avoiding infinity loop
          break;
        }
      }
    }

    getEntityB()->setSelectable(true);

    if (scene() != NULL)
      scene()->update();
  }
}

void QnstGraphicsBind::adjust_condition()
{
  if (getEntityA() != NULL && getEntityB() != NULL)
  {
    QLineF line = QLineF(QPointF(getEntityA()->getLeft() + getEntityA()->getWidth()/2,
                                 getEntityA()->getTop() + getEntityA()->getHeight()/2),
                         QPointF(getEntityB()->getLeft() + getEntityB()->getWidth()/2,
                                 getEntityB()->getTop() + getEntityB()->getHeight()/2));

    if (getEntityA()->getncgType() == Qncg::Interface)
    {
      line.setP1(getnstGraphicsParent()->mapFromItem(getEntityA()->getnstGraphicsParent(), line.p1()));
    }

    if (getEntityB()->getncgType() == Qncg::Interface)
    {
      line.setP2(getnstGraphicsParent()->mapFromItem(getEntityB()->getnstGraphicsParent(), line.p2()));
    }

    QPointF pointa = line.p1();
    QPointF pointb = line.p2();

    aux_adjust(pointa, pointb);

    getEntityA()->setSelectable(false);

    qreal index;

    if (pointa != pointb){

      index = 0;

      int n = 0;

      while(getEntityA()->collidesWithItem(this))
      {
        index += 0.01;

        if (getAngle() == 0)
          pointa = line.pointAt(index);
        else
          pointa = arcPointAt(line , index, false);

        aux_adjust(pointa, pointb);

        if (++n > 100) // avoiding infinity loop
          break;
      }
    }

    getEntityA()->setSelectable(true);

    if (scene() != NULL)
      scene()->update();
  }
}

void QnstGraphicsBind::aux_adjust(QPointF pointa, QPointF pointb)
{
  if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y())
  {
    setTop(pointa.y()-4);
    setLeft(pointa.x()-4);
    setWidth((pointb.x()-4)-(pointa.x()-4) + 8);
    setHeight((pointb.y()-4)-(pointa.y()-4) + 8);
  }
  else if (pointa.x() > pointb.x() && pointa.y() < pointb.y())
  {
    setTop(pointa.y()-4);
    setLeft(pointb.x()-4);
    setWidth((pointa.x()-4)-(pointb.x()-4) + 8);
    setHeight((pointb.y()-4)-(pointa.y()-4) + 8);
  }
  else if (pointa.x() < pointb.x() && pointa.y() > pointb.y())
  {
    setTop(pointb.y()-4);
    setLeft((pointa.x()-4));
    setWidth((pointb.x()-4)-(pointa.x()-4) + 8);
    setHeight((pointa.y()-4)-(pointb.y()-4) + 8);
  }
  else if (pointa.x() > pointb.x() && pointa.y() > pointb.y())
  {
    setTop(pointb.y()-4);
    setLeft(pointb.x()-4);
    setWidth((pointa.x()-4)-(pointb.x()-4) + 8);
    setHeight((pointa.y()-4)-(pointb.y()-4) + 8);
  }
}


void QnstGraphicsBind::draw(QPainter* painter)
{
  // if(isCondition())
  //  draw_condition(painter);
  // else
  draw_action(painter);
}

void QnstGraphicsBind::draw_action(QPainter* painter)
{
  if (getEntityA() != NULL && getEntityB() != NULL)
  {
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    QLineF line = QLineF(QPointF(getEntityA()->getLeft() + getEntityA()->getWidth()/2,
                                 getEntityA()->getTop() + getEntityA()->getHeight()/2),
                         QPointF(getEntityB()->getLeft() + getEntityB()->getWidth()/2,
                                 getEntityB()->getTop() + getEntityB()->getHeight()/2));

    if (getEntityA()->getncgType() == Qncg::Interface)
    {
      line.setP1(getnstGraphicsParent()->mapFromItem(getEntityA()->getnstGraphicsParent(), line.p1()));
    }

    if (getEntityB()->getncgType() == Qncg::Interface)
    {
      line.setP2(getnstGraphicsParent()->mapFromItem(getEntityB()->getnstGraphicsParent(), line.p2()));
    }

    QPointF pointa = line.p1();
    QPointF pointb = line.p2();

    if (!isInvalid())
      painter->setPen(QPen(QBrush(QColor("#000000")), 1));
    else
      painter->setPen(QPen(QBrush(QColor(255,0,0,200)), 1, Qt::DashLine));

    if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y())
    {
      if (getAngle() != 0)
      {
        qreal drawangle = getAdjAngle();

        QLineF localline(4+4,4+4, 4+8+getWidth()-16, 4+8+getHeight()-16);

        if (drawangle < 0)
        {
          drawangle = -drawangle;
        }

        qreal R = localline.length()/(::sin(((drawangle/2)*PI)/180)*2);

        qreal delta = (180-drawangle)/2 + (360 - localline.angle());

        QPointF center_a(localline.p2().x() - ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p2().y() + ::sin((180-delta-drawangle)*PI/180)*R);

        QPointF center_b(localline.p1().x() + ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p1().y() - ::sin((180-delta-drawangle)*PI/180)*R);

        if (getAdjAngle() < 0)
        {
          painter->drawArc(center_b.x()-R,center_b.y()-R,2*R,2*R,
                           16*((180-delta-drawangle)+180),16*drawangle);
        }
        else
        {
          painter->drawArc(center_a.x()-R,center_a.y()-R,2*R,2*R
                           ,16*(180-delta-drawangle),16*drawangle);
        }
      }
      else
      {
        painter->drawLine(4+4,4+4, 4+8+getWidth()-16, 4+8+getHeight()-16);
      }

      painter->setPen(Qt::NoPen);

      painter->drawPixmap(4+getWidth()-16, 4+getHeight()-16, 16, 16, QPixmap(icon));

      if (!isInvalid())
      {
        painter->setBrush(QBrush(QColor("#000000")));
      }
      else
      {
        painter->setBrush(QBrush(QColor(255,0,0,75)));
        painter->drawEllipse(3+getWidth()-16, 3+getHeight()-16, 18, 18);
      }
    }
    else if (pointa.x() > pointb.x() && pointa.y() <= pointb.y())
    {
      if (getAngle() != 0)
      {
        qreal drawangle = getAdjAngle();

        QLineF localline(4+4+getWidth()-8,4+4, 4+8, 4+8+getHeight()-16);

        if (drawangle < 0)
        {
          drawangle = -drawangle;
        }

        qreal R = localline.length()/(::sin(((drawangle/2)*PI)/180)*2);

        qreal delta = (180-drawangle)/2 + (360 - localline.angle());

        QPointF center_a(localline.p2().x() - ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p2().y() + ::sin((180-delta-drawangle)*PI/180)*R);

        QPointF center_b(localline.p1().x() + ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p1().y() - ::sin((180-delta-drawangle)*PI/180)*R);

        if (getAdjAngle() < 0)
        {
          painter->drawArc(center_b.x()-R,center_b.y()-R,2*R,2*R,
                           16*((180-delta-drawangle)+180),16*drawangle);
        }
        else
        {
          painter->drawArc(center_a.x()-R,center_a.y()-R,2*R,2*R
                           ,16*(180-delta-drawangle),16*drawangle);
        }
      }
      else
      {
        painter->drawLine(4+4+getWidth()-8,4+4, 4+8, 4+8+getHeight()-16);
      }

      painter->setPen(Qt::NoPen);

      painter->drawPixmap(4, 4+getHeight()-16, 16, 16, QPixmap(icon));

      if (!isInvalid())
      {
        painter->setBrush(QBrush(QColor("#000000")));
      }
      else
      {
        painter->setBrush(QBrush(QColor(255,0,0,75)));
        painter->drawEllipse(3, 3+getHeight()-16, 18, 18);
      }
    }
    else if (pointa.x() <= pointb.x() && pointa.y() > pointb.y())
    {
      if (getAngle() != 0)
      {
        qreal drawangle = getAdjAngle();

        QLineF localline(4+4, 4+4+getHeight()-8, 4+8+getWidth()-16, 4+8);

        if (drawangle < 0)
        {
          drawangle = -drawangle;
        }

        qreal R = localline.length()/(::sin(((drawangle/2)*PI)/180)*2);

        qreal delta = (180-drawangle)/2 + (360 - localline.angle());

        QPointF center_a(localline.p2().x() - ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p2().y() + ::sin((180-delta-drawangle)*PI/180)*R);

        QPointF center_b(localline.p1().x() + ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p1().y() - ::sin((180-delta-drawangle)*PI/180)*R);

        if (getAdjAngle() < 0)
        {
          painter->drawArc(center_b.x()-R,center_b.y()-R,2*R,2*R,
                           16*((180-delta-drawangle)+180),16*drawangle);
        }
        else
        {
          painter->drawArc(center_a.x()-R,center_a.y()-R,2*R,2*R
                           ,16*(180-delta-drawangle),16*drawangle);
        }
      }
      else
      {
        painter->drawLine(4+4, 4+4+getHeight()-8, 4+8+getWidth()-16, 4+8);
      }

      painter->setPen(Qt::NoPen);

      painter->drawPixmap(4+getWidth()-16,4,16,16, QPixmap(icon));

      if (!isInvalid())
      {
        painter->setBrush(QBrush(QColor("#000000")));
      }
      else
      {
        painter->setBrush(QBrush(QColor(255,0,0,75)));
        painter->drawEllipse(3+getWidth()-16,3,18,18);
      }
    }
    else if (pointa.x() > pointb.x() && pointa.y() > pointb.y())
    {
      if (getAngle() != 0){
        qreal drawangle = getAdjAngle();

        QLineF localline(4+4+getWidth()-8, 4+4+getHeight()-8, 4+8, 4+8);

        if (drawangle < 0)
        {
          drawangle = -drawangle;
        }

        qreal R = localline.length()/(::sin(((drawangle/2)*PI)/180)*2);

        qreal delta = (180-drawangle)/2 + (360 - localline.angle());

        QPointF center_a(localline.p2().x() - ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p2().y() + ::sin((180-delta-drawangle)*PI/180)*R);

        QPointF center_b(localline.p1().x() + ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p1().y() - ::sin((180-delta-drawangle)*PI/180)*R);

        if (getAdjAngle() < 0)
        {
          painter->drawArc(center_b.x()-R,center_b.y()-R,2*R,2*R,
                           16*((180-delta-drawangle)+180),16*drawangle);
        }
        else
        {
          painter->drawArc(center_a.x()-R,center_a.y()-R,2*R,2*R
                           ,16*(180-delta-drawangle),16*drawangle);
        }
      }
      else
      {
        painter->drawLine(4+4+getWidth()-8, 4+4+getHeight()-8, 4+8, 4+8);
      }

      painter->setPen(Qt::NoPen);

      painter->drawPixmap(4,4,16,16, QPixmap(icon));

      if (!isInvalid())
      {
        painter->setBrush(QBrush(QColor("#000000")));
      }
      else
      {
        painter->setBrush(QBrush(QColor(255,0,0,75)));
        painter->drawEllipse(3,3,18,18);
      }
    }
  }
}

void QnstGraphicsBind::draw_condition(QPainter* painter)
{
  if (getEntityA() != NULL && getEntityB() != NULL)
  {
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    QLineF line = QLineF(QPointF(getEntityA()->getLeft() + getEntityA()->getWidth()/2,
                                 getEntityA()->getTop() + getEntityA()->getHeight()/2),
                         QPointF(getEntityB()->getLeft() + getEntityB()->getWidth()/2,
                                 getEntityB()->getTop() + getEntityB()->getHeight()/2));

    if (getEntityA()->getncgType() == Qncg::Interface)
    {
      line.setP1(getnstGraphicsParent()->mapFromItem(getEntityA()->getnstGraphicsParent(), line.p1()));
    }

    if (getEntityB()->getncgType() == Qncg::Interface)
    {
      line.setP2(getnstGraphicsParent()->mapFromItem(getEntityB()->getnstGraphicsParent(), line.p2()));
    }

    QPointF pointa = line.p1();
    QPointF pointb = line.p2();

    if (!isInvalid())
      painter->setPen(QPen(QBrush(QColor("#000000")), 1));
    else
      painter->setPen(QPen(QBrush(QColor(255,0,0,200)), 1, Qt::DashLine));

    if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y())
    {
      if (getAngle() != 0)
      {
        qreal drawangle = getAdjAngle();

        QLineF localline(4+8,4+8, 4+getWidth()-4, 4+getHeight()-4);

        if (drawangle < 0)
        {
          drawangle = -drawangle;
        }

        qreal R = localline.length()/(::sin(((drawangle/2)*PI)/180)*2);

        qreal delta = (180-drawangle)/2 + (360 - localline.angle());

        QPointF center_a(localline.p2().x() - ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p2().y() + ::sin((180-delta-drawangle)*PI/180)*R);

        QPointF center_b(localline.p1().x() + ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p1().y() - ::sin((180-delta-drawangle)*PI/180)*R);

        if (getAdjAngle() < 0)
        {
          painter->drawArc(center_b.x()-R,center_b.y()-R,2*R,2*R,
                           16*((180-delta-drawangle)+180),16*drawangle);
        }
        else
        {
          painter->drawArc(center_a.x()-R,center_a.y()-R,2*R,2*R
                           ,16*(180-delta-drawangle),16*drawangle);
        }
      }
      else
      {
        painter->drawLine(4+8,4+8, 4+getWidth()-4, 4+getHeight()-4);
      }

      painter->setPen(Qt::NoPen);

      painter->drawPixmap(4,4,16,16, QPixmap(icon));

      if (!isInvalid())
      {
        painter->setBrush(QBrush(QColor("#000000")));
      }
      else
      {
        painter->setBrush(QBrush(QColor(255,0,0,75)));
        painter->drawEllipse(3,3, 18, 18);
      }
    }
    else if (pointa.x() >= pointb.x() && pointa.y() <= pointb.y())
    {
      if (getAngle() != 0)
      {
        qreal drawangle = getAdjAngle();

        QLineF localline(4+getWidth()-8,4+8, 4+4, 4+getHeight()-4);

        if (drawangle < 0)
        {
          drawangle = -drawangle;
        }

        qreal R = localline.length()/(::sin(((drawangle/2)*PI)/180)*2);

        qreal delta = (180-drawangle)/2 + (360 - localline.angle());

        QPointF center_a(localline.p2().x() - ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p2().y() + ::sin((180-delta-drawangle)*PI/180)*R);

        QPointF center_b(localline.p1().x() + ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p1().y() - ::sin((180-delta-drawangle)*PI/180)*R);

        if (getAdjAngle() < 0)
        {
          painter->drawArc(center_b.x()-R,center_b.y()-R,2*R,2*R,
                           16*((180-delta-drawangle)+180),16*drawangle);
        }
        else
        {
          painter->drawArc(center_a.x()-R,center_a.y()-R,2*R,2*R
                           ,16*(180-delta-drawangle),16*drawangle);
        }
      }
      else
      {
        painter->drawLine(4+getWidth()-8,4+8, 4+4, 4+getHeight()-4);
      }

      painter->setPen(Qt::NoPen);

      painter->drawPixmap(4+getWidth()-16,4,16,16, QPixmap(icon));

      if (!isInvalid())
      {
        painter->setBrush(QBrush(QColor("#000000")));
      }
      else
      {
        painter->setBrush(QBrush(QColor(255,0,0,75)));
        painter->drawEllipse(3+getWidth()-16,3, 18, 18);
      }
    }
    else if (pointa.x() < pointb.x() && pointa.y() > pointb.y())
    {
      if (getAngle() != 0)
      {
        qreal drawangle = getAdjAngle();
        QLineF localline(4+8, 4+getHeight()-8, 4+getWidth()-4, 4+4);

        if (drawangle < 0)
        {
          drawangle = -drawangle;
        }

        qreal R = localline.length()/(::sin(((drawangle/2)*PI)/180)*2);

        qreal delta = (180-drawangle)/2 + (360 - localline.angle());

        QPointF center_a(localline.p2().x() - ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p2().y() + ::sin((180-delta-drawangle)*PI/180)*R);

        QPointF center_b(localline.p1().x() + ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p1().y() - ::sin((180-delta-drawangle)*PI/180)*R);

        if (getAdjAngle() < 0)
        {
          painter->drawArc(center_b.x()-R,center_b.y()-R,2*R,2*R,
                           16*((180-delta-drawangle)+180),16*drawangle);
        }
        else
        {
          painter->drawArc(center_a.x()-R,center_a.y()-R,2*R,2*R
                           ,16*(180-delta-drawangle),16*drawangle);
        }
      }
      else
      {
        painter->drawLine(4+8, 4+getHeight()-8, 4+getWidth()-4, 4+4);
      }

      painter->setPen(Qt::NoPen);

      painter->drawPixmap(4, 4+getHeight()-16, 16, 16, QPixmap(icon));

      if (!isInvalid())
      {
        painter->setBrush(QBrush(QColor("#000000")));
      }
      else
      {
        painter->setBrush(QBrush(QColor(255,0,0,75)));
        painter->drawEllipse(3, 3+getHeight()-16, 18, 18);
      }
    }
    else if (pointa.x() > pointb.x() && pointa.y() > pointb.y())
    {
      if (getAngle() != 0)
      {
        qreal drawangle = getAdjAngle();

        QLineF localline(4+getWidth()-8, 4+getHeight()-8, 4+4, 4+4);

        if (drawangle < 0)
        {
          drawangle = -drawangle;
        }

        qreal R = localline.length()/(::sin(((drawangle/2)*PI)/180)*2);

        qreal delta = (180-drawangle)/2 + (360 - localline.angle());

        QPointF center_a(localline.p2().x() - ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p2().y() + ::sin((180-delta-drawangle)*PI/180)*R);

        QPointF center_b(localline.p1().x() + ::cos((180-delta-drawangle)*PI/180)*R,
                         localline.p1().y() - ::sin((180-delta-drawangle)*PI/180)*R);

        if (getAdjAngle() < 0)
        {
          painter->drawArc(center_b.x()-R,center_b.y()-R,2*R,2*R,
                           16*((180-delta-drawangle)+180),16*drawangle);
        }
        else
        {
          painter->drawArc(center_a.x()-R,center_a.y()-R,2*R,2*R
                           ,16*(180-delta-drawangle),16*drawangle);
        }
      }
      else
      {
        painter->drawLine(4+getWidth()-8, 4+getHeight()-8, 4+4, 4+4);
      }

      painter->setPen(Qt::NoPen);

      painter->drawPixmap(4+getWidth()-16, 4+getHeight()-16, 16, 16, QPixmap(icon));

      if (!isInvalid())
      {
        painter->setBrush(QBrush(QColor("#000000")));
      }
      else
      {
        painter->setBrush(QBrush(QColor(255,0,0,75)));
        painter->drawEllipse(3+getWidth()-16, 3+getHeight()-16, 18, 18);
      }
    }
  }
}

void QnstGraphicsBind::delineate(QPainterPath* painter) const
{
  // if(isCondition())
  //  delineate_condition(painter);
  // else
  delineate_action(painter);
}

void QnstGraphicsBind::delineate_action(QPainterPath* painter) const
{
  if (getEntityA() != NULL && getEntityB() != NULL)
  {
    QLineF line = QLineF(QPointF(getEntityA()->getLeft() + getEntityA()->getWidth()/2,
                                 getEntityA()->getTop() + getEntityA()->getHeight()/2),
                         QPointF(getEntityB()->getLeft() + getEntityB()->getWidth()/2,
                                 getEntityB()->getTop() + getEntityB()->getHeight()/2));

    if (getEntityA()->getncgType() == Qncg::Interface)
    {
      line.setP1(getnstGraphicsParent()->mapFromItem(getEntityA()->getnstGraphicsParent(), line.p1()));
    }

    if (getEntityB()->getncgType() == Qncg::Interface)
    {
      line.setP2(getnstGraphicsParent()->mapFromItem(getEntityB()->getnstGraphicsParent(), line.p2()));
    }

    QPointF pointa = line.p1();
    QPointF pointb = line.p2();

    if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y())
    {
      painter->addEllipse(4+getWidth()-16, 4+getHeight()-16, 16, 16);
    }
    else if (pointa.x() > pointb.x() && pointa.y() < pointb.y())
    {
      painter->addEllipse(4, 4+getHeight()-16, 16, 16);
    }
    else if (pointa.x() < pointb.x() && pointa.y() > pointb.y())
    {
      painter->addEllipse(4+getWidth()-16,4,16,16);
    }
    else if (pointa.x() > pointb.x() && pointa.y() > pointb.y())
    {
      painter->addEllipse(4,4,16,16);
    }
  }
}

void QnstGraphicsBind::delineate_condition(QPainterPath* painter) const
{
  if (getEntityA() != NULL && getEntityB() != NULL)
  {
    QLineF line = QLineF(QPointF(getEntityA()->getLeft() + getEntityA()->getWidth()/2,
                                 getEntityA()->getTop() + getEntityA()->getHeight()/2),
                         QPointF(getEntityB()->getLeft() + getEntityB()->getWidth()/2,
                                 getEntityB()->getTop() + getEntityB()->getHeight()/2));

    if (getEntityA()->getncgType() == Qncg::Interface)
    {
      line.setP1(getnstGraphicsParent()->mapFromItem(getEntityA()->getnstGraphicsParent(), line.p1()));
    }

    if (getEntityB()->getncgType() == Qncg::Interface)
    {
      line.setP2(getnstGraphicsParent()->mapFromItem(getEntityB()->getnstGraphicsParent(), line.p2()));
    }

    QPointF pointa = line.p1();
    QPointF pointb = line.p2();

    if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y())
    {
      //        painter->drawLine(4+4,4+4, 4+4+getWidth()-8-2, 4+4+getHeight()-8-2);

      painter->addEllipse(4,4,16,16);

    }
    else if (pointa.x() > pointb.x() && pointa.y() < pointb.y())
    {
      //        painter->drawLine(4+4+getWidth()-8,4+4, 4+4, 4+4+getHeight()-8);

      painter->addEllipse(4+getWidth()-16,4,16,16);


    }
    else if (pointa.x() < pointb.x() && pointa.y() > pointb.y())
    {
      //        painter->drawLine(4+4, 4+4+getHeight()-8, 4+4+getWidth()-8, 4+4);

      painter->addEllipse(4, 4+getHeight()-16, 16, 16);

    }
    else if (pointa.x() > pointb.x() && pointa.y() > pointb.y())
    {
      //        painter->drawLine(4+4+getWidth()-8, 4+4+getHeight()-8, 4+4, 4+4);

      painter->addEllipse(4+getWidth()-16, 4+getHeight()-16, 16, 16);
    }
  }
}

void QnstGraphicsBind::addParam(QString uid, QString name, QString value)
{
  if (name != "")
  {
    params[name] = value;
    names_uids[name] = uid;

    emit bindParamUpdated(getnstUid(), params, names_uids);
  }
}

void QnstGraphicsBind::setParam(QString name, QString value)
{
  if (name != "")
  {
    params[name] = value;

    emit bindParamUpdated(getnstUid(), params, names_uids);
  }
}

void QnstGraphicsBind::removeUId(QString uid)
{
  QString name = names_uids.key(uid);

  if (params.contains(name))
  {
    params.remove(name);
    names_uids.remove(name);

    emit bindParamUpdated(getnstUid(), params, names_uids);
  }
}

void QnstGraphicsBind::removeParam(QString name)
{
  if(name != "")
  {
    params.remove(name);
    names_uids.remove(name);

    emit bindParamUpdated(getnstUid(), params, names_uids);
  }
}

void QnstGraphicsBind::setConn(QnstConnector* conn)
{
  this->conn = conn;
}

void QnstGraphicsBind::setParams(QMap<QString, QString> params)
{
  this->params = params;
}

void QnstGraphicsBind::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  if (conn != NULL)
  {
    QString UID = "";
    QMap<QString, QString> values;

    if (params.isEmpty())
    {
      if (isAction())
      {
        foreach(QString type, conn->getActions().values())
        {
          if (type == QnstUtil::getStrFromBindType(getType()))
          {
            UID = conn->getActions().key(type,"");
          }
        }

        QMap<QPair<QString, QString>, QString> parameters = conn->getActionParams();

        QPair<QString, QString> key;

        foreach(key , parameters.keys())
        {
          if (key.first == UID)
          {
            values[parameters.value(key)] = params.value(parameters.value(key),"");
          }
        }



      }
      else if (isCondition())
      {
        foreach(QString type, conn->getConditions().values())
        {
          if (type == QnstUtil::getStrFromBindType(getType()))
          {
            UID = conn->getConditions().key(type,"");
          }
        }

        QMap<QPair<QString, QString>, QString> parameters = conn->getConditionParams();

        QPair<QString, QString> key;

        foreach(key , parameters.keys())
        {
          if (key.first == UID)
          {
            values[parameters.value(key)] = params.value(parameters.value(key),"");
          }
        }


      }
    }
    else
    {
      values = params;
    }

    dialog->init(values);

    if (dialog->exec())
    {
      params = dialog->getProperties();

      foreach(QString name, params.keys())
      {
        QMap<QString, QString> p;

        p["name"] = name;
        p["value"] = params[name];

        if (names_uids[name].isEmpty())
        {
          names_uids[name] = QUuid::createUuid().toString();
        }

        emit bindParamAdded(names_uids[name], getnstUid(), p);
      }

      emit bindParamUpdated(getnstUid(), params, names_uids);
    }
  }
  else
  {
    qWarning() << "[QNST] Connector is NULL. I can not load connectorParams";
  }
}

bool QnstGraphicsBind::isCondition() const
{
  return (getnstType() == Qnst::Condition);
}

bool QnstGraphicsBind::isAction() const
{
  return !isCondition();
}

QString QnstGraphicsBind::getRole() const
{
  return role;
}

void QnstGraphicsBind::setRole(QString role)
{
  this->role = role;
}

QString QnstGraphicsBind::getComponent() const
{
  return component;
}

void QnstGraphicsBind::setComponent(QString component)
{
  this->component = component;
}

QString QnstGraphicsBind::getComponentUid() const
{
  return componentUID;
}

void QnstGraphicsBind::setComponentUid(QString componentUID)
{
  this->componentUID = componentUID;
}

QString QnstGraphicsBind::getInterface() const
{
  return interface;
}

void QnstGraphicsBind::setInterface(QString interface)
{
  this->interface = interface;
}

QString QnstGraphicsBind::getInterfaceUid() const
{
  return interfaceUID;
}

void QnstGraphicsBind::setInterfaceUid(QString interfaceUID)
{
  this->interfaceUID = interfaceUID;
}

QMap<QString, QString> QnstGraphicsBind::getParams()
{
  return params;
}

QMap<QString, QString> QnstGraphicsBind::getNameUIDs()
{
  return names_uids;
}

void QnstGraphicsBind::setNamesUIDs(QMap<QString, QString> names_uids)
{
  this->names_uids = names_uids;
}

void QnstGraphicsBind::setLink(QnstGraphicsLink *link)
{
  this->link = link;
  setEntityA(link);
}

QnstGraphicsLink* QnstGraphicsBind::getLink()
{
  return this->link;
}

void QnstGraphicsBind::setTarget(QnstGraphicsEntity *target)
{
  setEntityB(target);
}

QnstGraphicsEntity *QnstGraphicsBind::getTarget()
{
  return getEntityB();
}

void QnstGraphicsBind::setProperties(const QMap<QString, QString> &properties)
{
  QnstGraphicsEdge::setProperties(properties);

  if(properties.contains("role"))
  {
    setRole(properties["role"]);
    setType(QnstUtil::getBindTypeFromStr(getRole()));
  }

  if(properties.contains("component"))
    setComponent("component");

  if(properties.contains("componentUid"))
    setComponentUid(properties["componentUid"]);

  if(properties.contains("interface"))
    setInterface(properties["interface"]);

  if(properties.contains("interfaceUid"))
    setInterfaceUid(properties["interfaceUid"]);
}

void QnstGraphicsBind::getProperties(QMap<QString, QString> &properties)
{
  QnstGraphicsEdge::getProperties(properties);

  properties["role"] = getRole();
  properties["component"] = getComponent();
  properties["componentUid"] = getComponentUid();
  properties["interface"] = getInterface();
  properties["interfaceUid"] = getComponentUid();
}
