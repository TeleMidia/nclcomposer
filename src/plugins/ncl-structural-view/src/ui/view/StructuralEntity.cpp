#include "StructuralEntity.h"

#include <QMimeData>

StructuralEntity::StructuralEntity(StructuralEntity* parent)
  : QObject(parent), QGraphicsItem(parent)
{
  setStructuralCategory(Structural::NoCategory);
  setStructuralType(Structural::NoType);

  setStructuralUid(StructuralUtil::createUid());
  setStructuralParent(parent);

  setMoveable(true);
  setResizable(true);
  setSelectable(true);
  setHoverable(true);
  setDraggable(false);

  setMoving(false);
  setHidden(false);
  setResizing(false);
  setSelected(false);
  setDragging(false);
  setHovering(false);

  setReference(false);

  setTop(0);
  setLeft(0);
  setWidth(0);
  setHeight(0);

  setMoveTop(0);
  setMoveLeft(0);

  setPressTop(0);
  setPressLeft(0);
  setPressWidth(0);
  setPressHeight(0);

  setResizeTop(0);
  setResizeLeft(0);
  setResizeWidth(0);
  setResizeHeight(0);

  setUncollapedTop(0);
  setUncollapedLeft(0);
  setUncollapedWidth(0);
  setUncollapedHeight(0);

  setzIndex(0);

  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsFocusable, true);

  setAcceptHoverEvents(true);
  setAcceptDrops(true);

  setSelected(false);
  setUncollapsed(true);
}

StructuralEntity::~StructuralEntity()
{

}

QString StructuralEntity::getStructuralUid() const
{
  return _uid;
}

void StructuralEntity::setStructuralUid(const QString &uid)
{
  _uid = uid;
  _properties[STR_PROPERTY_ENTITY_UID] = _uid;
}

QString StructuralEntity::getStructuralId() const
{
  return _id;
}

void StructuralEntity::setStructuralId(const QString &id)
{
  _id = id;
  _properties.insert(STR_PROPERTY_ENTITY_ID, _id) ;
}

StructualCategory StructuralEntity::getStructuralCategory() const
{
  return _category;
}

void StructuralEntity::setStructuralCategory(StructualCategory category)
{
  _category = category;
  _properties[STR_PROPERTY_ENTITY_CATEGORY] = StructuralUtil::translateCategoryToString(_category);
}

StructuralType StructuralEntity::getStructuralType() const
{
  return _type;
}

void StructuralEntity::setStructuralType(StructuralType type)
{
  _type = type;
  _properties[STR_PROPERTY_ENTITY_TYPE] = StructuralUtil::translateTypeToString(_type);

  _restrictions.clear();

//  addStructuralRestriction(STR_PROPERTY_ENTITY_AUTOSTART);

  switch (type)
  {
    case Structural::Media:
    case Structural::Context:
    case Structural::Switch:
    {
      if (type == Structural::Media)
      {
        addStructuralRestriction(STR_PROPERTY_CONTENT_TYPE);
        addStructuralRestriction(STR_PROPERTY_CONTENT_LOCATION);
        addStructuralRestriction(STR_PROPERTY_CONTENT_INSTANCE);
      }

      addStructuralRestriction(STR_PROPERTY_REFERENCE_REFER_ID);
      addStructuralRestriction(STR_PROPERTY_REFERENCE_REFER_UID);

      break;
    }

    case Structural::Port:
    case Structural::Mapping:
    {
      addStructuralRestriction(STR_PROPERTY_REFERENCE_COMPONENT_ID);
      addStructuralRestriction(STR_PROPERTY_REFERENCE_COMPONENT_UID);
      addStructuralRestriction(STR_PROPERTY_REFERENCE_INTERFACE_ID);
      addStructuralRestriction(STR_PROPERTY_REFERENCE_INTERFACE_UID);

      break;
    }

    case Structural::Bind:
    {
      addStructuralRestriction(STR_PROPERTY_REFERENCE_COMPONENT_ID);
      addStructuralRestriction(STR_PROPERTY_REFERENCE_COMPONENT_UID);
      addStructuralRestriction(STR_PROPERTY_REFERENCE_INTERFACE_ID);
      addStructuralRestriction(STR_PROPERTY_REFERENCE_INTERFACE_UID);

      addStructuralRestriction(STR_PROPERTY_LINKPARAM_NAME);
      addStructuralRestriction(STR_PROPERTY_LINKPARAM_VALUE);
      addStructuralRestriction(STR_PROPERTY_BINDPARAM_NAME);
      addStructuralRestriction(STR_PROPERTY_BINDPARAM_VALUE);

      break;
    }

    case Structural::Link:
    {
      addStructuralRestriction(STR_PROPERTY_LINKPARAM_NAME);
      addStructuralRestriction(STR_PROPERTY_LINKPARAM_VALUE);

      break;
    }

    case Structural::Body:
    {
      setzIndex(1);

      break;
    }

    default:
    {
      break;
    }
  }
}

