#include "StructuralEntity.h"

#include "util/Utilities.h"
#include <QMimeData>

StructuralEntity::StructuralEntity (StructuralEntity *parent)
    : QObject (parent), QGraphicsItem (parent)
{
  setCategory (Structural::NoCategory);
  setStructuralType (Structural::NoType);

  setUid (StructuralUtil::createUid ());
  setId ("");
  setStructuralParent (parent);

  setMoveable (true);
  setResizable (true);
  setSelectable (true);
  setHoverable (true);
  setDraggable (false);

  setMoving (false);
  setHidden (false);
  setResizing (false);
  setSelected (false);
  setDragging (false);
  setHovering (false);

  setTop (0);
  setLeft (0);
  setWidth (0);
  setHeight (0);

  setMoveTop (0);
  setMoveLeft (0);

  setPressTop (0);
  setPressLeft (0);
  setPressWidth (0);
  setPressHeight (0);

  setResizeTop (0);
  setResizeLeft (0);
  setResizeWidth (0);
  setResizeHeight (0);

  setUncollapsedTop (0);
  setUncollapsedLeft (0);
  setUncollapsedWidth (0);
  setUncollapsedHeight (0);

  setzIndex (0);

  setFlag (QGraphicsItem::ItemIsMovable, true);
  setFlag (QGraphicsItem::ItemIsFocusable, true);

  setAcceptHoverEvents (true);
  setAcceptDrops (true);

  setSelected (false);
  setCollapsed (false);
}

QString
StructuralEntity::uid () const
{
  return _uid;
}

void
StructuralEntity::setUid (const QString &uid)
{
  _uid = uid;
  _props[ST_ATTR_ENT_UID] = _uid;
}

QString
StructuralEntity::id () const
{
  return _id;
}

void
StructuralEntity::setId (const QString &id)
{
  _id = id;
  _props.insert (ST_ATTR_ENT_ID, _id);
}

StructuralCategory
StructuralEntity::category () const
{
  return _category;
}

void
StructuralEntity::setCategory (StructuralCategory category)
{
  _category = category;
  _props[ST_ATTR_ENT_CATEGORY] = StructuralUtil::categorytostr (_category);
}

StructuralType
StructuralEntity::structuralType () const
{
  return _type;
}

void
StructuralEntity::setStructuralType (StructuralType type)
{
  _type = type;
  _props[ST_ATTR_ENT_TYPE] = StructuralUtil::typetostr (_type);

  _restrictions.clear ();

  //  addStructuralRestriction(ST_ATTR_ENT_AUTOSTART);

  switch (type)
  {
    case Structural::Media:
    case Structural::Context:
    case Structural::Switch:
    {
      if (type == Structural::Media)
      {
        addRestriction (ST_ATTR_NODE_TYPE);
        addRestriction (ST_ATTR_NODE_SRC);
        addRestriction (ST_ATTR_NODE_INSTANCE);
      }
      break;
    }

    case Structural::Port:
    case Structural::Mapping:
    {
      addRestriction (ST_ATTR_REF_COMPONENT_ID);
      addRestriction (ST_ATTR_REF_COMPONENT_UID);
      addRestriction (ST_ATTR_REF_INTERFACE_ID);
      addRestriction (ST_ATTR_REF_INTERFACE_UID);
      break;
    }

    case Structural::Bind:
    {
      addRestriction (ST_ATTR_REF_COMPONENT_ID);
      addRestriction (ST_ATTR_REF_COMPONENT_UID);
      addRestriction (ST_ATTR_REF_INTERFACE_ID);
      addRestriction (ST_ATTR_REF_INTERFACE_UID);

      addRestriction (ST_ATTR_LINKPARAM_NAME);
      addRestriction (ST_ATTR_LINKPARAM_VALUE);
      addRestriction (ST_ATTR_BINDPARAM_NAME);
      addRestriction (ST_ATTR_BINDPARAM_VALUE);
      break;
    }

    case Structural::Link:
    {
      addRestriction (ST_ATTR_LINKPARAM_NAME);
      addRestriction (ST_ATTR_LINKPARAM_VALUE);
      break;
    }

    case Structural::Body:
    {
      setzIndex (1);

      break;
    }

    default:
    {
      break;
    }
  }
}

