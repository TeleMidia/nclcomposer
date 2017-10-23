#include "StructuralEntity.h"

#include "util/Utilities.h"
#include <QMimeData>

StructuralEntity::StructuralEntity (StructuralEntity *parent)
    : QObject (parent), QGraphicsItem (parent)
{
  setCategory (Structural::NoCategory);
  setType (Structural::NoType);

  setUid (StructuralUtil::createUid ());
  setId ("");
  setParent (parent);

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

  setReference (false);

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

  setUncollapedTop (0);
  setUncollapedLeft (0);
  setUncollapedWidth (0);
  setUncollapedHeight (0);

  setzIndex (0);

  setFlag (QGraphicsItem::ItemIsMovable, true);
  setFlag (QGraphicsItem::ItemIsFocusable, true);

  setAcceptHoverEvents (true);
  setAcceptDrops (true);

  setSelected (false);
  setUncollapsed (true);
}

StructuralEntity::~StructuralEntity () {}

QString
StructuralEntity::getUid () const
{
  return _uid;
}

void
StructuralEntity::setUid (const QString &uid)
{
  _uid = uid;
  _properties[ST_ATTR_ENT_UID] = _uid;
}

QString
StructuralEntity::getId () const
{
  return _id;
}

void
StructuralEntity::setId (const QString &id)
{
  _id = id;
  _properties.insert (ST_ATTR_ENT_ID, _id);
}

StructuralCategory
StructuralEntity::getCategory () const
{
  return _category;
}

void
StructuralEntity::setCategory (StructuralCategory category)
{
  _category = category;
  _properties[ST_ATTR_ENT_CATEGORY]
      = StructuralUtil::translateCategoryToString (_category);
}

StructuralType
StructuralEntity::getType () const
{
  return _type;
}

