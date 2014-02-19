#include "StructuralMedia.h"

#include <QDrag>

#include "StructuralUtil.h"

StructuralMedia::StructuralMedia(StructuralEntity* parent)
  : StructuralNode(parent), enableDrag(false)
{
  setnstSubtype(Structural::Media);

  setResizable(false);

  createObjects();
  createConnections();

  setnstId("");

  /* Default media position */
  if(parent)
  {
    setTop(parent->getHeight()/2 - DEFAULT_MEDIA_HEIGHT/2);
    setLeft(parent->getWidth()/2 - DEFAULT_MEDIA_WIDTH/2);
  }
  else
  {
    setTop(0);
    setLeft(0);
  }

  setWidth(DEFAULT_MEDIA_WIDTH);
  setHeight(DEFAULT_MEDIA_HEIGHT);
}

StructuralMedia::~StructuralMedia()
{

}

QString StructuralMedia::getIcon() const
{
  return icon;
}

QString StructuralMedia::getSource() const
{
  return source;
}

void StructuralMedia::setSource(QString source)
{
  this->source = source;
}

void StructuralMedia::setnstSubtype(QnstSubtype type)
{
  StructuralNode::setnstSubtype(type);

  this->icon = StructuralUtil::iconFromMediaType(type);
}

void StructuralMedia::updateToolTip()
{
  QString tip = "";
  QString name = (getnstProperty(":nst:id") != "" ? getnstProperty(":nst:id") : "?");

  /*
  switch(getnstSubtype())
  {
    case Qnst::Image:
      tip += "Image ("+name+")";
      break;

    case Qnst::Audio:
      tip += "Audio ("+name+")";
      break;

    case Qnst::Text:
      tip += "Text ("+name+")";
      break;

    case Qnst::Video:
      tip += "Video ("+name+")";
      break;

    case Qnst::NCLua:
      tip += "NCLua ("+name+")";
      break;

    case Qnst::Html:
      tip += "HTML ("+name+")";
      break;

    case Qnst::NCL:
      tip += "NCL ("+name+")";
      break;

    case Qnst::Settings:
      tip += "Settings ("+name+")";
      break;

    default:
      tip += "Media ("+name+")";
      break;
  }
  */

  tip += "Media ("+name+")";

  if (hasError)
    tip += " - Error: " + erroMsg;
  else if (getSource().isEmpty())
    tip += " - Alert: Missing 'src' attribute";

  setToolTip(tip);
}

QString StructuralMedia::getRefer() const
{
  return refer;
}

void StructuralMedia::setRefer(QString refer)
{
  this->refer = refer;
}

QString StructuralMedia::getReferUID() const
{
  return referUID;
}

void StructuralMedia::setReferUID(QString refetUID)
{
  this->referUID = refetUID;
}

QString StructuralMedia::getInstance() const
{
  return instance;
}

void StructuralMedia::setInstance(QString instance)
{
  this->instance = instance;
}

void StructuralMedia::createObjects()
{
  menu = new StructuralMenu();
  menu->actionCut->setEnabled(true);
  menu->actionCopy->setEnabled(true);

  menu->actionDelete->setEnabled(true);

  menu->actionExport->setEnabled(true);

  menu->menuInsert->setEnabled(true);
  menu->actionAddArea->setEnabled(true);
  menu->actionAddProperty->setEnabled(true);
}

void StructuralMedia::createConnections()
{
  // connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
  // connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));


  connect(menu, SIGNAL(menuAddEntityTriggered(QnstSubtype)),
          SLOT(newChild(QnstSubtype)));
}

void StructuralMedia::draw(QPainter* painter)
{
  painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

  painter->drawPixmap(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-16-8, QPixmap(icon));

  drawMouseHoverHighlight(painter); // This should not be HERE!!

  painter->setPen(QPen(QBrush(Qt::black),0));

  if(hasError)
  {
    painter->drawPixmap((getWidth()-8)/2 + 12, (getHeight()-8)/2 + 4, 12, 12, QPixmap(":/icon/delete"));
  }
  else if (getSource().isEmpty())
  {
    painter->drawPixmap((getWidth()-8)/2 + 12, (getHeight()-8)/2 + 4, 12, 12, QPixmap(":/icon/alert"));
  }

  if(!getReferUID().isEmpty() && !getReferUID().isEmpty())
  {
    painter->drawPixmap(8, getHeight()-40, 32, 32, QPixmap(":/icon/alias"));
  }

  QString localid = (getnstProperty(":nst:id") != "" ? getnstProperty(":nst:id") : "?");

  if (localid.length() > 5)
  {
    localid = getnstProperty(":nst:id").replace(3,getnstProperty(":nst:id").length()-3,"...");
  }

  // draw a formated text with underline when there is error
  if(hasError)
  {
    int N_STEPS = 20;
    int begin_w = 8;
    int end_w = getWidth();
    double current_x = begin_w;
    double step = (double) ( end_w - begin_w ) / N_STEPS;

    QPolygonF polygon;
    painter->setPen(QPen(QBrush(Qt::red), 0)); // 0px = cosmetic border
    painter->setRenderHint(QPainter::Antialiasing, true);

    for (int i = 0; i < N_STEPS; i++)
    {
      current_x = begin_w + (double) i * step;

      if( i % 2)
        polygon << QPointF( current_x, getHeight() - 3 );
      else
        polygon << QPointF( current_x, getHeight() );
    }

    painter->drawPolyline(polygon);
  }

  // draw the text
  painter->drawText(4 + 8/2, 4 + 8/2 + getHeight()-16-8, getWidth()-8, 16, Qt::AlignCenter, localid);

  if (isMoving())
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

    painter->setRenderHint(QPainter::Antialiasing,false);
    painter->drawRect(getMoveLeft()+4-getLeft(), getMoveTop()+4-getTop(), getWidth()-1, getHeight()-1);
  }
}

void StructuralMedia::delineate(QPainterPath* painter) const
{
  painter->addRect(4, 4, getWidth(), getHeight());
}


void StructuralMedia::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
//  qDebug() << "QnstGraphicsMedia::mousePressEvent" << isDraggable();
//  if(isDraggable())
//  {
//    QMimeData *data = new QMimeData;
//    data->setColorData(Qt::green);
//    data->setData("nclcomposer/mediaid", getnstProperty(":nst:uid").toAscii());
//    data->setData("nclcomposer/qnstuid", getnstUid().toAscii());

//    QDrag *drag = new QDrag(event->widget());
//    drag->setMimeData(data);
//    drag->start();
//  }
//  else
    StructuralNode::mousePressEvent(event);
}
