#include "StructuralComposition.h"

#include <QFileInfo>
#include <QMimeData>
#include <QUrl>

#include "StructuralContent.h"
#include "StructuralUtil.h"
#include "util/Utilities.h"

StructuralComposition::StructuralComposition (StructuralEntity *parent)
    : StructuralNode (parent)
{
  setWidth (ST_DEFAULT_COMPOSITION_W);
  setHeight (ST_DEFAULT_COMPOSITION_H);

  setHoverable (false);
  setCollapsed (false);
}

void
StructuralComposition::collapse ()
{
  CPR_ASSERT (!isCollapsed ());

  setHoverable (true);
  setResizable (false);

  setUncollapsedWidth (width ());
  setUncollapsedHeight (height ());

  setTop (top () + uncollapsedHeight () / 2 - ST_DEFAULT_CONTENT_H / 2);
  setLeft (left () + uncollapsedWidth () / 2 - ST_DEFAULT_CONTENT_W / 2);
  setWidth (ST_DEFAULT_CONTENT_W);
  setHeight (ST_DEFAULT_CONTENT_H);

  for (StructuralEntity *ent : children ())
  {
    ent->setUncollapsedTop (ent->top ());
    ent->setUncollapsedLeft (ent->left ());

    if (ent->category () == Structural::Interface)
    {
      ent->setTop (
          ((ent->top () * ST_DEFAULT_CONTENT_H) / uncollapsedHeight ()));
      ent->setLeft (
          ((ent->left () * ST_DEFAULT_CONTENT_W) / uncollapsedWidth ()));
    }
    else
    {
      ent->setHidden (true);

      if (!ent->isCollapsed ())
      {
        ent->setUncollapsedWidth (ent->width ());
        ent->setUncollapsedHeight (ent->height ());
      }
    }
  }

  setCollapsed (true);
}

void
StructuralComposition::uncollapse ()
{
  CPR_ASSERT (isCollapsed ());

  QRect uncollapsed = uncollapsedRect ();

  setHoverable (false);
  setResizable (true);

  setTop (top () - (uncollapsed.height () / 2 - ST_DEFAULT_CONTENT_H / 2));
  setLeft (left () - (uncollapsed.width () / 2 - ST_DEFAULT_CONTENT_W / 2));
  setWidth (uncollapsed.width ());
  setHeight (uncollapsed.height ());

  for (StructuralEntity *ent : children ())
  {
    if (ent->category () == Structural::Interface)
    {
      ent->setTop (ent->top () * uncollapsed.height () / ST_DEFAULT_CONTENT_H);
      ent->setLeft (ent->left () * uncollapsed.width ()
                    / ST_DEFAULT_CONTENT_W);
    }
    else
    {
      ent->setTop (ent->uncollapsedTop ());
      ent->setLeft (ent->uncollapsedLeft ());

      ent->setHidden (false);

      if (!ent->isCollapsed ())
      {
        ent->setWidth (ent->uncollapsedWidth ());
        ent->setHeight (ent->uncollapsedHeight ());
      }
    }
  }

  setCollapsed (false);
}

