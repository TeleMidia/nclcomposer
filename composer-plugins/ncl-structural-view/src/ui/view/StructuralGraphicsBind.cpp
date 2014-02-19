#include "StructuralGraphicsBind.h"

#include <assert.h>

StructuralGraphicsBind::StructuralGraphicsBind(StructuralGraphicsEntity* parent)
  : StructuralGraphicsEdge(parent)
{
  // \todo We should have only Qnst::Bind
  setnstType(Structural::Condition);

  setType(Structural::NoBindType);

  dialog = new StructuralGraphicsBindDialog();

  conn = NULL;

  link = NULL;
}

StructuralGraphicsBind::~StructuralGraphicsBind()
{
  delete dialog;
}

void StructuralGraphicsBind::setType(Structural::BindType type)
{
  this->type = type;

  switch(type)
  {
    //ACTIONS
    case Structural::Start:
      setnstType(Structural::Action);
      this->icon = ":/icon/start";
      break;

    case Structural::Stop:
      setnstType(Structural::Action);
      this->icon = ":/icon/stop";
      break;

    case Structural::Resume:
      setnstType(Structural::Action);
      this->icon = ":/icon/resume";
      break;

    case Structural::Pause:
      setnstType(Structural::Action);
      this->icon = ":/icon/pause";
      break;

    case Structural::Set:
      setnstType(Structural::Action);
      this->icon = ":/icon/set";
      break;

    //CONDITIONS
    case Structural::onBegin:
      setnstType(Structural::Condition);
      this->icon = ":/icon/onbegin";
      break;

    case Structural::onEnd:
      setnstType(Structural::Condition);
      this->icon = ":/icon/onend";
      break;

    case Structural::onResume:
      setnstType(Structural::Condition);
      this->icon = ":/icon/onresume";
      break;

    case Structural::onPause:
      setnstType(Structural::Condition);
      this->icon = ":/icon/onpause";
      break;

    case Structural::onSelection:
      setnstType(Structural::Condition);
      this->icon = ":/icon/onselection";
      break;

    default:
      if(getnstType() == Structural::Action)
        this->icon = ":/icon/noaction";
      else
        this->icon = ":/icon/nocondition";
      break;
  }
}

Structural::BindType StructuralGraphicsBind::getType()
{
  return type;
}

QString StructuralGraphicsBind::getIcon()
{
  return icon;
}

//void QnstGraphicsBind::adjust()
//{
  //if(isCondition())
  //  adjust_condition();
  //else
    //adjust_action();
//}

void StructuralGraphicsBind::adjust_action()
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

void StructuralGraphicsBind::adjust_condition()
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

void StructuralGraphicsBind::aux_adjust(QPointF pointa, QPointF pointb)
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


void StructuralGraphicsBind::draw(QPainter* painter)
{
  drawMouseHoverHighlight(painter); // This should not be HERE!!

  //if(isCondition())
  //  draw_condition(painter);
  //else
    draw_action(painter);
}

void StructuralGraphicsBind::draw_action(QPainter* painter)
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

    if (!isInvalid() && !hasError)
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

      if (!isInvalid() && !hasError)
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

      if (!isInvalid() && !hasError)
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

      if (!isInvalid() && !hasError)
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

      if (!isInvalid() && !hasError)
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

void StructuralGraphicsBind::draw_condition(QPainter* painter)
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

    if (!isInvalid() && !hasError)
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

      if (!isInvalid() && !hasError)
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

      if (!isInvalid() && !hasError)
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

      if (!isInvalid() && !hasError)
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

      if (!isInvalid() && !hasError)
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

void StructuralGraphicsBind::delineate(QPainterPath* painter) const
{
  // if(isCondition())
  //  delineate_condition(painter);
  // else
  delineate_action(painter);
}