StructuralResize StructuralEntity::getStructuralResize() const
{
  return _resize;
}

void StructuralEntity::setStructuralResize(StructuralResize resize)
{
  _resize = resize;
}

QMap<QString, QString> StructuralEntity::getStructuralProperties() const
{
  return _properties;
}

void StructuralEntity::setStructuralProperties(const QMap<QString, QString> &properties)
{
  if (!_restrictions.isEmpty()) {
    QMap<QString, QString> previous = _properties;

    foreach (QString key, previous.keys()) {
      foreach (QString restriction, _restrictions) {
        if (key.contains(restriction))
          if (!properties.contains(key))
            _properties.remove(key);
      }
    }
  }

  foreach (QString name, properties.keys())
    _properties[name] = properties.value(name);

  adjust(true);
}

QString StructuralEntity::getStructuralProperty(const QString &name) const
{
  return _properties.value(name,"");
}

void StructuralEntity::setStructuralProperty(const QString &name, const QString &value)
{
  _properties[name] = value;
}

void StructuralEntity::adjust(bool collision, bool recursion)
{
  Q_UNUSED(collision);
  Q_UNUSED(recursion);

  setStructuralId(_properties[STR_PROPERTY_ENTITY_ID]);
  setStructuralUid(_properties[STR_PROPERTY_ENTITY_UID]);
  setStructuralCategory(StructuralUtil::translateStringToCategory(_properties[STR_PROPERTY_ENTITY_CATEGORY]));
  setStructuralType(StructuralUtil::translateStringToType(_properties[STR_PROPERTY_ENTITY_TYPE]));

  setTop(_properties[STR_PROPERTY_ENTITY_TOP].toDouble());
  setLeft(_properties[STR_PROPERTY_ENTITY_LEFT].toDouble());
  setWidth(_properties[STR_PROPERTY_ENTITY_WIDTH].toDouble());
  setHeight(_properties[STR_PROPERTY_ENTITY_HEIGHT].toDouble());

  setUncollapedTop(_properties[STR_PROPERTY_ENTITY_UNCOLLAPSED_TOP].toDouble());
  setUncollapedLeft(_properties[STR_PROPERTY_ENTITY_UNCOLLAPSED_LEFT].toDouble());
  setUncollapedWidth(_properties[STR_PROPERTY_ENTITY_UNCOLLAPSED_WIDTH].toDouble());
  setUncollapedHeight(_properties[STR_PROPERTY_ENTITY_UNCOLLAPSED_HEIGHT].toDouble());

  setzIndex(_properties[STR_PROPERTY_ENTITY_ZINDEX].toInt());

  setHidden((_properties[STR_PROPERTY_ENTITY_HIDDEN] == STR_VALUE_TRUE ? true : false));
  setReference((_properties[STR_PROPERTY_ENTITY_REFERENCE] == STR_VALUE_TRUE ? true : false));
  setUncollapsed((_properties[STR_PROPERTY_ENTITY_UNCOLLAPSED] == STR_VALUE_TRUE ? true : false));

  setToolTip(StructuralUtil::getTooltip(_type, _id, _info, _warnning, _error, _properties[STR_PROPERTY_REFERENCE_XCONNECTOR_ID]));

  if (scene() != NULL)
    scene()->update();
}

bool StructuralEntity::isMoveable() const
{
  return _moveable;
}

void StructuralEntity::setMoveable(bool moveable)
{
  _moveable = moveable;
}

bool StructuralEntity::isSelectable() const
{
  return _selectable;
}

void StructuralEntity::setSelectable(bool selectable)
{
  _selectable = selectable;
}

bool StructuralEntity::isHoverable() const
{
  return _hoverable;
}