void
StructuralEntity::setType (StructuralType type)
{
  _type = type;
  _properties[ST_ATTR_ENT_TYPE] = StructuralUtil::typeToString (_type);

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

      addRestriction (ST_ATTR_REFERENCE_REFER_ID);
      addRestriction (ST_ATTR_REFERENCE_REFER_UID);

      break;
    }

    case Structural::Port:
    case Structural::Mapping:
    {
      addRestriction (ST_ATTR_REFERENCE_COMPONENT_ID);
      addRestriction (ST_ATTR_REFERENCE_COMPONENT_UID);
      addRestriction (ST_ATTR_REFERENCE_INTERFACE_ID);
      addRestriction (ST_ATTR_REFERENCE_INTERFACE_UID);

      break;
    }

    case Structural::Bind:
    {
      addRestriction (ST_ATTR_REFERENCE_COMPONENT_ID);
      addRestriction (ST_ATTR_REFERENCE_COMPONENT_UID);
      addRestriction (ST_ATTR_REFERENCE_INTERFACE_ID);
      addRestriction (ST_ATTR_REFERENCE_INTERFACE_UID);

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
StructuralEntity::getStructuralResize () const
{
  return _resize;
}

void
StructuralEntity::setStructuralResize (StructuralResize resize)
{
  _resize = resize;
}

QMap<QString, QString>
StructuralEntity::getProperties () const
{
  return _properties;
}

void
StructuralEntity::setProperties (const QMap<QString, QString> &properties)
{
  if (!_restrictions.isEmpty ())
  {
    QMap<QString, QString> previous = _properties;

    foreach (const QString &key, previous.keys ())
    {
      foreach (const QString &restriction, _restrictions)
      {
        if (key.contains (restriction))
          if (!properties.contains (key))
            _properties.remove (key);
      }
    }
  }

  foreach (const QString &name, properties.keys ())
    _properties[name] = properties.value (name);

  adjust (true);
}

QString
StructuralEntity::getProperty (const QString &name) const
{
  if (_properties.contains (name))
    return _properties.value (name, "");
  else
    return "";
}

void
StructuralEntity::setProperty (const QString &name, const QString &value)
{
  _properties[name] = value;
}

void
StructuralEntity::adjust (bool collision, bool recursion)
{
  Q_UNUSED (collision);
  Q_UNUSED (recursion);

  setId (_properties[ST_ATTR_ENT_ID]);
  setUid (_properties[ST_ATTR_ENT_UID]);

  setCategory (StructuralUtil::translateStringToCategory (
      _properties[ST_ATTR_ENT_CATEGORY]));
  setType (StructuralUtil::stringToType (_properties[ST_ATTR_ENT_TYPE]));

  setTop (_properties[ST_ATTR_ENT_TOP].toDouble ());
  setLeft (_properties[ST_ATTR_ENT_LEFT].toDouble ());
  setWidth (_properties[ST_ATTR_ENT_WIDTH].toDouble ());
  setHeight (_properties[ST_ATTR_ENT_HEIGHT].toDouble ());

  setUncollapedTop (_properties[ST_ATTR_ENT_UNCOLLAPSED_TOP].toDouble ());
  setUncollapedLeft (_properties[ST_ATTR_ENT_UNCOLLAPSED_LEFT].toDouble ());
  setUncollapedWidth (_properties[ST_ATTR_ENT_UNCOLLAPSED_WIDTH].toDouble ());
  setUncollapedHeight (
      _properties[ST_ATTR_ENT_UNCOLLAPSED_HEIGHT].toDouble ());

  setzIndex (_properties[ST_ATTR_ENT_ZINDEX].toInt ());

  setHidden (
      (_properties[ST_ATTR_ENT_HIDDEN] == ST_VALUE_TRUE ? true : false));
  setReference (
      (_properties[ST_ATTR_ENT_REFERENCE] == ST_VALUE_TRUE ? true : false));
  setUncollapsed (
      (_properties[ST_ATTR_ENT_UNCOLLAPSED] == ST_VALUE_TRUE ? true : false));

  setToolTip (StructuralUtil::getTooltip (
      _type, _id, _info, _warnning, _error,
      _properties[ST_ATTR_REFERENCE_XCONNECTOR_ID]));

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
  _properties[ST_ATTR_ENT_HIDDEN] = (hidden ? ST_VALUE_TRUE : ST_VALUE_FALSE);

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
StructuralEntity::isUncollapsed () const
{
  return _uncollapsed;
}

void
StructuralEntity::setUncollapsed (bool uncollapsed)
{
  _uncollapsed = uncollapsed;
  _properties[ST_ATTR_ENT_UNCOLLAPSED]
      = (uncollapsed ? ST_VALUE_TRUE : ST_VALUE_FALSE);
}

bool
StructuralEntity::isReference () const
{
  return _reference;
}

void
StructuralEntity::setReference (bool reference)
{
  _reference = reference;
  _properties[ST_ATTR_ENT_REFERENCE]
      = (reference ? ST_VALUE_TRUE : ST_VALUE_FALSE);
}

qreal
StructuralEntity::getTop () const
{
  return _top;
}

void
StructuralEntity::setTop (qreal top)
{
  _top = top;
  _properties[ST_ATTR_ENT_TOP] = QString::number (top);

  setY (top - 4);
}

qreal
StructuralEntity::getMoveTop () const
{
  return _moveTop;
}

void
StructuralEntity::setMoveTop (qreal moveTop)
{
  _moveTop = moveTop;
}

qreal
StructuralEntity::getPressTop () const
{
  return _pressTop;
}

void
StructuralEntity::setPressTop (qreal pressTop)
{
  _pressTop = pressTop;
}

qreal
StructuralEntity::getResizeTop () const
{
  return _resizeTop;
}

void
StructuralEntity::setResizeTop (qreal resizeTop)
{
  _resizeTop = resizeTop;
}

qreal
StructuralEntity::getLeft () const
{
  return _left;
}

void
StructuralEntity::setLeft (qreal left)
{
  _left = left;
  _properties[ST_ATTR_ENT_LEFT] = QString::number (left);

  setX (left - 4);
}

qreal
StructuralEntity::getMoveLeft () const
{
  return _moveLeft;
}

void
StructuralEntity::setMoveLeft (qreal moveLeft)
{
  _moveLeft = moveLeft;
}

qreal
StructuralEntity::getPressLeft () const
{
  return _pressLeft;
}

void
StructuralEntity::setPressLeft (qreal pressLeft)
{
  _pressLeft = pressLeft;
}

qreal
StructuralEntity::getResizeLeft () const
{
  return _resizeLeft;
}

void
StructuralEntity::setResizeLeft (qreal resizeLeft)
{
  _resizeLeft = resizeLeft;
}

qreal
StructuralEntity::getWidth () const
{
  return _width;
}

void
StructuralEntity::setWidth (qreal width)
{
  _width = width;
  _properties[ST_ATTR_ENT_WIDTH] = QString::number (width);
}

qreal
StructuralEntity::getPressWidth () const
{
  return _pressWidth;
}

void
StructuralEntity::setPressWidth (qreal pressWidth)
{
  _pressWidth = pressWidth;
}

qreal
StructuralEntity::getResizeWidth () const
{
  return _resizeWidth;
}

void
StructuralEntity::setResizeWidth (qreal resizeWidth)
{
  _resizeWidth = resizeWidth;
}

qreal
StructuralEntity::getHeight () const
{
  return _height;
}

void
StructuralEntity::setHeight (qreal height)
{
  _height = height;
  _properties[ST_ATTR_ENT_HEIGHT] = QString::number (height);
}

qreal
StructuralEntity::getPressHeight () const
{
  return _pressHeight;
}

void
StructuralEntity::setPressHeight (qreal pressHeight)
{
  _pressHeight = pressHeight;
}

qreal
StructuralEntity::getResizeHeight () const
{
  return _resizeHeight;
}

void
StructuralEntity::setResizeHeight (qreal resizeHeight)
{
  _resizeHeight = resizeHeight;
}

qreal
StructuralEntity::getUncollapedTop () const
{
  return _uncollapsedTop;
}

void
StructuralEntity::setUncollapedTop (qreal uncollapedTop)
{
  _uncollapsedTop = uncollapedTop;
  _properties[ST_ATTR_ENT_UNCOLLAPSED_TOP] = QString::number (uncollapedTop);
}

qreal
StructuralEntity::getUncollapedLeft () const
{
  return _uncollapsedLeft;
}

void
StructuralEntity::setUncollapedLeft (qreal uncollapedLeft)
{
  _uncollapsedLeft = uncollapedLeft;
  _properties[ST_ATTR_ENT_UNCOLLAPSED_LEFT] = QString::number (uncollapedLeft);
}

qreal
StructuralEntity::getUncollapedWidth () const
{
  return _uncollapsedWidth;
}

void
StructuralEntity::setUncollapedWidth (qreal uncollapedWidth)
{
  _uncollapsedWidth = uncollapedWidth;
  _properties[ST_ATTR_ENT_UNCOLLAPSED_WIDTH]
      = QString::number (uncollapedWidth);
}

qreal
StructuralEntity::getUncollapedHeight () const
{
  return _uncollapsedHeight;
}

void
StructuralEntity::setUncollapedHeight (qreal uncollapedHeight)
{
  _uncollapsedHeight = uncollapedHeight;
  _properties[ST_ATTR_ENT_UNCOLLAPSED_HEIGHT]
      = QString::number (uncollapedHeight);
}

qreal
StructuralEntity::getzIndex () const
{
  return _zindex;
}

void
StructuralEntity::setzIndex (qreal zIndex)
{
  _zindex = zIndex;
  _properties[ST_ATTR_ENT_ZINDEX] = QString::number (zIndex);

  setZValue (zIndex);
}

void
StructuralEntity::setMenu (StructuralMenu *menu)
{
  _menu = menu;
}

StructuralEntity *
StructuralEntity::getParent () const
{
  return _parent;
}

QString
StructuralEntity::getError () const
{
  return _error;
}

void
StructuralEntity::setError (const QString &error)
{
  _error = error;
}

QString
StructuralEntity::getWarning () const
{
  return _warnning;
}

void
StructuralEntity::setWarning (const QString &warning)
{
  _warnning = warning;
}

QString
StructuralEntity::getInfo () const
{
  return _info;
}

void
StructuralEntity::setInfo (const QString &info)
{
  _info = info;
}

void
StructuralEntity::setParent (StructuralEntity *parent)
{
  _parent = parent;

  QObject::setParent (parent);
  QGraphicsItem::setParentItem (parent);

  if (parent != NULL)
    parent->addChild (this);
}

QVector<StructuralEntity *>
StructuralEntity::getChildren () const
{
  return _children;
}

void
StructuralEntity::addChild (StructuralEntity *entity)
{
  CPR_ASSERT (entity != nullptr);
  CPR_ASSERT (!_children.contains (entity));

  if (entity->getType () == Structural::Link)
    entity->setzIndex (_zindex + 1);
  else if (entity->getType () == Structural::Context
           || entity->getType () == Structural::Switch)
  {
    entity->setzIndex (_zindex + 2);
    ;
  }
  else
  {
    entity->setzIndex (_zindex);
  }

  _children.append (entity);
}

void
StructuralEntity::removeChild (StructuralEntity *entity)
{
  CPR_ASSERT (entity != nullptr);

  int index = _children.indexOf (entity);

  if (index >= 0)
    _children.remove (index);

  entity->setParent (NULL);
}

QVector<QString>
StructuralEntity::getRestrictions () const
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
                    getWidth (), getHeight ());
}