StructuralResize
StructuralEntity::structuralResize () const
{
  return _resize;
}

void
StructuralEntity::setStructuralResize (StructuralResize resize)
{
  _resize = resize;
}

QMap<QString, QString>
StructuralEntity::properties () const
{
  return _props;
}

void
StructuralEntity::setProperties (const QMap<QString, QString> &props)
{
  if (!_restrictions.isEmpty ())
  {
    for (const QString &key : _props.keys ())
    {
      for (const QString &restriction : _restrictions)
      {
        if (key.contains (restriction) && !props.contains (key))
          _props.remove (key);
      }
    }
  }

  for (const QString &name : props.keys ())
    _props[name] = props.value (name);

  adjust (true);
}

QString
StructuralEntity::property (const QString &name) const
{
  return _props.value (name, "");
}

bool
StructuralEntity::hasProperty (const QString &name) const
{
  return _props.contains (name);
}

void
StructuralEntity::setProperty (const QString &name, const QString &value)
{
  _props[name] = value;
}

void
StructuralEntity::adjust (bool collision, bool recursion)
{
  Q_UNUSED (collision);
  Q_UNUSED (recursion);

  setId (_props[ST_ATTR_ENT_ID]);
  setUid (_props[ST_ATTR_ENT_UID]);

  setCategory (StructuralUtil::strtocategory (_props[ST_ATTR_ENT_CATEGORY]));
  setStructuralType (StructuralUtil::strtotype (_props[ST_ATTR_ENT_TYPE]));

  setTop (_props[ST_ATTR_ENT_TOP].toDouble ());
  setLeft (_props[ST_ATTR_ENT_LEFT].toDouble ());
  setWidth (_props[ST_ATTR_ENT_WIDTH].toDouble ());
  setHeight (_props[ST_ATTR_ENT_HEIGHT].toDouble ());

  setUncollapsedTop (_props[ST_ATTR_ENT_UNCOLLAPSED_TOP].toDouble ());
  setUncollapsedLeft (_props[ST_ATTR_ENT_UNCOLLAPSED_LEFT].toDouble ());
  setUncollapsedWidth (_props[ST_ATTR_ENT_UNCOLLAPSED_WIDTH].toDouble ());
  setUncollapsedHeight (_props[ST_ATTR_ENT_UNCOLLAPSED_HEIGHT].toDouble ());

  setzIndex (_props[ST_ATTR_ENT_ZINDEX].toInt ());

  setHidden ((_props[ST_ATTR_ENT_HIDDEN] == ST_VALUE_TRUE ? true : false));
  setCollapsed (
      (_props[ST_ATTR_ENT_COLLAPSED] == ST_VALUE_TRUE ? true : false));

  setToolTip (StructuralUtil::tooltip (_type, _id, _info, _warnning, _error,
                                       _props[ST_ATTR_REF_XCONNECTOR_ID]));

  if (scene () != NULL)
    scene ()->update ();
}

bool
StructuralEntity::isMoveable () const
{
  return _moveable;
}

void
StructuralEntity::setMoveable (bool moveable)
{
  _moveable = moveable;
}

bool
StructuralEntity::isSelectable () const
{
  return _selectable;
}

void
StructuralEntity::setSelectable (bool selectable)
{
  _selectable = selectable;
}

bool
StructuralEntity::isHoverable () const
{
  return _hoverable;
}

void
StructuralEntity::setHoverable (bool hoverable)
{
  _hoverable = hoverable;
}

bool
StructuralEntity::isResizable () const
{
  return _resizable;
}

void
StructuralEntity::setResizable (bool resizable)
{
  _resizable = resizable;
}

bool
StructuralEntity::isDraggable ()
{
  return _draggable;
}

void
StructuralEntity::setDraggable (bool draggable)
{
  _draggable = draggable;
}

bool
StructuralEntity::isMoving () const
{
  return _moving;
}

void
StructuralEntity::setMoving (bool moving)
{
  _moving = moving;
}

bool
StructuralEntity::isHidden () const
{
  return _hidden;
}

