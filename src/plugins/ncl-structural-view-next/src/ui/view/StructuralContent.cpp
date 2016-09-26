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

QString StructuralContent::getLocation() const
{
  return _location;
}

void StructuralContent::setLocation(const QString &location)
{
  this->_location = location;

  setStructuralProperty(STR_PROPERTY_CONTENT_LOCATION, location);
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
  setLocation(getStructuralProperty(STR_PROPERTY_CONTENT_LOCATION));
  setMimeType(StructuralUtil::translateStringToMimeType(getStructuralProperty(STR_PROPERTY_CONTENT_MIMETYPE)));

  StructuralMimeType mimetype = getMimeType();

  if (mimetype != Structural::NoMimeType)
    setToolTip(StructuralUtil::getMimeTypeTooltip(mimetype, getStructuralId()));
}

void StructuralContent::draw(QPainter* painter)
{
  painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

  painter->drawPixmap(STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
                      STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
                      getWidth() - 2*STR_DEFAULT_CONTENT_PADDING,
                      getHeight() - 2*STR_DEFAULT_CONTENT_PADDING - 4*STR_DEFAULT_CONTENT_PADDING,
                      QPixmap(StructuralUtil::getMimeTypeIcon(getMimeType())));

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
