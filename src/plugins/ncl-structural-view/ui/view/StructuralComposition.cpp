#include "StructuralComposition.h"

#include <QFileInfo>
#include <QMimeData>
#include <QUrl>

#include "StructuralContent.h"
#include "StructuralUtil.h"

StructuralComposition::StructuralComposition (StructuralEntity *parent)
    : StructuralNode (parent)
{
  setWidth (ST_DEFAULT_COMPOSITION_W);
  setHeight (ST_DEFAULT_COMPOSITION_H);

  setHoverable (false);
  setUncollapsed (true);
}

void
StructuralComposition::collapse (bool notify)
{
  QMap<QString, QString> previous = getProperties ();

  if (!isUncollapsed ())
  {
    setHoverable (false);
    setResizable (true);

    setTop (getTop ()
            - (getUncollapedHeight () / 2 - ST_DEFAULT_CONTENT_H / 2));
    setLeft (getLeft ()
             - (getUncollapedWidth () / 2 - ST_DEFAULT_CONTENT_W / 2));
    setWidth (getUncollapedWidth ());
    setHeight (getUncollapedHeight ());

    for (StructuralEntity *ent : getChildren ())
    {
      if (ent->getCategory () == Structural::Interface)
      {
        ent->setTop (ent->getTop () * getUncollapedHeight ()
                     / ST_DEFAULT_CONTENT_H);
        ent->setLeft (ent->getLeft () * getUncollapedWidth ()
                      / ST_DEFAULT_CONTENT_W);
      }
      else
      {
        ent->setTop (ent->getUncollapedTop ());
        ent->setLeft (ent->getUncollapedLeft ());

        ent->setHidden (false);

        if (ent->isUncollapsed ())
        {
          ent->setWidth (ent->getUncollapedWidth ());
          ent->setHeight (ent->getUncollapedHeight ());
        }
      }
    }
  }
  else
  {
    setHoverable (true);
    setResizable (false);

    setUncollapedWidth (getWidth ());
    setUncollapedHeight (getHeight ());

    setTop (getTop () + getUncollapedHeight () / 2 - ST_DEFAULT_CONTENT_H / 2);
    setLeft (getLeft () + getUncollapedWidth () / 2
             - ST_DEFAULT_CONTENT_W / 2);
    setWidth (ST_DEFAULT_CONTENT_W);
    setHeight (ST_DEFAULT_CONTENT_H);

    for (StructuralEntity *ent : getChildren ())
    {
      ent->setUncollapedTop (ent->getTop ());
      ent->setUncollapedLeft (ent->getLeft ());

      if (ent->getCategory () == Structural::Interface)
      {
        ent->setTop (((ent->getTop () * ST_DEFAULT_CONTENT_H)
                      / getUncollapedHeight ()));
        ent->setLeft (((ent->getLeft () * ST_DEFAULT_CONTENT_W)
                       / getUncollapedWidth ()));
      }
      else
      {
        ent->setHidden (true);

        if (ent->isUncollapsed ())
        {
          ent->setUncollapedWidth (ent->getWidth ());
          ent->setUncollapedHeight (ent->getHeight ());
        }
      }
    }
  }

  setUncollapsed (!isUncollapsed ());

  if (notify)
  {
    emit changeAsked (getUid (), getProperties (), previous,
                      StructuralUtil::createSettings (true, false));
  }
}