void
StructuralEntity::setHidden (bool hidden)
{
  _hidden = hidden;
  _props[ST_ATTR_ENT_HIDDEN] = (hidden ? ST_VALUE_TRUE : ST_VALUE_FALSE);

  setVisible (!hidden);
}

bool
StructuralEntity::isResizing () const
{
  return _resizing;
}

void
StructuralEntity::setResizing (bool resizing)
{
  _resizing = resizing;
}

bool
StructuralEntity::isHovering () const
{
  return _hovering;
}

void
StructuralEntity::setHovering (bool hovering)
{
  _hovering = hovering;
}

bool
StructuralEntity::isDragging () const
{
  return _dragging;
}

void
StructuralEntity::setDragging (bool dragging)
{
  _dragging = dragging;
}

bool
StructuralEntity::isSelected () const
{
  return _selected;
}

void
StructuralEntity::setSelected (bool selected)
{
  _selected = selected;
}

bool
StructuralEntity::isCollapsed () const
{
  return _collapsed;
}

void
StructuralEntity::setCollapsed (bool collapsed)
{
  _collapsed = collapsed;
  _props[ST_ATTR_ENT_COLLAPSED]
      = (_collapsed ? ST_VALUE_TRUE : ST_VALUE_FALSE);
}

qreal
StructuralEntity::top () const
{
  return _top;
}

void
StructuralEntity::setTop (qreal top)
{
  _top = top;
  _props[ST_ATTR_ENT_TOP] = QString::number (top);

  setY (top - 4);
}

qreal
StructuralEntity::moveTop () const
{
  return _moveTop;
}

void
StructuralEntity::setMoveTop (qreal moveTop)
{
  _moveTop = moveTop;
}

qreal
StructuralEntity::pressTop () const
{
  return _pressTop;
}

void
StructuralEntity::setPressTop (qreal pressTop)
{
  _pressTop = pressTop;
}

qreal
StructuralEntity::resizeTop () const
{
  return _resizeTop;
}

void
StructuralEntity::setResizeTop (qreal resizeTop)
{
  _resizeTop = resizeTop;
}

qreal
StructuralEntity::left () const
{
  return _left;
}

void
StructuralEntity::setLeft (qreal left)
{
  _left = left;
  _props[ST_ATTR_ENT_LEFT] = QString::number (left);

  setX (left - 4);
}

qreal
StructuralEntity::moveLeft () const
{
  return _moveLeft;
}

void
StructuralEntity::setMoveLeft (qreal moveLeft)
{
  _moveLeft = moveLeft;
}

qreal
StructuralEntity::pressLeft () const
{
  return _pressLeft;
}

void
StructuralEntity::setPressLeft (qreal pressLeft)
{
  _pressLeft = pressLeft;
}

qreal
StructuralEntity::resizeLeft () const
{
  return _resizeLeft;
}

void
StructuralEntity::setResizeLeft (qreal resizeLeft)
{
  _resizeLeft = resizeLeft;
}

qreal
StructuralEntity::width () const
{
  return _width;
}

void
StructuralEntity::setWidth (qreal width)
{
  _width = width;
  _props[ST_ATTR_ENT_WIDTH] = QString::number (width);
}

qreal
StructuralEntity::pressWidth () const
{
  return _pressWidth;
}

void
StructuralEntity::setPressWidth (qreal pressWidth)
{
  _pressWidth = pressWidth;
}

qreal
StructuralEntity::resizeWidth () const
{
  return _resizeWidth;
}

void
StructuralEntity::setResizeWidth (qreal resizeWidth)
{
  _resizeWidth = resizeWidth;
}

qreal
StructuralEntity::height () const
{
  return _height;
}

void
StructuralEntity::setHeight (qreal height)
{
  _height = height;
  _props[ST_ATTR_ENT_HEIGHT] = QString::number (height);
}

qreal
StructuralEntity::pressHeight () const
{
  return _pressHeight;
}

void
StructuralEntity::setPressHeight (qreal pressHeight)
{
  _pressHeight = pressHeight;
}

qreal
StructuralEntity::resizeHeight () const
{
  return _resizeHeight;
}