void StructuralEntity::setHoverable(bool hoverable)
{
  _hoverable = hoverable;
}

bool StructuralEntity::isResizable() const
{
  return _resizable;
}

void StructuralEntity::setResizable(bool resizable)
{
  _resizable = resizable;
}

bool StructuralEntity::isDraggable()
{
  return _draggable;
}

void StructuralEntity::setDraggable(bool draggable)
{
  _draggable = draggable;
}

bool StructuralEntity::isMoving() const
{
  return _moving;
}

void StructuralEntity::setMoving(bool moving)
{
  _moving = moving;
}

bool StructuralEntity::isHidden() const
{
  return _hidden;
}

void StructuralEntity::setHidden(bool hidden)
{
  _hidden = hidden;
  _properties[STR_PROPERTY_ENTITY_HIDDEN] = (hidden ? STR_VALUE_TRUE : STR_VALUE_FALSE);

  setVisible(!hidden);
}

bool StructuralEntity::isResizing() const
{
  return _resizing;
}

void StructuralEntity::setResizing(bool resizing)
{
  _resizing = resizing;
}

bool StructuralEntity::isHovering() const
{
  return _hovering;
}

void StructuralEntity::setHovering(bool hovering)
{
  _hovering = hovering;
}

bool StructuralEntity::isDragging() const
{
  return _dragging;
}

void StructuralEntity::setDragging(bool dragging)
{
  _dragging = dragging;
}

bool StructuralEntity::isSelected() const
{
  return _selected;
}

void StructuralEntity::setSelected(bool selected)
{
  _selected = selected;
}

bool StructuralEntity::isUncollapsed() const
{
  return _uncollapsed;
}

void StructuralEntity::setUncollapsed(bool uncollapsed)
{
  _uncollapsed = uncollapsed;
  _properties[STR_PROPERTY_ENTITY_UNCOLLAPSED] = (uncollapsed ? STR_VALUE_TRUE : STR_VALUE_FALSE);
}

bool StructuralEntity::isReference() const
{
  return _reference;
}

void StructuralEntity::setReference(bool reference)
{
  _reference = reference;
  _properties[STR_PROPERTY_ENTITY_REFERENCE] = (reference ? STR_VALUE_TRUE : STR_VALUE_FALSE);
}

qreal StructuralEntity::getTop() const
{
  return _top;
}

void StructuralEntity::setTop(qreal top)
{
  _top = top;
  _properties[STR_PROPERTY_ENTITY_TOP] = QString::number(top);

  setY(top-4);
}

qreal StructuralEntity::getMoveTop() const
{
  return _moveTop;
}

void StructuralEntity::setMoveTop(qreal moveTop)
{
  _moveTop = moveTop;
}

qreal StructuralEntity::getPressTop() const
{
  return _pressTop;
}

void StructuralEntity::setPressTop(qreal pressTop)
{
  _pressTop = pressTop;
}

qreal StructuralEntity::getResizeTop() const
{
  return _resizeTop;
}

void StructuralEntity::setResizeTop(qreal resizeTop)
{
  _resizeTop = resizeTop;
}

qreal StructuralEntity::getLeft() const
{
  return _left;
}

void StructuralEntity::setLeft(qreal left)
{
  _left = left;
  _properties[STR_PROPERTY_ENTITY_LEFT] = QString::number(left);

  setX(left-4);
}

qreal StructuralEntity::getMoveLeft() const
{
  return _moveLeft;
}

void StructuralEntity::setMoveLeft(qreal moveLeft)
{
  _moveLeft = moveLeft;
}

qreal StructuralEntity::getPressLeft() const
{
  return _pressLeft;
}

void StructuralEntity::setPressLeft(qreal pressLeft)
{
  _pressLeft = pressLeft;
}

qreal StructuralEntity::getResizeLeft() const
{
  return _resizeLeft;
}

void StructuralEntity::setResizeLeft(qreal resizeLeft)
{
  _resizeLeft = resizeLeft;
}

qreal StructuralEntity::getWidth() const
{
  return _width;
}

void StructuralEntity::setWidth(qreal width)
{
  _width = width;
  _properties[STR_PROPERTY_ENTITY_WIDTH] = QString::number(width);
}

qreal StructuralEntity::getPressWidth() const
{
  return _pressWidth;
}

void StructuralEntity::setPressWidth(qreal pressWidth)
{
  _pressWidth = pressWidth;
}