void
StructuralComposition::draw (QPainter *painter)
{
  if (isUncollapsed ())
  {
    painter->setRenderHint (QPainter::Antialiasing, true);
    painter->setRenderHint (QPainter::SmoothPixmapTransform, false);

    QColor drawColor = QColor (StructuralUtil::getColor (getType ()));
    painter->setBrush (drawColor);

    if (!getError ().isEmpty () || !getWarning ().isEmpty ())
    {
      QString color;

      if (!getError ().isEmpty ())
      {
        color = QString (ST_DEFAULT_ALERT_ERROR_COLOR);
      }
      else
      {
        color = QString (ST_DEFAULT_ALERT_WARNING_COLOR);
      }

      painter->setPen (QPen (QBrush (QColor (color)), 2));
    }
    else
    {
      if (!ST_DEFAULT_WITH_BODY && !ST_DEFAULT_WITH_FLOATING_INTERFACES)
      {
        if (getProperty (ST_ATTR_ENT_AUTOSTART) == ST_VALUE_TRUE)
        {
          painter->setPen (QPen (QBrush (QColor (76, 76, 76)), 2));
        }
      }
      else
      {
        painter->setPen (QPen (drawColor.darker (), 0));
      }
    }

#if ST_DEFAULT_RECT_COMPOSITIONS
# define drawComposition(x,y,w,h) drawRoundedRect (x,y,w,h,20,20)
#else
# define drawComposition(x,y,w,h) drawEllipse (x,y,w,h)
#endif

    painter->drawComposition (ST_DEFAULT_ENTITY_PADDING,
                              ST_DEFAULT_ENTITY_PADDING,
                              getWidth (), getHeight ());

    painter->setBrush (Qt::NoBrush);

    if (isMoving ())
      painter->drawComposition (
          getMoveLeft () + ST_DEFAULT_ENTITY_PADDING - getLeft (),
          getMoveTop () + ST_DEFAULT_ENTITY_PADDING - getTop (), getWidth (),
          getHeight ());

    else if (isResizing ())
      painter->drawComposition (
          getResizeLeft () + ST_DEFAULT_ENTITY_PADDING - getLeft (),
          getResizeTop () + ST_DEFAULT_ENTITY_PADDING - getTop (),
          getResizeWidth (), getResizeHeight ());
  }
  else
  {
    painter->setRenderHint (QPainter::Antialiasing, false);
    painter->setRenderHint (QPainter::SmoothPixmapTransform, true);

    if (!ST_DEFAULT_WITH_BODY && !ST_DEFAULT_WITH_FLOATING_INTERFACES)
    {
      if (getProperty (ST_ATTR_ENT_AUTOSTART) == ST_VALUE_TRUE)
      {
        painter->fillRect (
            ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
            ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
            getWidth () - 2 * ST_DEFAULT_CONTENT_PADDING,
            getHeight () - 2 * ST_DEFAULT_CONTENT_PADDING
                - 4 * ST_DEFAULT_CONTENT_PADDING,
            QBrush (QColor (76, 76, 76, 95)));

        painter->setPen (QPen (QBrush (QColor (76, 76, 76)), 2));

        painter->drawRect (
            ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
            ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
            getWidth () - 2 * ST_DEFAULT_CONTENT_PADDING,
            getHeight () - 2 * ST_DEFAULT_CONTENT_PADDING
                - 4 * ST_DEFAULT_CONTENT_PADDING);
      }
    }

    painter->drawPixmap (
        ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
        ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
        getWidth () - 2 * ST_DEFAULT_CONTENT_PADDING,
        getHeight () - 2 * ST_DEFAULT_CONTENT_PADDING
            - 4 * ST_DEFAULT_CONTENT_PADDING,
        QPixmap (StructuralUtil::getIcon (getType ())));

    if (!getError ().isEmpty () || !getWarning ().isEmpty ())
    {
      QString icon;
      QString color;

      if (!getError ().isEmpty ())
      {
        icon = QString (ST_DEFAULT_ALERT_ERROR_ICON);
        color = QString (ST_DEFAULT_ALERT_ERROR_COLOR);
      }
      else
      {
        icon = QString (ST_DEFAULT_ALERT_WARNING_ICON);
        color = QString (ST_DEFAULT_ALERT_WARNING_COLOR);
      }

      painter->drawPixmap ((getWidth () - 2 * ST_DEFAULT_CONTENT_PADDING) / 2
                               + ST_DEFAULT_ALERT_ICON_W,
                           (getHeight () - 2 * ST_DEFAULT_CONTENT_PADDING) / 2
                               + ST_DEFAULT_CONTENT_PADDING,
                           ST_DEFAULT_ALERT_ICON_W, ST_DEFAULT_ALERT_ICON_H,
                           QPixmap (icon));

      int max = 20;

      int start = 8;
      int end = getWidth ();

      double current = start;
      double step = (double)(end - start) / max;

      QPolygonF polygon;
      painter->setPen (QPen (QBrush (QColor (color)), 0));
      painter->setRenderHint (QPainter::Antialiasing, true);

      for (int i = 0; i < max; i++)
      {
        current = start + (double)i * step;

        if (i % 2)
          polygon << QPointF (current, getHeight () - 3);
        else
          polygon << QPointF (current, getHeight ());
      }

      painter->drawPolyline (polygon);
    }

    painter->setBrush (Qt::NoBrush);
    painter->setPen (QPen (QBrush (Qt::black), 0));

    QString text = getId ();

    if (text.isEmpty ())
      text = "(?)";

    if (text.length () > 5)
      text = text.replace (3, text.length () - 3, "...");

    painter->drawText (ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
                       ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING
                           + getHeight () - ST_DEFAULT_CONTENT_TEXT_H
                           - 2 * ST_DEFAULT_CONTENT_PADDING,
                       getWidth () - 2 * ST_DEFAULT_CONTENT_PADDING,
                       ST_DEFAULT_CONTENT_TEXT_H, Qt::AlignCenter, text);

    if (isMoving ())
      painter->drawRect (getMoveLeft () + ST_DEFAULT_ENTITY_PADDING
                             - getLeft (),
                         getMoveTop () + ST_DEFAULT_ENTITY_PADDING - getTop (),
                         getWidth (), getHeight ());
  }
}

