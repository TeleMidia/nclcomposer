#include "StructuralContent.h"

#include <QDrag>

#include "StructuralUtil.h"

StructuralContent::StructuralContent(StructuralEntity* parent)
  : StructuralNode(parent), enableDrag(false)
{
  setStructuralType(Structural::Media);

  setMediaType(Structural::NoContent);

  setResizable(false);

  createObjects();
  createConnections();

  setStructuralId("");

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

StructuralContent::~StructuralContent()
{

}

QString StructuralContent::getIcon() const
{
  return icon;
}

QString StructuralContent::getSource() const
{
  return source;
}

void StructuralContent::setSource(const QString &source)
{
  this->source = source;

  setMediaType(StructuralUtil::getnstTypeFromExtension(QFileInfo(source).suffix().toLower()));

  StructuralNode::setStructuralProperty(PLG_ENTITY_SRC, source);
}

void StructuralContent::setStructuralType(StructuralType type)
{
  StructuralNode::setStructuralType(type);
}

void StructuralContent::setStructuralProperty(const QString &name, const QString &value)
{
  if (name == PLG_ENTITY_SRC)
    setSource(value);
  else if (name == PLG_ENTITY_MEDIA)
    setMediaType((Structural::StructuralMedia) value.toInt());
  else
    StructuralNode::setStructuralProperty(name, value);
}

void StructuralContent::refresh()
{
  QString tip = "";
  QString name = (getStructuralId() != "" ? getStructuralId() : "?");


  switch(mediatype)
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

    case Structural::HTML:
      tip += "HTML ("+name+")";
      break;

    case Structural::NCL:
      tip += "NCL ("+name+")";
      break;

    case Structural::Settings:
      tip += "Settings ("+name+")";
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

QString StructuralContent::getRefer() const
{
  return refer;
}

void StructuralContent::setRefer(QString refer)
{
  this->refer = refer;
}

QString StructuralContent::getReferUID() const
{
  return referUID;
}

void StructuralContent::setReferUID(QString refetUID)
{
  this->referUID = refetUID;
}

QString StructuralContent::getInstance() const
{
  return instance;
}

void StructuralContent::setInstance(QString instance)
{
  this->instance = instance;
}

void StructuralContent::createObjects()
{
//  menu->_cutAction->setEnabled(true);
//  menu->_copyAction->setEnabled(true);

//  menu->_deleteAction->setEnabled(true);

//  menu->actionExport->setEnabled(true);

//  menu->_insertMenu->setEnabled(true);
//  menu->_areaAction->setEnabled(true);
//  menu->_propertyAction->setEnabled(true);
}

void StructuralContent::createConnections()
{
  // connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
  // connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));


//  connect(menu, SIGNAL(menuAddEntityTriggered(QnstSubtype)),
//          SLOT(newChild(QnstSubtype)));
}

void StructuralContent::draw(QPainter* painter)
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

  QString localid = (getStructuralId() != "" ? getStructuralId() : "?");

  if (localid.length() > 5)
  {
    localid = getStructuralId().replace(3,getStructuralId().length()-3,"...");
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

void StructuralContent::delineate(QPainterPath* painter) const
{
  painter->addRect(4, 4, getWidth(), getHeight());
}

void StructuralContent::setMediaType(Structural::StructuralMedia type)
{
  mediatype = type;

  this->icon = StructuralUtil::iconFromMediaType(mediatype);

  StructuralNode::setStructuralProperty(PLG_ENTITY_MEDIA, QString::number(type));
}


void StructuralContent::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    StructuralNode::mousePressEvent(event);
}