void StructuralGraphicsBind::delineate_action(QPainterPath* painter) const
{
  // \fixme This should never reach here with dome of the following NULL
 /*
  assert (getEntityA() != NULL);
  assert (getEntityA()->getnstGraphicsParent() != NULL);
  assert (getEntityB() != NULL);
  assert (getnstGraphicsParent() != NULL);
  assert (getEntityB()->getnstGraphicsParent() != NULL); */

  if (getEntityA() != NULL && getEntityB() != NULL && getnstGraphicsParent() != NULL)
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

void StructuralGraphicsBind::delineate_condition(QPainterPath* painter) const
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

void StructuralGraphicsBind::addParam(QString uid, QString name, QString value)
{
  if (name != "")
  {
    params[name] = value;
    names_uids[name] = uid;

    emit bindParamUpdated(getnstUid(), params, names_uids);
  }
}

void StructuralGraphicsBind::setParam(QString name, QString value)
{
  if (name != "")
  {
    params[name] = value;

    emit bindParamUpdated(getnstUid(), params, names_uids);
  }
}

void StructuralGraphicsBind::removeUId(QString uid)
{
  QString name = names_uids.key(uid);

  if (params.contains(name))
  {
    params.remove(name);
    names_uids.remove(name);

    emit bindParamUpdated(getnstUid(), params, names_uids);
  }
}

void StructuralGraphicsBind::removeParam(QString name)
{
  if(name != "")
  {
    params.remove(name);
    names_uids.remove(name);

    emit bindParamUpdated(getnstUid(), params, names_uids);
  }
}

void StructuralGraphicsBind::setConn(StructuralConnector* conn)
{
  this->conn = conn;
}

void StructuralGraphicsBind::setParams(QMap<QString, QString> params)
{
  this->params = params;
}

void StructuralGraphicsBind::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  if (conn != NULL)
  {
    QString UID = "";
    QMap<QString, QString> values;

    if (params.isEmpty())
    {
      if (isAction())
      {
        /*
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
        */

        QMap<QPair<QString, QString>, QString> parameters = conn->getActionParams();

        QPair<QString, QString> key;
        foreach(key , parameters.keys())
        {
          values[parameters[key]] = "";
        }

      }
      else if (isCondition())
      {
        /*
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
        */
        QMap<QPair<QString, QString>, QString> parameters = conn->getConditionParams();

        QPair<QString, QString> key;
        foreach(key , parameters.keys())
        {
          values[parameters[key]] = "";
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

bool StructuralGraphicsBind::isCondition() const
{
  return (getnstType() == Structural::Condition);
}

bool StructuralGraphicsBind::isAction() const
{
  return !isCondition();
}

QString StructuralGraphicsBind::getRole() const
{
  return role;
}

void StructuralGraphicsBind::setRole(QString role)
{
  this->role = role;
}

QString StructuralGraphicsBind::getComponent() const
{
  return component;
}

void StructuralGraphicsBind::setComponent(QString component)
{
  this->component = component;
}

QString StructuralGraphicsBind::getComponentUid() const
{
  return componentUID;
}

void StructuralGraphicsBind::setComponentUid(QString componentUID)
{
  this->componentUID = componentUID;
}

QString StructuralGraphicsBind::getInterface() const
{
  return interface;
}

void StructuralGraphicsBind::setInterface(QString interface)
{
  this->interface = interface;
}

QString StructuralGraphicsBind::getInterfaceUid() const
{
  return interfaceUID;
}

void StructuralGraphicsBind::setInterfaceUid(QString interfaceUID)
{
  this->interfaceUID = interfaceUID;
}

QMap<QString, QString> StructuralGraphicsBind::getParams()
{
  return params;
}

QMap<QString, QString> StructuralGraphicsBind::getNameUIDs()
{
  return names_uids;
}

void StructuralGraphicsBind::setNamesUIDs(QMap<QString, QString> names_uids)
{
  this->names_uids = names_uids;
}

void StructuralGraphicsBind::setLink(StructuralGraphicsLink *link)
{
  this->link = link;
  setEntityA(link);
}

StructuralGraphicsLink* StructuralGraphicsBind::getLink()
{
  return this->link;
}

void StructuralGraphicsBind::setTarget(StructuralGraphicsEntity *target)
{
  setEntityB(target);
}

StructuralGraphicsEntity *StructuralGraphicsBind::getTarget()
{
  return getEntityB();
}

void StructuralGraphicsBind::setProperties(const QMap<QString, QString> &properties)
{
  StructuralGraphicsEdge::setProperties(properties);

  if(properties.contains("role"))
  {
    setRole(properties["role"]);
    setType(StructuralUtil::getBindTypeFromStr(getRole()));
  }

  if(properties.contains("component"))
    setComponent(properties["component"]);

  if(properties.contains("componentUid"))
    setComponentUid(properties["componentUid"]);

  if(properties.contains("interface"))
    setInterface(properties["interface"]);

  if(properties.contains("interfaceUid"))
    setInterfaceUid(properties["interfaceUid"]);
}

void StructuralGraphicsBind::getProperties(QMap<QString, QString> &properties)
{
  StructuralGraphicsEdge::getProperties(properties);

  properties["role"] = getRole();
  properties["component"] = getComponent();
  properties["componentUid"] = getComponentUid();
  properties["interface"] = getInterface();
  properties["interfaceUid"] = getComponentUid();
}

void StructuralGraphicsBind::updateToolTip()
{
  QString tip = getRole() + "(" + getComponent();
  if(getInterface() != "")
  {
    tip += "#";
    tip += getInterface();
  }
  tip += ")";

  setToolTip(tip);
}