qreal StructuralEntity::getResizeWidth() const
{
  return _resizeWidth;
}

void StructuralEntity::setResizeWidth(qreal resizeWidth)
{
  _resizeWidth = resizeWidth;
}

qreal StructuralEntity::getHeight() const
{
  return _height;
}

void StructuralEntity::setHeight(qreal height)
{
  _height = height;
  _properties[STR_PROPERTY_ENTITY_HEIGHT] = QString::number(height);
}

qreal StructuralEntity::getPressHeight() const
{
  return _pressHeight;
}

void StructuralEntity::setPressHeight(qreal pressHeight)
{
  _pressHeight = pressHeight;
}

qreal StructuralEntity::getResizeHeight() const
{
  return _resizeHeight;
}

void StructuralEntity::setResizeHeight(qreal resizeHeight)
{
  _resizeHeight = resizeHeight;
}

qreal StructuralEntity::getUncollapedTop() const
{
  return _uncollapsedTop;
}

void StructuralEntity::setUncollapedTop(qreal uncollapedTop)
{
  _uncollapsedTop = uncollapedTop;
  _properties[STR_PROPERTY_ENTITY_UNCOLLAPSED_TOP] = QString::number(uncollapedTop);
}

qreal StructuralEntity::getUncollapedLeft() const
{
  return _uncollapsedLeft;
}

void StructuralEntity::setUncollapedLeft(qreal uncollapedLeft)
{
  _uncollapsedLeft = uncollapedLeft;
  _properties[STR_PROPERTY_ENTITY_UNCOLLAPSED_LEFT] = QString::number(uncollapedLeft);
}

qreal StructuralEntity::getUncollapedWidth() const
{
  return _uncollapsedWidth;
}

void StructuralEntity::setUncollapedWidth(qreal uncollapedWidth)
{
  _uncollapsedWidth = uncollapedWidth;
  _properties[STR_PROPERTY_ENTITY_UNCOLLAPSED_WIDTH] = QString::number(uncollapedWidth);
}

qreal StructuralEntity::getUncollapedHeight() const
{
  return _uncollapsedHeight;
}

void StructuralEntity::setUncollapedHeight(qreal uncollapedHeight)
{
  _uncollapsedHeight = uncollapedHeight;
  _properties[STR_PROPERTY_ENTITY_UNCOLLAPSED_HEIGHT] = QString::number(uncollapedHeight);
}

qreal StructuralEntity::getzIndex() const
{
  return _zindex;
}

void StructuralEntity::setzIndex(qreal zIndex)
{
  _zindex = zIndex;
  _properties[STR_PROPERTY_ENTITY_ZINDEX] = QString::number(zIndex);

  setZValue(zIndex);
}

StructuralMenu* StructuralEntity::getMenu() const
{
  return _menu;
}

void StructuralEntity::setMenu(StructuralMenu* menu)
{
  _menu = menu;
}

StructuralEntity* StructuralEntity::getStructuralParent() const
{
  return _parent;
}

QString StructuralEntity::getError() const
{
  return _error;
}

void StructuralEntity::setError(const QString &error)
{
  _error = error;
}

QString StructuralEntity::getWarning() const
{
  return _warnning;
}

void StructuralEntity::setWarning(const QString &warning)
{
  _warnning = warning;
}

QString StructuralEntity::getInfo() const
{
  return _info;
}

void StructuralEntity::setInfo(const QString &info)
{
  _info = info;
}

void StructuralEntity::setStructuralParent(StructuralEntity* parent)
{
  _parent = parent;

  setParent(parent);
  setParentItem(parent);

  if (parent != NULL)
    parent->addStructuralEntity(this);
}

QVector<StructuralEntity*> StructuralEntity::getStructuralEntities() const
{
  return _children;
}

void StructuralEntity::addStructuralEntity(StructuralEntity* entity)
{
  if (entity != NULL)
    if(!_children.contains(entity))
    {
      if (entity->getStructuralType() == Structural::Link)
        entity->setzIndex(_zindex+1);
      else if (entity->getStructuralType() == Structural::Context ||
               entity->getStructuralType() == Structural::Switch)
      {
        entity->setzIndex(_zindex+2);;
      }
      else
      {
        entity->setzIndex(_zindex);
      }

      _children.append(entity);
    }
}