void
StructuralEntity::setResizeHeight (qreal resizeHeight)
{
  _resizeHeight = resizeHeight;
}

qreal
StructuralEntity::uncollapsedTop () const
{
  return _uncollapsedTop;
}

void
StructuralEntity::setUncollapsedTop (qreal uncollapedTop)
{
  _uncollapsedTop = uncollapedTop;
  _props[ST_ATTR_ENT_UNCOLLAPSED_TOP] = QString::number (uncollapedTop);
}

qreal
StructuralEntity::uncollapsedLeft () const
{
  return _uncollapsedLeft;
}

void
StructuralEntity::setUncollapsedLeft (qreal uncollapsedLeft)
{
  _uncollapsedLeft = uncollapsedLeft;
  _props[ST_ATTR_ENT_UNCOLLAPSED_LEFT] = QString::number (uncollapsedLeft);
}

qreal
StructuralEntity::uncollapsedWidth () const
{
  return _uncollapsedWidth;
}

void
StructuralEntity::setUncollapsedWidth (qreal uncollapsedWidth)
{
  _uncollapsedWidth = uncollapsedWidth;
  _props[ST_ATTR_ENT_UNCOLLAPSED_WIDTH] = QString::number (uncollapsedWidth);
}

qreal
StructuralEntity::uncollapsedHeight () const
{
  return _uncollapsedHeight;
}

void
StructuralEntity::setUncollapsedHeight (qreal uncollapsedHeight)
{
  _uncollapsedHeight = uncollapsedHeight;
  _props[ST_ATTR_ENT_UNCOLLAPSED_HEIGHT] = QString::number (uncollapsedHeight);
}

QRect
StructuralEntity::uncollapsedRect ()
{
  return QRect (_uncollapsedLeft, _uncollapsedTop, _uncollapsedWidth,
                _uncollapsedHeight);
}

qreal
StructuralEntity::zIndex () const
{
  return _zindex;
}

void
StructuralEntity::setzIndex (qreal zIndex)
{
  _zindex = zIndex;
  _props[ST_ATTR_ENT_ZINDEX] = QString::number (zIndex);

  setZValue (zIndex);
}

void
StructuralEntity::setMenu (StructuralMenu *menu)
{
  _menu = menu;
}

StructuralEntity *
StructuralEntity::structuralParent () const
{
  return _parent;
}

QString
StructuralEntity::error () const
{
  return _error;
}

void
StructuralEntity::setError (const QString &error)
{
  _error = error;
}

QString
StructuralEntity::warning () const
{
  return _warnning;
}

void
StructuralEntity::setWarning (const QString &warning)
{
  _warnning = warning;
}

QString
StructuralEntity::info () const
{
  return _info;
}

void
StructuralEntity::setInfo (const QString &info)
{
  _info = info;
}

void
StructuralEntity::setStructuralParent (StructuralEntity *parent)
{
  _parent = parent;

  QObject::setParent (parent);
  QGraphicsItem::setParentItem (parent);

  if (parent)
    parent->addChild (this);
}

QVector<StructuralEntity *>
StructuralEntity::children () const
{
  return _children;
}

void
StructuralEntity::addChild (StructuralEntity *entity)
{
  CPR_ASSERT (entity != nullptr);
  CPR_ASSERT (!_children.contains (entity));

  if (entity->structuralType () == Structural::Link)
    entity->setzIndex (_zindex + 1);
  else if (entity->structuralType () == Structural::Context
           || entity->structuralType () == Structural::Switch)
  {
    entity->setzIndex (_zindex + 2);
  }
  else
  {
    entity->setzIndex (_zindex);
  }

  _children.append (entity);
}

void
StructuralEntity::removeChild (StructuralEntity *ent)
{
  CPR_ASSERT_NON_NULL (ent);

  int index = _children.indexOf (ent);

  if (index >= 0)
    _children.remove (index);

  ent->setStructuralParent (nullptr);
}

bool
StructuralEntity::isChild (StructuralEntity *child) const
{
  CPR_ASSERT (this != child);

  for (StructuralEntity *c : children ())
  {
    if (c->uid () == child->uid () || c->isChild (child))
      return true;
  }

  return false;
}