void
StructuralEntity::move (QGraphicsSceneMouseEvent *event)
{
  // setting
  qreal x = getLeft ();
  qreal y = getTop ();

  // setting delta
  qreal dx = event->pos ().x () - getPressLeft (); // (x1 - x0)
  qreal dy = event->pos ().y () - getPressTop ();  // (y1 - y0)

  // setting next position
  qreal nextx = x + dx;
  qreal nexty = y + dy;

  // moving
  setMoveTop (nexty);
  setMoveLeft (nextx);

  if (scene () != NULL)
    scene ()->update ();
}

void
StructuralEntity::resize (QGraphicsSceneMouseEvent *event)
{
  // setting
  qreal x = getLeft ();
  qreal y = getTop ();
  qreal w = getWidth ();
  qreal h = getHeight ();

  // setting delta
  qreal dx = event->pos ().x () - getPressLeft (); // (x1 - x0)
  qreal dy = event->pos ().y () - getPressTop ();  // (y1 - y0)
  qreal dw = -dx;
  qreal dh = -dy;

  // setting next bounds
  qreal nextx = x + dx;
  qreal nexty = y + dy;
  qreal nextw = w + dw;
  qreal nexth = h + dh;

  // adjusting
  switch (getStructuralResize ())
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

  if (_reference)
    painter->setOpacity (0.5);

  draw (painter);

  if (_selectable && _selected)
  {
    painter->setRenderHint (QPainter::Antialiasing, false);

    painter->setBrush (Qt::NoBrush);
    painter->setPen (QPen (QBrush (Qt::black), 0, Qt::DashLine));

    painter->drawRect (ST_DEFAULT_ENTITY_ANCHOR_W / 2,
                       ST_DEFAULT_ENTITY_ANCHOR_H / 2, getWidth (),
                       getHeight ());

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
                       getWidth (), getHeight ());
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

    minedata->setObjectName (StructuralUtil::typeToString (_type));
    minedata->setText (_uid);

    drag->setMimeData (minedata);
    drag->exec ();
  }
}