void StructuralEntity::removeStructuralEntity(StructuralEntity* entity)
{
  if (entity != NULL)
  {
    int index = _children.indexOf(entity);

    if (index >= 0)
      _children.remove(index);

    entity->setStructuralParent(NULL);
  }
}

QVector<QString> StructuralEntity::getStructuralRestrictions() const
{
  return _restrictions;
}

void StructuralEntity::addStructuralRestriction(const QString &restriction)
{
  if(!_restrictions.contains(restriction))
    _restrictions.append(restriction);
}

void StructuralEntity::removeStructuralRestriction(const QString &restriction)
{

}

void StructuralEntity::delineate(QPainterPath* painter) const
{
  painter->addRect(STR_DEFAULT_ENTITY_PADDING,
                   STR_DEFAULT_ENTITY_PADDING,
                   getWidth(),
                   getHeight());
}

void StructuralEntity::move(QGraphicsSceneMouseEvent* event)
{
  // setting
  qreal x = getLeft();
  qreal y = getTop();

  StructuralEntity* parent = getStructuralParent();

  // setting minimal position
  qreal minx;
  qreal miny;

  if (parent != NULL)
  {
    minx = STR_DEFAULT_ENTITY_PADDING;
    miny = STR_DEFAULT_ENTITY_PADDING;
  }
  else
  {
    minx = 0;
    miny = 0;
  }

  // setting maximal position
  qreal maxx;
  qreal maxy;

  if (parent != NULL)
  {
    maxx = parent->getWidth() - getWidth() - STR_DEFAULT_ENTITY_PADDING;
    maxy = parent->getHeight() - getHeight() - STR_DEFAULT_ENTITY_PADDING;
  }
  else
  {
    maxx = scene()->width() - getWidth();
    maxy = scene()->height() - getHeight();
  }

  // setting delta
  qreal dx = event->pos().x() - getPressLeft(); // (x1 - x0)
  qreal dy = event->pos().y() - getPressTop();  // (y1 - y0)

  // setting next position
  qreal nextx = x + dx;
  qreal nexty = y + dy;

  // moving
  setMoveTop(nexty);
  setMoveLeft(nextx);

  if (scene() != NULL)
    scene()->update();
}

void StructuralEntity::resize(QGraphicsSceneMouseEvent* event)
{
  // setting
  qreal x = getLeft();
  qreal y = getTop();
  qreal w = getWidth();
  qreal h = getHeight();

  StructuralEntity* parent = getStructuralParent();

  // setting minimal bounds
  qreal minx;
  qreal miny;
  qreal minw;
  qreal minh;

  if (parentItem() != NULL)
  {
    minx = STR_DEFAULT_ENTITY_PADDING;
    miny = STR_DEFAULT_ENTITY_PADDING;
    minw = -1; // not used
    minh = -1; // not used
  }
  else
  {
    minx = 0;
    miny = 0;
    minw = -1; // not used
    minh = -1; // not used
  }

  // setting maximal bounds
  qreal maxx;
  qreal maxy;
  qreal maxw;
  qreal maxh;

  if (parentItem() != NULL)
  {
    maxx = parent->getWidth() - getWidth() - STR_DEFAULT_ENTITY_PADDING;
    maxy = parent->getHeight() - getHeight() - STR_DEFAULT_ENTITY_PADDING;
    maxw = parent->getWidth() - STR_DEFAULT_ENTITY_PADDING;
    maxh = parent->getHeight() - STR_DEFAULT_ENTITY_PADDING;
  }
  else
  {
    maxx = scene()->width() - getWidth();
    maxy = scene()->height() - getHeight();
    maxw = scene()->width();
    maxh = scene()->height();
  }

  // setting delta
  qreal dx = event->pos().x() - getPressLeft();    // (x1 - x0)
  qreal dy = event->pos().y() - getPressTop();     // (y1 - y0)
  qreal dw = -dx;
  qreal dh = -dy;

  // setting next bounds
  qreal nextx = x + dx;
  qreal nexty = y + dy;
  qreal nextw = w + dw;
  qreal nexth = h + dh;

  // adjusting
  switch(getStructuralResize())
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
  }

  // resizing
  setResizeTop(nexty);
  setResizeLeft(nextx);
  setResizeWidth(nextw);
  setResizeHeight(nexth);

  if (scene() != NULL)
    scene()->update();
}

