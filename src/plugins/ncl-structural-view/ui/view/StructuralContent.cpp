#include "StructuralContent.h"

#include <QDrag>

#include "StructuralUtil.h"

StructuralContent::StructuralContent (StructuralEntity *parent)
    : StructuralNode (parent)
{
  setStructuralType (Structural::Media);

  setMimeType (Structural::NoMimeType);

  setWidth (ST_DEFAULT_CONTENT_W);
  setHeight (ST_DEFAULT_CONTENT_H);

  setResizable (false);
}

StructuralMimeType
StructuralContent::getMimeType () const
{
  return _mimetype;
}

void
StructuralContent::setMimeType (Structural::MimeType type)
{
  _mimetype = type;

  StructuralNode::setProperty (ST_ATTR_NODE_MIMETYPE,
                               StructuralUtil::mimeToStr (type));
}

void
StructuralContent::adjust (bool collision, bool recursion)
{
  StructuralNode::adjust (collision, recursion);

  // Adjusting properties
  if (!property (ST_ATTR_NODE_TYPE).isEmpty ())
  {
    QString type = property (ST_ATTR_NODE_TYPE);

    if (type.startsWith ("image"))
    {
      setMimeType (Structural::Image);
    }
    else if (type.startsWith ("audio"))
    {
      setMimeType (Structural::Audio);
    }
    else if (type.startsWith ("video"))
    {
      setMimeType (Structural::Video);
    }
    else if (type.startsWith ("text"))
    {
      if (type.endsWith ("html"))
        setMimeType (Structural::HTML);
      else
        setMimeType (Structural::Text);
    }
    else if (type.endsWith ("NCLua"))
    {
      setMimeType (Structural::NCLua);
    }
    else if (type.endsWith ("NCL"))
    {
      setMimeType (Structural::NCL);
    }
    else if (type.endsWith ("settings"))
    {
      setMimeType (Structural::Settings);
    }
    else if (type.endsWith ("time"))
    {
      setMimeType (Structural::Time);
    }
    else
    {
      setMimeType (Structural::NoMimeType);
    }
  }
  else if (!property (ST_ATTR_NODE_SRC).isEmpty ())
  {
    QString location = property (ST_ATTR_NODE_SRC);
    QString suffix
        = location.right (location.length () - location.lastIndexOf ('.') - 1);

    setMimeType (StructuralUtil::mimeByExtension (suffix));
  }
  else
  {
    setMimeType (Structural::NoMimeType);
  }

  if (_mimetype != Structural::NoMimeType)
    setToolTip (StructuralUtil::mimeTooltip (_mimetype, id (), info (),
                                             warning (), error ()));
}

void
StructuralContent::draw (QPainter *painter)
{
  painter->setRenderHint (QPainter::SmoothPixmapTransform, true);

  painter->drawPixmap (
      ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
      ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
      width () - 2 * ST_DEFAULT_CONTENT_PADDING,
      height () - 2 * ST_DEFAULT_CONTENT_PADDING
          - 4 * ST_DEFAULT_CONTENT_PADDING,
      QPixmap (StructuralUtil::mimeTypeIcon (getMimeType ())));

  if (!ST_OPT_WITH_BODY && !ST_OPT_USE_FLOATING_INTERFACES)
  {
    if (property (ST_ATTR_ENT_AUTOSTART) == ST_VALUE_TRUE)
    {
      painter->drawPixmap ((width () - 2 * ST_DEFAULT_CONTENT_PADDING) / 2
                               + ST_DEFAULT_ALERT_ICON_W,
                           (height () - 2 * ST_DEFAULT_CONTENT_PADDING) / 2
                               + ST_DEFAULT_CONTENT_PADDING,
                           ST_DEFAULT_ALERT_ICON_W, ST_DEFAULT_ALERT_ICON_H,
                           QPixmap (":/icon/autostart"));
    }
  }

  if (!error ().isEmpty () || !warning ().isEmpty ())
  {
    QString icon;
    QString color;

    if (!error ().isEmpty ())
    {
      icon = QString (ST_DEFAULT_ALERT_ERROR_ICON);
      color = QString (ST_DEFAULT_ALERT_ERROR_COLOR);
    }
    else
    {
      icon = QString (ST_DEFAULT_ALERT_WARNING_ICON);
      color = QString (ST_DEFAULT_ALERT_WARNING_COLOR);
    }

    painter->drawPixmap ((width () - 2 * ST_DEFAULT_CONTENT_PADDING) / 2
                             + ST_DEFAULT_ALERT_ICON_W,
                         (height () - 2 * ST_DEFAULT_CONTENT_PADDING) / 2
                             + ST_DEFAULT_CONTENT_PADDING,
                         ST_DEFAULT_ALERT_ICON_W, ST_DEFAULT_ALERT_ICON_H,
                         QPixmap (icon));

    int max = 20;
    int start = 8;
    int end = width ();

    double current = start;
    double step = (double)(end - start) / max;

    QPolygonF polygon;
    painter->setPen (QPen (QBrush (QColor (color)), 0));
    painter->setRenderHint (QPainter::Antialiasing, true);

    for (int i = 0; i < max; i++)
    {
      current = start + (double)i * step;

      if (i % 2)
        polygon << QPointF (current, height () - 3);
      else
        polygon << QPointF (current, height ());
    }

    painter->drawPolyline (polygon);
  }

  painter->setBrush (Qt::NoBrush);
  painter->setPen (QPen (QBrush (Qt::black), 0));

  QString text = id ();

  if (text.isEmpty ())
    text = "(?)";

  if (text.length () > 5)
    text = text.replace (3, text.length () - 3, "...");

  painter->drawText (ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
                     ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING
                         + height () - ST_DEFAULT_CONTENT_TEXT_H
                         - 2 * ST_DEFAULT_CONTENT_PADDING,
                     width () - 2 * ST_DEFAULT_CONTENT_PADDING,
                     ST_DEFAULT_CONTENT_TEXT_H, Qt::AlignCenter, text);

  if (isMoving ())
    painter->drawRect (moveLeft () + ST_DEFAULT_ENTITY_PADDING - left (),
                       moveTop () + ST_DEFAULT_ENTITY_PADDING - top (),
                       width (), height ());
}