QVector<QString>
StructuralEntity::restrictions () const
{
  return _restrictions;
}

void
StructuralEntity::addRestriction (const QString &restriction)
{
  if (!_restrictions.contains (restriction))
    _restrictions.append (restriction);
}

void
StructuralEntity::removeRestriction (const QString &restriction)
{
  Q_UNUSED (restriction);
}

void
StructuralEntity::delineate (QPainterPath *painter) const
{
  painter->addRect (ST_DEFAULT_ENTITY_PADDING, ST_DEFAULT_ENTITY_PADDING,
                    width (), height ());
}

void
StructuralEntity::move (QGraphicsSceneMouseEvent *event)
{
  // setting
  qreal x = left ();
  qreal y = top ();

  // setting delta
  qreal dx = event->pos ().x () - pressLeft (); // (x1 - x0)
  qreal dy = event->pos ().y () - pressTop ();  // (y1 - y0)

  // setting next position
  qreal nextx = x + dx;
  qreal nexty = y + dy;

  // moving
  setMoveTop (nexty);
  setMoveLeft (nextx);

  if (scene ())
    scene ()->update ();
}

void
StructuralEntity::resize (QGraphicsSceneMouseEvent *event)
{
  // setting
  qreal x = left ();
  qreal y = top ();
  qreal w = width ();
  qreal h = height ();

  // setting delta
  qreal dx = event->pos ().x () - pressLeft (); // (x1 - x0)
  qreal dy = event->pos ().y () - pressTop ();  // (y1 - y0)
  qreal dw = -dx;
  qreal dh = -dy;

  // setting next bounds
  qreal nextx = x + dx;
  qreal nexty = y + dy;
  qreal nextw = w + dw;
  qreal nexth = h + dh;

  // adjusting
  switch (structuralResize ())
  {
    case Structural::TopLeft:
    {
      break;
    }

    case Structural::Top:
    {
      nextx = x; // fixed x
      nextw = w; // fixed width

      break;
    }

    case Structural::TopRight:
    {
      nextx = x; // fixed x
      nextw = w - dw;

      break;
    }

    case Structural::Right:
    {
      nextx = x; // fixed x
      nextw = w - dw;

      nexty = y; // fixed y
      nexth = h; // fixed height

      break;
    }

    case Structural::BottomRight:
    {
      nextx = x; // fixed x
      nextw = w - dw;

      nexty = y; // fixed y
      nexth = h - dh;

      break;
    }

    case Structural::Bottom:
    {
      nextx = x; // fixed x
      nextw = w; // fixed width

      nexty = y; // fixed y
      nexth = h - dh;

      break;
    }

    case Structural::BottomLeft:
    {
      nexty = y; // fixed y
      nexth = h - dh;

      break;
    }

    case Structural::Left:
    {
      nexty = y; // fixed y
      nexth = h; // fixed height

      break;
    }

    case Structural::NoResize:
      // do nothing
      break;
  }

  // resizing
  setResizeTop (nexty);
  setResizeLeft (nextx);
  setResizeWidth (nextw);
  setResizeHeight (nexth);

  if (scene () != NULL)
    scene ()->update ();
}

QPainterPath
StructuralEntity::shape () const
{
  QPainterPath painter;

  delineate (&painter);

  if (_selectable && _selected && _resizable)
  {
    painter.setFillRule (Qt::WindingFill);

    qreal w = ST_DEFAULT_ENTITY_ANCHOR_W;
    qreal h = ST_DEFAULT_ENTITY_ANCHOR_H;

    painter.addRect (0, 0, w, h);                        // topleft
    painter.addRect ((_width + w) / 2 - w / 2, 0, w, h); // top
    painter.addRect ((_width + w) - w, 0, w, h);         // topright
    painter.addRect ((_width + w) - w, (_height + h) / 2 - h / 2, w,
                     h);                                         // right
    painter.addRect ((_width + w) - w, (_height + h) - h, w, h); // bottomright
    painter.addRect ((_width + w) / 2 - w / 2, (_height + h) - h, w,
                     h);                                  // bottom
    painter.addRect (0, (_height + h) - h, w, h);         // bottomleft
    painter.addRect (0, (_height + h) / 2 - h / 2, w, h); // left
  }

  return painter;
}