QPainterPath StructuralEntity::shape() const
{
  QPainterPath painter;

  delineate(&painter);

  if (_selectable && _selected && _resizable)
  {
    painter.setFillRule(Qt::WindingFill);

    qreal w = STR_DEFAULT_ENTITY_ANCHOR_W;
    qreal h = STR_DEFAULT_ENTITY_ANCHOR_H;

    painter.addRect(0,0,w,h);                                // topleft
    painter.addRect((_width+w)/2 - w/2,0,w,h);               // top
    painter.addRect((_width+w) - w,0,w,h);                   // topright
    painter.addRect((_width+w) - w,(_height+h)/2 - h/2,w,h); // right
    painter.addRect((_width+w) - w,(_height+h) - h,w,h);     // bottomright
    painter.addRect((_width+w)/2 - w/2,(_height+h) - h,w,h); // bottom
    painter.addRect(0,(_height+h) - h,w,h);                  // bottomleft
    painter.addRect(0,(_height+h)/2 - h/2,w,h);              // left
  }

  return painter;
}

QRectF StructuralEntity::boundingRect() const
{
  QRectF bounds;

  bounds.setX(0);
  bounds.setY(0);
  bounds.setWidth(_width+STR_DEFAULT_ENTITY_ANCHOR_W);
  bounds.setHeight(_height+STR_DEFAULT_ENTITY_ANCHOR_H);

  return bounds;
}

void StructuralEntity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  if (_reference)
    painter->setOpacity(0.5);

  draw(painter);

  if (_selectable && _selected)
  {
    painter->setRenderHint(QPainter::Antialiasing,false);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0, Qt::DashLine));

    painter->drawRect(STR_DEFAULT_ENTITY_ANCHOR_W/2, STR_DEFAULT_ENTITY_ANCHOR_H/2, getWidth(), getHeight());

    if (_resizable)
    {
      painter->setBrush(QBrush(Qt::white));
      painter->setPen(QPen(QBrush(Qt::black), 0));

      qreal w = STR_DEFAULT_ENTITY_ANCHOR_W;
      qreal h = STR_DEFAULT_ENTITY_ANCHOR_H;

      painter->drawRect(0,0,w,h);                                // topleft
      painter->drawRect((_width+w)/2 - w/2,0,w,h);               // top
      painter->drawRect((_width+w) - w,0,w,h);                   // topright
      painter->drawRect((_width+w) - w,(_height+h)/2 - h/2,w,h); // right
      painter->drawRect((_width+w) - w,(_height+h) - h,w,h);     // bottomright
      painter->drawRect((_width+w)/2 - w/2,(_height+h) - h,w,h); // bottom
      painter->drawRect(0,(_height+h) - h,w,h);                  // bottomleft
      painter->drawRect(0,(_height+h)/2 - h/2,w,h);              // left
    }
  }
  else if(_hoverable && _hovering)
  {
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(QColor("#999999")), 0, Qt::DashLine));

    painter->drawRect(STR_DEFAULT_ENTITY_PADDING, STR_DEFAULT_ENTITY_PADDING, getWidth(), getHeight());
  }
}

void StructuralEntity::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  if (_moveable && _moving)
  {
    move(event);
  }
  else if (_resizable && _resizing)
  {
    resize(event);
  }
  else if(_draggable && _dragging)
  {
    QDrag *drag = new QDrag(event->widget());
    QMimeData* minedata = new QMimeData();

    minedata->setObjectName(StructuralUtil::translateTypeToString(_type));
    minedata->setText(_uid);

    drag->setMimeData(minedata);
    drag->exec();
  }
}