void
StructuralComposition::draw (QPainter *painter)
{
  if (!isCollapsed ())
  {
    painter->setRenderHint (QPainter::Antialiasing, true);
    painter->setRenderHint (QPainter::SmoothPixmapTransform, false);

    QColor drawColor = QColor (StructuralUtil::color (structuralType ()));
    painter->setBrush (drawColor);

    if (!error ().isEmpty () || !warning ().isEmpty ())
    {
      QString color;

      if (!error ().isEmpty ())
        color = QString (ST_DEFAULT_ALERT_ERROR_COLOR);
      else
        color = QString (ST_DEFAULT_ALERT_WARNING_COLOR);

      painter->setPen (QPen (QBrush (QColor (color)), 2));
    }
    else
    {
      if (!ST_OPT_WITH_BODY && !ST_OPT_USE_FLOATING_INTERFACES)
      {
        if (property (ST_ATTR_ENT_AUTOSTART) == ST_VALUE_TRUE)
          painter->setPen (QPen (QBrush (QColor (76, 76, 76)), 2));
      }
      else
      {
        painter->setPen (QPen (drawColor.darker (), 0));
      }
    }

#if ST_OPT_COMPOSITIONS_AS_RECT
#define drawComposition(x, y, w, h) drawRoundedRect (x, y, w, h, 20, 20)
#else
#define drawComposition(x, y, w, h) drawEllipse (x, y, w, h)
#endif

    painter->drawComposition (ST_DEFAULT_ENTITY_PADDING,
                              ST_DEFAULT_ENTITY_PADDING, width (), height ());

    painter->setBrush (Qt::NoBrush);

    if (isMoving ())
      painter->drawComposition (
          moveLeft () + ST_DEFAULT_ENTITY_PADDING - left (),
          moveTop () + ST_DEFAULT_ENTITY_PADDING - top (), width (),
          height ());

    else if (isResizing ())
      painter->drawComposition (
          resizeLeft () + ST_DEFAULT_ENTITY_PADDING - left (),
          resizeTop () + ST_DEFAULT_ENTITY_PADDING - top (), resizeWidth (),
          resizeHeight ());
  }
  else
  {
    painter->setRenderHint (QPainter::Antialiasing, false);
    painter->setRenderHint (QPainter::SmoothPixmapTransform, true);

    if (!ST_OPT_WITH_BODY && !ST_OPT_USE_FLOATING_INTERFACES)
    {
      if (property (ST_ATTR_ENT_AUTOSTART) == ST_VALUE_TRUE)
      {
        painter->fillRect (
            ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
            ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
            width () - 2 * ST_DEFAULT_CONTENT_PADDING,
            height () - 2 * ST_DEFAULT_CONTENT_PADDING
                - 4 * ST_DEFAULT_CONTENT_PADDING,
            QBrush (QColor (76, 76, 76, 95)));

        painter->setPen (QPen (QBrush (QColor (76, 76, 76)), 2));

        painter->drawRect (
            ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
            ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
            width () - 2 * ST_DEFAULT_CONTENT_PADDING,
            height () - 2 * ST_DEFAULT_CONTENT_PADDING
                - 4 * ST_DEFAULT_CONTENT_PADDING);
      }
    }

    painter->drawPixmap (
        ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
        ST_DEFAULT_ENTITY_PADDING + ST_DEFAULT_CONTENT_PADDING,
        width () - 2 * ST_DEFAULT_CONTENT_PADDING,
        height () - 2 * ST_DEFAULT_CONTENT_PADDING
            - 4 * ST_DEFAULT_CONTENT_PADDING,
        QPixmap (StructuralUtil::icon (structuralType ())));

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
}

void
StructuralComposition::delineate (QPainterPath *painter) const
{
  int x = ST_DEFAULT_ENTITY_PADDING;
  int y = ST_DEFAULT_ENTITY_PADDING;
  int w = width ();
  int h = height ();

  if (isCollapsed ())
    painter->addRect (x, y, w, h);
  else
    painter->addEllipse (x, y, w, h);
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
StructuralComposition::dropEvent (QGraphicsSceneDragDropEvent *evt)
{
  QList<QUrl> urls = evt->mimeData ()->urls ();

  if (!urls.isEmpty ())
  {
    evt->setAccepted (false);

    for (const QUrl &url : urls)
    {
      QString filename = url.toLocalFile ();

      QMap<QString, QString> props = {
        { ST_ATTR_ENT_TYPE, StructuralUtil::typetostr (Structural::Media) },
        { ST_ATTR_ENT_ID,
          StructuralUtil::formatId (QFileInfo (filename).baseName ()) },
        { ST_ATTR_NODE_SRC, filename },
        { ST_ATTR_ENT_TOP,
          QString::number (evt->pos ().y () - ST_DEFAULT_CONTENT_H / 2) },
        { ST_ATTR_ENT_LEFT,
          QString::number (evt->pos ().x () - ST_DEFAULT_CONTENT_W / 2) }
      };

      insertAsked (StructuralUtil::createUid (), uid (), props,
                   StructuralUtil::createSettings ());
    }
  }
  else
  {
    StructuralNode::dropEvent (evt);
  }
}

void
StructuralComposition::mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event)
{
  Q_UNUSED (event);
  QStrMap previous = properties ();

  if (!isCollapsed ())
    collapse ();
  else
    uncollapse ();

  emit changeAsked (uid (), properties (), previous,
                    StructuralUtil::createSettings (true, false));
}