QRectF
StructuralEntity::boundingRect () const
{
  QRectF bounds;

  bounds.setX (0);
  bounds.setY (0);
  bounds.setWidth (_width + ST_DEFAULT_ENTITY_ANCHOR_W);
  bounds.setHeight (_height + ST_DEFAULT_ENTITY_ANCHOR_H);

  return bounds;
}

void
StructuralEntity::paint (QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
  Q_UNUSED (option);
  Q_UNUSED (widget);

//  if (_reference)
//    painter->setOpacity (0.5);

  draw (painter);

  if (_selectable && _selected)
  {
    painter->setRenderHint (QPainter::Antialiasing, false);

    painter->setBrush (Qt::NoBrush);
    painter->setPen (QPen (QBrush (Qt::black), 0, Qt::DashLine));

    painter->drawRect (ST_DEFAULT_ENTITY_ANCHOR_W / 2,
                       ST_DEFAULT_ENTITY_ANCHOR_H / 2, width (), height ());

    if (_resizable)
    {
      painter->setBrush (QBrush (Qt::white));
      painter->setPen (QPen (QBrush (Qt::black), 0));

      qreal w = ST_DEFAULT_ENTITY_ANCHOR_W;
      qreal h = ST_DEFAULT_ENTITY_ANCHOR_H;

      painter->drawRect (0, 0, w, h);                        // topleft
      painter->drawRect ((_width + w) / 2 - w / 2, 0, w, h); // top
      painter->drawRect ((_width + w) - w, 0, w, h);         // topright
      painter->drawRect ((_width + w) - w, (_height + h) / 2 - h / 2, w,
                         h); // right
      painter->drawRect ((_width + w) - w, (_height + h) - h, w,
                         h); // bottomright
      painter->drawRect ((_width + w) / 2 - w / 2, (_height + h) - h, w,
                         h);                                  // bottom
      painter->drawRect (0, (_height + h) - h, w, h);         // bottomleft
      painter->drawRect (0, (_height + h) / 2 - h / 2, w, h); // left
    }
  }
  else if (_hoverable && _hovering)
  {
    painter->setBrush (Qt::NoBrush);
    painter->setPen (QPen (QBrush (QColor ("#999999")), 0, Qt::DashLine));

    painter->drawRect (ST_DEFAULT_ENTITY_PADDING, ST_DEFAULT_ENTITY_PADDING,
                       width (), height ());
  }
}

void
StructuralEntity::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
  if (_moveable && _moving)
  {
    move (event);
  }
  else if (_resizable && _resizing)
  {
    resize (event);
  }
  else if (_draggable && _dragging)
  {
    QDrag *drag = new QDrag (event->widget ());
    QMimeData *minedata = new QMimeData ();

    minedata->setObjectName (StructuralUtil::typetostr (_type));
    minedata->setText (_uid);

    drag->setMimeData (minedata);
    drag->exec ();
  }
}