void StructuralEntity::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  if (event->button() == Qt::RightButton)
  {
    event->ignore();
  }
  else if (event->button() == Qt::LeftButton)
  {
    if (_selectable && !_selected)
    {
      setSelected(true);
      emit selected(getStructuralUid(), StructuralUtil::createSettings());
    }

    setPressTop(event->pos().y());
    setPressLeft(event->pos().x());
    setPressWidth(_width);
    setPressHeight(_height);

    qreal w = STR_DEFAULT_ENTITY_ANCHOR_W;
    qreal h = STR_DEFAULT_ENTITY_ANCHOR_H;

    if (_draggable)
    {
      setDragging(true);
    }
    else if (_resizable)
    {
      setResizeTop(_top);
      setResizeLeft(_left);
      setResizeWidth(_width);
      setResizeHeight(_height);

      if (QRectF(0,0,w,h).contains(event->pos()))
      {
        // if over TOPLEFT resize region
        setStructuralResize(Structural::TopLeft);
        setResizing(true);      
      }
      else if (QRectF((_width+w)/2 - w/2,0,w,h).contains(event->pos()))
      {
        // if over TOP resize region
        setStructuralResize(Structural::Top);
        setResizing(true);
      }
      else if (QRectF((_width+w) - w,0,w,h).contains(event->pos()))
      {
        // if over TOPRIGHT resize region
        setStructuralResize(Structural::TopRight);
        setResizing(true);      
      }
      else if (QRectF((_width+w) - w,(_height+h)/2 - h/2,w,h).contains(event->pos()))
      {
        // if over RIGHT resize region
        setStructuralResize(Structural::Right);
        setResizing(true);

      }
      else if (QRectF((_width+w) - w,(_height+h) - h,w,h).contains(event->pos()))
      {
        // if over BOTTOMRIGHT resize region
        setStructuralResize(Structural::BottomRight);
        setResizing(true);
      }
      else if (QRectF((_width+w)/2 - w/2,(_height+h) - h,w,h).contains(event->pos()))
      {
        // if over BOTTOM resize region
        setStructuralResize(Structural::Bottom);
        setResizing(true);
      }
      else if (QRectF(0,(_height+h) - h,w,h).contains(event->pos()))
      {
        // if over BOTTOMLEFT resize region
        setStructuralResize(Structural::BottomLeft);
        setResizing(true);
      }
      else if (QRectF(0,(_height+h)/2 - h/2,w,h).contains(event->pos()))
      {
        // if over LEFT resize region
        setStructuralResize(Structural::Left);
        setResizing(true);
      }
      else if (_moveable)
      {
        // if not over any resize region
        setMoveTop(_top);
        setMoveLeft(_left);

        setMoving(true);
      }
    }
    else if (_moveable)
    {
      setMoveTop(_top);
      setMoveLeft(_left);

      setMoving(true);
    }
  }

  QGraphicsItem::mousePressEvent(event);
}

void StructuralEntity::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
{
  if (_moving)
  {
    setMoving(false);

    if ((_top != _moveTop || _left != _moveLeft))
    {
      QMap<QString, QString> previous = getStructuralProperties();

      setTop(_moveTop);
      setLeft(_moveLeft);

      emit changed(getStructuralUid(), getStructuralProperties(), previous, StructuralUtil::createSettings());
    }
  }
  else if (_resizing)
  {
    setResizing(false);

    QMap<QString, QString> previous = getStructuralProperties();

    if ((_top != _resizeTop || _left != _resizeLeft || _width != _resizeWidth || _height != _resizeHeight))
    {
      if (_resizeTop > _top + _height)
      {
        setResizeHeight(_resizeTop - (_top + _height));
        setResizeTop(_top + _height);
      }

      if (_resizeLeft > _left + _width)
      {
        setResizeWidth(_resizeLeft - (_left + _width));
        setResizeLeft(_left + _width);
      }

      if (_resizeWidth < 0)
      {
        setResizeLeft(_resizeLeft + _resizeWidth);
        setResizeWidth(-_resizeWidth);
      }

      if (_resizeHeight < 0)
      {
        setResizeTop(_resizeTop + _resizeHeight);
        setResizeHeight(-_resizeHeight);
      }

      foreach(StructuralEntity* entity, _children)
      {
        entity->setTop(entity->getTop() - (_resizeTop - _top));
        entity->setLeft(entity->getLeft() - (_resizeLeft - _left));
      }

      setTop(_resizeTop);
      setLeft(_resizeLeft);
      setWidth(_resizeWidth);
      setHeight(_resizeHeight);

      emit changed(getStructuralUid(), getStructuralProperties(), previous, StructuralUtil::createSettings());
    }
  }

  update();

  QGraphicsItem::mouseReleaseEvent(event);
}