void
StructuralEntity::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
  if (event->button () == Qt::RightButton)
  {
    event->ignore ();
  }
  else if (event->button () == Qt::LeftButton)
  {
    if (_selectable && !_selected)
    {
      setSelected (true);
      emit selected (getUid (), StructuralUtil::createSettings ());
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
      QMap<QString, QString> previous = getProperties ();

      setTop (_moveTop);
      setLeft (_moveLeft);

      emit changed (getUid (), getProperties (), previous,
                    StructuralUtil::createSettings ());
    }
  }
  else if (_resizing)
  {
    setResizing (false);

    QMap<QString, QString> previous = getProperties ();

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

      foreach (StructuralEntity *entity, _children)
      {
        entity->setTop (entity->getTop () - (_resizeTop - _top));
        entity->setLeft (entity->getLeft () - (_resizeLeft - _left));
      }

      setTop (_resizeTop);
      setLeft (_resizeLeft);
      setWidth (_resizeWidth);
      setHeight (_resizeHeight);

      emit changed (getUid (), getProperties (), previous,
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
      emit selected (getUid (), StructuralUtil::createSettings ());
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
      = StructuralUtil::stringToType (event->mimeData ()->objectName ());

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
      = StructuralUtil::stringToType (event->mimeData ()->objectName ());

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
      = StructuralUtil::stringToType (event->mimeData ()->objectName ());

  if (StructuralUtil::validateKinship (type, _type)
      && event->mimeData ()->text () != _uid)
  {
    QMap<QString, QString> properties;
    properties[ST_ATTR_ENT_TOP] = QString::number (event->pos ().y ());
    properties[ST_ATTR_ENT_LEFT] = QString::number (event->pos ().x ());

    emit moved (event->mimeData ()->text (), _uid, properties,
                StructuralUtil::createSettings ());
  }
}