void
StructuralEntity::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
  if (event->button () == Qt::LeftButton)
  {
    if (_selectable /*&& !_selected*/)
    {
      setSelected (true);
      emit selectAsked (uid (), StructuralUtil::createSettings ());
    }

    setPressTop (event->pos ().y ());
    setPressLeft (event->pos ().x ());
    setPressWidth (_width);
    setPressHeight (_height);

    qreal w = ST_DEFAULT_ENTITY_ANCHOR_W;
    qreal h = ST_DEFAULT_ENTITY_ANCHOR_H;

    if (_draggable)
    {
      setDragging (true);
    }
    else if (_resizable)
    {
      setResizeTop (_top);
      setResizeLeft (_left);
      setResizeWidth (_width);
      setResizeHeight (_height);

      if (QRectF (0, 0, w, h).contains (event->pos ()))
      {
        // if over TOPLEFT resize region
        setStructuralResize (Structural::TopLeft);
        setResizing (true);
      }
      else if (QRectF ((_width + w) / 2 - w / 2, 0, w, h)
                   .contains (event->pos ()))
      {
        // if over TOP resize region
        setStructuralResize (Structural::Top);
        setResizing (true);
      }
      else if (QRectF ((_width + w) - w, 0, w, h).contains (event->pos ()))
      {
        // if over TOPRIGHT resize region
        setStructuralResize (Structural::TopRight);
        setResizing (true);
      }
      else if (QRectF ((_width + w) - w, (_height + h) / 2 - h / 2, w, h)
                   .contains (event->pos ()))
      {
        // if over RIGHT resize region
        setStructuralResize (Structural::Right);
        setResizing (true);
      }
      else if (QRectF ((_width + w) - w, (_height + h) - h, w, h)
                   .contains (event->pos ()))
      {
        // if over BOTTOMRIGHT resize region
        setStructuralResize (Structural::BottomRight);
        setResizing (true);
      }
      else if (QRectF ((_width + w) / 2 - w / 2, (_height + h) - h, w, h)
                   .contains (event->pos ()))
      {
        // if over BOTTOM resize region
        setStructuralResize (Structural::Bottom);
        setResizing (true);
      }
      else if (QRectF (0, (_height + h) - h, w, h).contains (event->pos ()))
      {
        // if over BOTTOMLEFT resize region
        setStructuralResize (Structural::BottomLeft);
        setResizing (true);
      }
      else if (QRectF (0, (_height + h) / 2 - h / 2, w, h)
                   .contains (event->pos ()))
      {
        // if over LEFT resize region
        setStructuralResize (Structural::Left);
        setResizing (true);
      }
      else if (_moveable)
      {
        // if not over any resize region
        setMoveTop (_top);
        setMoveLeft (_left);

        setMoving (true);
      }
    }
    else if (_moveable)
    {
      setMoveTop (_top);
      setMoveLeft (_left);

      setMoving (true);
    }
  }

  QGraphicsItem::mousePressEvent (event);
}

void
StructuralEntity::mouseReleaseEvent (QGraphicsSceneMouseEvent *event)
{
  if (_moving)
  {
    setMoving (false);

    if ((_top != _moveTop || _left != _moveLeft))
    {
      QMap<QString, QString> previous = properties ();

      setTop (_moveTop);
      setLeft (_moveLeft);

      emit changeAsked (uid (), properties (), previous,
                        StructuralUtil::createSettings ());
    }
  }
  else if (_resizing)
  {
    setResizing (false);

    QMap<QString, QString> previous = properties ();

    if ((_top != _resizeTop || _left != _resizeLeft || _width != _resizeWidth
         || _height != _resizeHeight))
    {
      if (_resizeTop > _top + _height)
      {
        setResizeHeight (_resizeTop - (_top + _height));
        setResizeTop (_top + _height);
      }

      if (_resizeLeft > _left + _width)
      {
        setResizeWidth (_resizeLeft - (_left + _width));
        setResizeLeft (_left + _width);
      }

      if (_resizeWidth < 0)
      {
        setResizeLeft (_resizeLeft + _resizeWidth);
        setResizeWidth (-_resizeWidth);
      }

      if (_resizeHeight < 0)
      {
        setResizeTop (_resizeTop + _resizeHeight);
        setResizeHeight (-_resizeHeight);
      }

      for (StructuralEntity *ent : _children)
      {
        ent->setTop (ent->top () - (_resizeTop - _top));
        ent->setLeft (ent->left () - (_resizeLeft - _left));
      }

      setTop (_resizeTop);
      setLeft (_resizeLeft);
      setWidth (_resizeWidth);
      setHeight (_resizeHeight);

      emit changeAsked (uid (), properties (), previous,
                        StructuralUtil::createSettings ());
    }
  }

  update ();

  QGraphicsItem::mouseReleaseEvent (event);
}

void
StructuralEntity::contextMenuEvent (QGraphicsSceneContextMenuEvent *event)
{
  QGraphicsItem::contextMenuEvent (event);

  if (!event->isAccepted ())
  {
    if (!isSelected ())
    {
      setSelected (true);
      emit selectAsked (uid (), StructuralUtil::createSettings ());
    }

    CPR_ASSERT (_menu != nullptr);
    _menu->exec (event->screenPos (), this);
    event->accept ();
  }
}