void StructuralEntity::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
  QGraphicsItem::contextMenuEvent(event);

  if (!event->isAccepted())
  {
    if (!isSelected())
    {
      setSelected(true);
      emit selected(getStructuralUid(), StructuralUtil::createSettings());
    }

    if (_menu != NULL)
    {
      _menu->setInsertTop(event->pos().y());
      _menu->setInsertLeft(event->pos().x());
      _menu->adjust(_type);

      if (!STR_DEFAULT_WITH_BODY &&
          !STR_DEFAULT_WITH_FLOATING_INTERFACES)
      {
        if (getStructuralParent() == NULL)
        {
          _menu->switchAutostart(true);

          if (getStructuralProperty(STR_PROPERTY_ENTITY_AUTOSTART) == STR_VALUE_TRUE)
            _menu->switchAutostartProperty(true);
        }
        else if (getStructuralParent()->getStructuralParent() == NULL)
        {
          if (getStructuralCategory() == Structural::Interface)
          {
            _menu->switchAutostart(true);

            if (getStructuralProperty(STR_PROPERTY_ENTITY_AUTOSTART) == STR_VALUE_TRUE)
              _menu->switchAutostartProperty(true);
          }
        }
      }

      _menu->exec(event->screenPos());
    }

    event->accept();
  }
}

/*
 *
*/

void StructuralEntity::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
  if (_selected)
  {
    if (_resizable)
    {
      qreal w = STR_DEFAULT_ENTITY_ANCHOR_W;
      qreal h = STR_DEFAULT_ENTITY_ANCHOR_H;


      if (QRectF(0,0,w,h).contains(event->pos()))
      {
        // if over TOPLEFT resize region
        setCursor(Qt::SizeFDiagCursor);
      }
      else if (QRectF((_width+w)/2 - w/2,0,w,h).contains(event->pos()))
      {
        // if over TOP resize region
        setCursor(Qt::SizeVerCursor);
      }
      else if (QRectF((_width+w) - w,0,w,h).contains(event->pos()))
      {
        // if over TOPRIGHT resize region
        setCursor(Qt::SizeBDiagCursor);
      }
      else if (QRectF((_width+w) - w,(_height+h)/2 - h/2,w,h).contains(event->pos()))
      {
        // if over RIGHT resize region
        setCursor(Qt::SizeHorCursor);
      }
      else if (QRectF((_width+w) - w,(_height+h) - h,w,h).contains(event->pos()))
      {
        // if over BOTTOMRIGHT resize region
        setCursor(Qt::SizeFDiagCursor);
      }
      else if (QRectF((_width+w)/2 - w/2,(_height+h) - h,w,h).contains(event->pos()))
      {
        // if over BOTTOM resize region
        setCursor(Qt::SizeVerCursor);
      }
      else if (QRectF(0,(_height+h) - h,w,h).contains(event->pos()))
      {
        // if over BOTTOMLEFT resize region
        setCursor(Qt::SizeBDiagCursor);
      }
      else if (QRectF(0,(_height+h)/2 - h/2,w,h).contains(event->pos()))
      {
        // if over LEFT resize region
        setCursor(Qt::SizeHorCursor);
      }
      else if (_moveable)
      {
        // if not over any resize region
        setCursor(Qt::SizeAllCursor);
      }
    }
    else if (_moveable)
    {
      setCursor(Qt::SizeAllCursor);
    }
    else
    {
      setCursor(Qt::ArrowCursor);
    }
  }
  else
  {
    setCursor(Qt::ArrowCursor);
  }
}

void StructuralEntity::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
  QGraphicsItem::hoverEnterEvent(event);

  setHovering(true);
}

void StructuralEntity::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
  QGraphicsItem::hoverLeaveEvent(event);

  setHovering(false);
}

void StructuralEntity::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
  StructuralType type = StructuralUtil::translateStringToType(event->mimeData()->objectName());

  if (StructuralUtil::validateKinship(type, _type) &&
      event->mimeData()->text() != _uid)
    event->setAccepted(true);
  else
    event->setAccepted(false);
}

void StructuralEntity::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
  StructuralType type = StructuralUtil::translateStringToType(event->mimeData()->objectName());

  if (StructuralUtil::validateKinship(type, _type) &&
      event->mimeData()->text() != _uid)
    event->setAccepted(true);
  else
    event->setAccepted(false);
}

void StructuralEntity::dropEvent(QGraphicsSceneDragDropEvent* event)
{
  StructuralType type = StructuralUtil::translateStringToType(event->mimeData()->objectName());

  if (StructuralUtil::validateKinship(type, _type) &&
      event->mimeData()->text() != _uid)
    emit move(event->mimeData()->text(), _uid);
}