void
StructuralComposition::delineate (QPainterPath *painter) const
{
  int x = ST_DEFAULT_ENTITY_PADDING;
  int y = ST_DEFAULT_ENTITY_PADDING;
  int w = getWidth ();
  int h = getHeight ();

  if (isUncollapsed ())
    painter->addEllipse (x, y, w, h);
  else
    painter->addRect (x, y, w, h);
}

void
StructuralComposition::dragEnterEvent (QGraphicsSceneDragDropEvent *event)
{
  QList<QUrl> list = event->mimeData ()->urls ();

  if (!list.isEmpty ())
    event->setAccepted (true);
  else
    StructuralNode::dragEnterEvent (event);
}

void
StructuralComposition::dragMoveEvent (QGraphicsSceneDragDropEvent *event)
{
  QList<QUrl> list = event->mimeData ()->urls ();

  if (!list.isEmpty ())
    event->setAccepted (true);
  else
    StructuralNode::dragMoveEvent (event);
}

void
StructuralComposition::dropEvent (QGraphicsSceneDragDropEvent *event)
{
  QList<QUrl> list = event->mimeData ()->urls ();

  if (!list.isEmpty ())
  {
    event->setAccepted (false);

    for (const QUrl &url : list)
    {
      QString filename = url.toLocalFile ();

      QMap<QString, QString> props;
      props[ST_ATTR_ENT_TYPE] = StructuralUtil::typeToStr (Structural::Media);
      props[ST_ATTR_ENT_ID]
          = StructuralUtil::formatId (QFileInfo (filename).baseName ());
      props[ST_ATTR_NODE_SRC] = filename;

      props[ST_ATTR_ENT_TOP]
          = QString::number (event->pos ().y () - ST_DEFAULT_CONTENT_H / 2);
      props[ST_ATTR_ENT_LEFT]
          = QString::number (event->pos ().x () - ST_DEFAULT_CONTENT_W / 2);

      insertAsked (StructuralUtil::createUid (), getUid (), props,
                   StructuralUtil::createSettings ());
    }
  }
  else
  {
    StructuralNode::dropEvent (event);
  }
}

void
StructuralComposition::mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED (event);
  collapse (true);
}