void
StructuralEntity::hoverMoveEvent (QGraphicsSceneHoverEvent *event)
{
  if (_selected)
  {
    if (_resizable)
    {
      qreal w = ST_DEFAULT_ENTITY_ANCHOR_W;
      qreal h = ST_DEFAULT_ENTITY_ANCHOR_H;

      if (QRectF (0, 0, w, h).contains (event->pos ()))
      {
        // if over TOPLEFT resize region
        setCursor (Qt::SizeFDiagCursor);
      }
      else if (QRectF ((_width + w) / 2 - w / 2, 0, w, h)
                   .contains (event->pos ()))
      {
        // if over TOP resize region
        setCursor (Qt::SizeVerCursor);
      }
      else if (QRectF ((_width + w) - w, 0, w, h).contains (event->pos ()))
      {
        // if over TOPRIGHT resize region
        setCursor (Qt::SizeBDiagCursor);
      }
      else if (QRectF ((_width + w) - w, (_height + h) / 2 - h / 2, w, h)
                   .contains (event->pos ()))
      {
        // if over RIGHT resize region
        setCursor (Qt::SizeHorCursor);
      }
      else if (QRectF ((_width + w) - w, (_height + h) - h, w, h)
                   .contains (event->pos ()))
      {
        // if over BOTTOMRIGHT resize region
        setCursor (Qt::SizeFDiagCursor);
      }
      else if (QRectF ((_width + w) / 2 - w / 2, (_height + h) - h, w, h)
                   .contains (event->pos ()))
      {
        // if over BOTTOM resize region
        setCursor (Qt::SizeVerCursor);
      }
      else if (QRectF (0, (_height + h) - h, w, h).contains (event->pos ()))
      {
        // if over BOTTOMLEFT resize region
        setCursor (Qt::SizeBDiagCursor);
      }
      else if (QRectF (0, (_height + h) / 2 - h / 2, w, h)
                   .contains (event->pos ()))
      {
        // if over LEFT resize region
        setCursor (Qt::SizeHorCursor);
      }
      else if (_moveable)
      {
        // if not over any resize region
        setCursor (Qt::SizeAllCursor);
      }
    }
    else if (_moveable)
    {
      setCursor (Qt::SizeAllCursor);
    }
    else
    {
      setCursor (Qt::ArrowCursor);
    }
  }
  else
  {
    setCursor (Qt::ArrowCursor);
  }
}

void
StructuralEntity::hoverEnterEvent (QGraphicsSceneHoverEvent *event)
{
  QGraphicsItem::hoverEnterEvent (event);

  setHovering (true);
}

void
StructuralEntity::hoverLeaveEvent (QGraphicsSceneHoverEvent *event)
{
  QGraphicsItem::hoverLeaveEvent (event);

  setHovering (false);
}

void
StructuralEntity::dragEnterEvent (QGraphicsSceneDragDropEvent *event)
{
  StructuralType type
      = StructuralUtil::strtotype (event->mimeData ()->objectName ());

  if (StructuralUtil::validateKinship (type, _type)
      && event->mimeData ()->text () != _uid)
    event->setAccepted (true);
  else
    event->setAccepted (false);
}

void
StructuralEntity::dragMoveEvent (QGraphicsSceneDragDropEvent *event)
{
  StructuralType type
      = StructuralUtil::strtotype (event->mimeData ()->objectName ());

  if (StructuralUtil::validateKinship (type, _type)
      && event->mimeData ()->text () != _uid)
    event->setAccepted (true);
  else
    event->setAccepted (false);
}

void
StructuralEntity::dropEvent (QGraphicsSceneDragDropEvent *event)
{
  StructuralType type
      = StructuralUtil::strtotype (event->mimeData ()->objectName ());

  if (StructuralUtil::validateKinship (type, _type)
      && event->mimeData ()->text () != _uid)
  {
    QMap<QString, QString> props
        = { { ST_ATTR_ENT_TOP, QString::number (event->pos ().y ()) },
            { ST_ATTR_ENT_LEFT, QString::number (event->pos ().x ()) } };

    emit moveAsked (event->mimeData ()->text (), _uid, props,
                    StructuralUtil::createSettings ());
  }
}
