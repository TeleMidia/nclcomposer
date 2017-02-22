#include "StructuralContent.h"

#include <QDrag>

#include "StructuralUtil.h"

StructuralContent::StructuralContent(StructuralEntity* parent)
  : StructuralNode(parent)
{
  setStructuralType(Structural::Media);

  setMimeType(Structural::NoMimeType);

  setWidth(STR_DEFAULT_CONTENT_W);
  setHeight(STR_DEFAULT_CONTENT_H);

  setResizable(false);
}

StructuralContent::~StructuralContent()
{

}

StructuralMimeType StructuralContent::getMimeType() const
{
  return _mimetype;
}

void StructuralContent::setMimeType(Structural::StructuralMimeType type)
{
  _mimetype = type;

  StructuralNode::setStructuralProperty(STR_PROPERTY_CONTENT_MIMETYPE, StructuralUtil::translateMimeTypeToString(type));
}

void StructuralContent::adjust(bool collision,  bool recursion)
{
  StructuralNode::adjust(collision, recursion);

  // Adjusting properties
  if (!getStructuralProperty(STR_PROPERTY_CONTENT_TYPE).isEmpty())
  {
    QString type = getStructuralProperty(STR_PROPERTY_CONTENT_TYPE);

    if (type.startsWith("image"))
    {
      setMimeType(Structural::Image);
    }
    else if (type.startsWith("audio"))
    {
      setMimeType(Structural::Audio);
    }
    else if (type.startsWith("video"))
    {
      setMimeType(Structural::Video);
    }
    else if (type.startsWith("text"))
    {
      if (type.endsWith("html"))
        setMimeType(Structural::HTML);
      else
        setMimeType(Structural::Text);
    }
    else if (type.endsWith("NCLua"))
    {
      setMimeType(Structural::NCLua);
    }
    else if (type.endsWith("NCL"))
    {
      setMimeType(Structural::NCL);
    }
    else if (type.endsWith("settings"))
    {
      setMimeType(Structural::Settings);
    }
    else if (type.endsWith("time"))
    {
      setMimeType(Structural::Time);
    }
    else
    {
      setMimeType(Structural::NoMimeType);
    }
  }
  else if (!getStructuralProperty(STR_PROPERTY_CONTENT_LOCATION).isEmpty())
  {
    QString location = getStructuralProperty(STR_PROPERTY_CONTENT_LOCATION);
    QString suffix = location.right(location.length() - location.lastIndexOf('.') - 1);

    setMimeType(StructuralUtil::getMimeTypeByExtension(suffix));
  }
  else
  {
    setMimeType(Structural::NoMimeType);
  }

  if (_mimetype != Structural::NoMimeType)
    setToolTip(StructuralUtil::getMimeTypeTooltip(_mimetype,
                                                getStructuralId(),
                                                getInfo(),
                                                getWarning(),
                                                getError()));
}

void StructuralContent::draw(QPainter* painter)
{
  painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

  if (!STR_DEFAULT_WITH_BODY &&
      !STR_DEFAULT_WITH_FLOATING_INTERFACES)
  {
    if (getStructuralProperty(STR_PROPERTY_ENTITY_AUTOSTART) == STR_VALUE_TRUE)
    {
      painter->fillRect(STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
                        STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
                        getWidth() - 2*STR_DEFAULT_CONTENT_PADDING,
                        getHeight() - 2*STR_DEFAULT_CONTENT_PADDING - 4*STR_DEFAULT_CONTENT_PADDING,
                        QBrush(QColor(76,76,76,95)));

      painter->setPen(QPen(QBrush(QColor(76,76,76)),2));

      painter->drawRect(STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
                        STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
                        getWidth() - 2*STR_DEFAULT_CONTENT_PADDING,
                        getHeight() - 2*STR_DEFAULT_CONTENT_PADDING - 4*STR_DEFAULT_CONTENT_PADDING);
    }
  }

  painter->drawPixmap(STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
                      STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
                      getWidth() - 2*STR_DEFAULT_CONTENT_PADDING,
                      getHeight() - 2*STR_DEFAULT_CONTENT_PADDING - 4*STR_DEFAULT_CONTENT_PADDING,
                      QPixmap(StructuralUtil::getMimeTypeIcon(getMimeType())));

  if (!getError().isEmpty() ||
      !getWarning().isEmpty())
  {
    QString icon;
    QString color;

    if (!getError().isEmpty())
    {
      icon = QString(STR_DEFAULT_ALERT_ERROR_ICON);
      color = QString(STR_DEFAULT_ALERT_ERROR_COLOR);
    }
    else
    {
      icon = QString(STR_DEFAULT_ALERT_WARNING_ICON);
      color = QString(STR_DEFAULT_ALERT_WARNING_COLOR);
    }

    painter->drawPixmap((getWidth() - 2*STR_DEFAULT_CONTENT_PADDING)/2 + STR_DEFAULT_ALERT_ICON_W,
                        (getHeight() - 2*STR_DEFAULT_CONTENT_PADDING)/2 + STR_DEFAULT_CONTENT_PADDING,
                        STR_DEFAULT_ALERT_ICON_W, STR_DEFAULT_ALERT_ICON_H, QPixmap(icon));

    int max = 20;
    int start = 8;
    int end = getWidth();

    double current = start;
    double step = (double) ( end - start ) / max;

    QPolygonF polygon;
    painter->setPen(QPen(QBrush(QColor(color)), 0));
    painter->setRenderHint(QPainter::Antialiasing, true);

    for (int i = 0; i < max; i++)
    {
      current = start + (double) i * step;

      if( i % 2)
        polygon << QPointF( current, getHeight() - 3 );
      else
        polygon << QPointF( current, getHeight() );
    }

    painter->drawPolyline(polygon);
  }

  painter->setBrush(Qt::NoBrush);
  painter->setPen(QPen(QBrush(Qt::black),0));

  QString text = getStructuralId();

  if (text.isEmpty())
    text = "(?)";

  if (text.length() > 5)
    text = text.replace(3, text.length()-3, "...");

  painter->drawText(STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
                    STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING + getHeight() - STR_DEFAULT_CONTENT_TEXT_H - 2*STR_DEFAULT_CONTENT_PADDING,
                    getWidth() - 2*STR_DEFAULT_CONTENT_PADDING,
                    STR_DEFAULT_CONTENT_TEXT_H,
                    Qt::AlignCenter, text);

  if (isMoving())
    painter->drawRect(getMoveLeft() + STR_DEFAULT_ENTITY_PADDING - getLeft(),
                      getMoveTop() + STR_DEFAULT_ENTITY_PADDING - getTop(),
                      getWidth(),
                      getHeight());
}
