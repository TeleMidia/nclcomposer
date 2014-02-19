#include "StructuralGraphicsMedia.h"

#include <QDrag>

#include "StructuralUtil.h"

StructuralGraphicsMedia::StructuralGraphicsMedia(StructuralGraphicsEntity* parent)
  : StructuralGraphicsNode(parent), enableDrag(false)
{
  setnstType(Structural::Media);

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

StructuralGraphicsMedia::~StructuralGraphicsMedia()
{

}

QString StructuralGraphicsMedia::getIcon() const
{
  return icon;
}

QString StructuralGraphicsMedia::getSource() const
{
  return source;
}

void StructuralGraphicsMedia::setSource(QString source)
{
  this->source = source;
}

void StructuralGraphicsMedia::setnstType(QnstType type)
{
  StructuralGraphicsNode::setnstType(type);

  this->icon = StructuralUtil::iconFromMediaType(type);
}

void StructuralGraphicsMedia::updateToolTip()
{
  QString tip = "";
  QString name = (getnstId() != "" ? getnstId() : "?");

  switch(getnstType())
  {
    case Structural::Image:
      tip += "Image ("+name+")";
      break;

    case Structural::Audio:
      tip += "Audio ("+name+")";
      break;

    case Structural::Text:
      tip += "Text ("+name+")";
      break;

    case Structural::Video:
      tip += "Video ("+name+")";
      break;

    case Structural::NCLua:
      tip += "NCLua ("+name+")";
      break;

    case Structural::Html:
      tip += "HTML ("+name+")";
      break;

    case Structural::NCL:
      tip += "NCL ("+name+")";
      break;

    case Structural::Settings:
      tip += "Settings ("+name+")";
      break;

    case Structural::Stream:
      tip += "Stream ("+name+")";
      break;

    default:
      tip += "Media ("+name+")";
      break;
  }

  if (hasError)
    tip += " - Error: " + erroMsg;
  else if (getSource().isEmpty())
    tip += " - Alert: Missing 'src' attribute";

  setToolTip(tip);
}

QString StructuralGraphicsMedia::getRefer() const
{
  return refer;
}

void StructuralGraphicsMedia::setRefer(QString refer)
{
  this->refer = refer;
}

QString StructuralGraphicsMedia::getReferUID() const
{
  return referUID;
}

void StructuralGraphicsMedia::setReferUID(QString refetUID)
{
  this->referUID = refetUID;
}

QString StructuralGraphicsMedia::getInstance() const
{
  return instance;
}

void StructuralGraphicsMedia::setInstance(QString instance)
{
  this->instance = instance;
}

void StructuralGraphicsMedia::createObjects()
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

void StructuralGraphicsMedia::createConnections()
{
  // connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
  // connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

  connect(menu, SIGNAL(cutRequested()), SIGNAL(cutRequested()));
  connect(menu, SIGNAL(copyRequested()), SIGNAL(copyRequested()));
  connect(menu, SIGNAL(pasteRequested()), SIGNAL(pasteRequested()));

  connect(menu, SIGNAL(deleteRequested()), SIGNAL(deleteRequested()));

  connect(menu, SIGNAL(exportRequested()), SIGNAL(exportRequested()));

  connect(menu, SIGNAL(menuAddEntityTriggered(Structural::EntityType)),
          SLOT(createEntity(Structural::EntityType)));

  connect(menu, SIGNAL(zoominRequested()), SIGNAL(zoominRequested()));
  connect(menu, SIGNAL(zoomoutRequested()), SIGNAL(zoomoutRequested()));
  connect(menu, SIGNAL(zoomresetRequested()), SIGNAL(zoomresetRequested()));
  connect(menu, SIGNAL(fullscreenRequested()), SIGNAL(fullscreenRequested()));
}

void StructuralGraphicsMedia::draw(QPainter* painter)
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

  QString localid = (getnstId() != "" ? getnstId() : "?");

  if (localid.length() > 5)
  {
    localid = getnstId().replace(3,getnstId().length()-3,"...");
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

void StructuralGraphicsMedia::delineate(QPainterPath* painter) const
{
  painter->addRect(4, 4, getWidth(), getHeight());
}


void StructuralGraphicsMedia::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  qDebug() << "QnstGraphicsMedia::mousePressEvent" << isDraggable();
  if(isDraggable())
  {
    QMimeData *data = new QMimeData;
    data->setColorData(QColor(Qt::green));
    data->setData("nclcomposer/mediaid", getnstId().toLatin1());
    data->setData("nclcomposer/qnstuid", getnstUid().toLatin1());

    QDrag *drag = new QDrag(event->widget());
    drag->setMimeData(data);
    drag->start();
  }
  else
    StructuralGraphicsNode::mousePressEvent(event);
}

void StructuralGraphicsMedia::setProperties(const QMap<QString, QString> &properties)
{
  StructuralGraphicsNode::setProperties(properties);

  setSource(properties["src"]);
  setRefer(properties["refer"]);
  setReferUID(properties["referUID"]);
  setInstance(properties["instance"]);
}

void StructuralGraphicsMedia::getProperties(QMap<QString, QString> &properties)
{
  StructuralGraphicsNode::getProperties(properties);

  properties["src"] = getSource();
  properties["refer"] = getRefer();
  properties["referUID"] = getReferUID();
  properties["instance"] = getInstance();
}
