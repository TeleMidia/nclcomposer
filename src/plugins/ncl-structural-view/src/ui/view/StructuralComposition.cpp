#include "StructuralComposition.h"

#include <QFileInfo>
#include <QMimeData>
#include <QUrl>

#include "StructuralContent.h"
#include "StructuralUtil.h"

StructuralComposition::StructuralComposition (StructuralEntity *parent)
    : StructuralNode (parent)
{
  setWidth (STR_DEFAULT_COMPOSITION_W);
  setHeight (STR_DEFAULT_COMPOSITION_H);

  setHoverable (false);
  setUncollapsed (true);
}

StructuralComposition::~StructuralComposition () {}

void
StructuralComposition::collapse (bool notify)
{
  QMap<QString, QString> previous = getProperties ();

  if (!isUncollapsed ())
  {
    setHoverable (false);
    setResizable (true);

    setTop (getTop ()
            - (getUncollapedHeight () / 2 - STR_DEFAULT_CONTENT_H / 2));
    setLeft (getLeft ()
             - (getUncollapedWidth () / 2 - STR_DEFAULT_CONTENT_W / 2));
    setWidth (getUncollapedWidth ());
    setHeight (getUncollapedHeight ());

    foreach (StructuralEntity *entity, getChildren ())
    {
      if (entity->getCategory () == Structural::Interface)
      {
        entity->setTop (entity->getTop () * getUncollapedHeight ()
                        / STR_DEFAULT_CONTENT_H);
        entity->setLeft (entity->getLeft () * getUncollapedWidth ()
                         / STR_DEFAULT_CONTENT_W);
      }
      else
      {
        entity->setTop (entity->getUncollapedTop ());
        entity->setLeft (entity->getUncollapedLeft ());

        entity->setHidden (false);

        if (entity->isUncollapsed ())
        {
          entity->setWidth (entity->getUncollapedWidth ());
          entity->setHeight (entity->getUncollapedHeight ());
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

    setTop (getTop () + getUncollapedHeight () / 2
            - STR_DEFAULT_CONTENT_H / 2);
    setLeft (getLeft () + getUncollapedWidth () / 2
             - STR_DEFAULT_CONTENT_W / 2);
    setWidth (STR_DEFAULT_CONTENT_W);
    setHeight (STR_DEFAULT_CONTENT_H);

    foreach (StructuralEntity *entity, getChildren ())
    {
      entity->setUncollapedTop (entity->getTop ());
      entity->setUncollapedLeft (entity->getLeft ());

      if (entity->getCategory () == Structural::Interface)
      {
        entity->setTop (((entity->getTop () * STR_DEFAULT_CONTENT_H)
                         / getUncollapedHeight ()));
        entity->setLeft (((entity->getLeft () * STR_DEFAULT_CONTENT_W)
                          / getUncollapedWidth ()));
      }
      else
      {
        entity->setHidden (true);

        if (entity->isUncollapsed ())
        {
          entity->setUncollapedWidth (entity->getWidth ());
          entity->setUncollapedHeight (entity->getHeight ());
        }
      }
    }
  }

  setUncollapsed (!isUncollapsed ());

  if (notify)
    emit changed (getUid (), getProperties (), previous,
                  StructuralUtil::createSettings (true, false));
}

void
StructuralComposition::draw (QPainter *painter)
{
  if (isUncollapsed ())
  {
    painter->setRenderHint (QPainter::Antialiasing, true);
    painter->setRenderHint (QPainter::SmoothPixmapTransform, false);

    QColor drawColor
        = QColor (StructuralUtil::getColor (getType ()));
    painter->setBrush (drawColor);

    if (!getError ().isEmpty () || !getWarning ().isEmpty ())
    {
      QString color;

      if (!getError ().isEmpty ())
      {
        color = QString (STR_DEFAULT_ALERT_ERROR_COLOR);
      }
      else
      {
        color = QString (STR_DEFAULT_ALERT_WARNING_COLOR);
      }

      painter->setPen (QPen (QBrush (QColor (color)), 2));
    }
    else
    {
      if (!STR_DEFAULT_WITH_BODY && !STR_DEFAULT_WITH_FLOATING_INTERFACES)
      {
        if (getProperty (STR_PROPERTY_ENTITY_AUTOSTART)
            == STR_VALUE_TRUE)
        {
          painter->setPen (QPen (QBrush (QColor (76, 76, 76)), 2));
        }
      }
      else
      {
        painter->setPen (QPen (drawColor.darker (), 0));
      }
    }

    painter->drawEllipse (STR_DEFAULT_ENTITY_PADDING,
                          STR_DEFAULT_ENTITY_PADDING, getWidth (),
                          getHeight ());

    painter->setBrush (Qt::NoBrush);

    if (isMoving ())
      painter->drawEllipse (
          getMoveLeft () + STR_DEFAULT_ENTITY_PADDING - getLeft (),
          getMoveTop () + STR_DEFAULT_ENTITY_PADDING - getTop (), getWidth (),
          getHeight ());

    else if (isResizing ())
      painter->drawEllipse (
          getResizeLeft () + STR_DEFAULT_ENTITY_PADDING - getLeft (),
          getResizeTop () + STR_DEFAULT_ENTITY_PADDING - getTop (),
          getResizeWidth (), getResizeHeight ());
  }
  else
  {
    painter->setRenderHint (QPainter::Antialiasing, false);
    painter->setRenderHint (QPainter::SmoothPixmapTransform, true);

    if (!STR_DEFAULT_WITH_BODY && !STR_DEFAULT_WITH_FLOATING_INTERFACES)
    {
      if (getProperty (STR_PROPERTY_ENTITY_AUTOSTART)
          == STR_VALUE_TRUE)
      {
        painter->fillRect (
            STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
            STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
            getWidth () - 2 * STR_DEFAULT_CONTENT_PADDING,
            getHeight () - 2 * STR_DEFAULT_CONTENT_PADDING
                - 4 * STR_DEFAULT_CONTENT_PADDING,
            QBrush (QColor (76, 76, 76, 95)));

        painter->setPen (QPen (QBrush (QColor (76, 76, 76)), 2));

        painter->drawRect (
            STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
            STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
            getWidth () - 2 * STR_DEFAULT_CONTENT_PADDING,
            getHeight () - 2 * STR_DEFAULT_CONTENT_PADDING
                - 4 * STR_DEFAULT_CONTENT_PADDING);
      }
    }

    painter->drawPixmap (
        STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
        STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
        getWidth () - 2 * STR_DEFAULT_CONTENT_PADDING,
        getHeight () - 2 * STR_DEFAULT_CONTENT_PADDING
            - 4 * STR_DEFAULT_CONTENT_PADDING,
        QPixmap (StructuralUtil::getIcon (getType ())));

    if (!getError ().isEmpty () || !getWarning ().isEmpty ())
    {
      QString icon;
      QString color;

      if (!getError ().isEmpty ())
      {
        icon = QString (STR_DEFAULT_ALERT_ERROR_ICON);
        color = QString (STR_DEFAULT_ALERT_ERROR_COLOR);
      }
      else
      {
        icon = QString (STR_DEFAULT_ALERT_WARNING_ICON);
        color = QString (STR_DEFAULT_ALERT_WARNING_COLOR);
      }

      painter->drawPixmap ((getWidth () - 2 * STR_DEFAULT_CONTENT_PADDING) / 2
                               + STR_DEFAULT_ALERT_ICON_W,
                           (getHeight () - 2 * STR_DEFAULT_CONTENT_PADDING) / 2
                               + STR_DEFAULT_CONTENT_PADDING,
                           STR_DEFAULT_ALERT_ICON_W, STR_DEFAULT_ALERT_ICON_H,
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

    painter->drawText (
        STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
        STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING + getHeight ()
            - STR_DEFAULT_CONTENT_TEXT_H - 2 * STR_DEFAULT_CONTENT_PADDING,
        getWidth () - 2 * STR_DEFAULT_CONTENT_PADDING,
        STR_DEFAULT_CONTENT_TEXT_H, Qt::AlignCenter, text);

    if (isMoving ())
      painter->drawRect (
          getMoveLeft () + STR_DEFAULT_ENTITY_PADDING - getLeft (),
          getMoveTop () + STR_DEFAULT_ENTITY_PADDING - getTop (), getWidth (),
          getHeight ());
  }
}

void
StructuralComposition::delineate (QPainterPath *painter) const
{
  int x = STR_DEFAULT_ENTITY_PADDING;
  int y = STR_DEFAULT_ENTITY_PADDING;
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

    foreach (QUrl url, list)
    {
      QString filename = url.toLocalFile ();

      QMap<QString, QString> properties;
      properties[STR_PROPERTY_ENTITY_TYPE]
          = StructuralUtil::translateTypeToString (Structural::Media);
      properties[STR_PROPERTY_ENTITY_ID]
          = StructuralUtil::formatId (QFileInfo (filename).baseName ());
      properties[STR_PROPERTY_CONTENT_LOCATION] = filename;

      properties[STR_PROPERTY_ENTITY_TOP]
          = QString::number (event->pos ().y () - STR_DEFAULT_CONTENT_H / 2);
      properties[STR_PROPERTY_ENTITY_LEFT]
          = QString::number (event->pos ().x () - STR_DEFAULT_CONTENT_W / 2);

      inserted (StructuralUtil::createUid (), getUid (), properties,
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
