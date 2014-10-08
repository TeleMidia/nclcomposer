#include "StructuralEntity.h"
#include "StructuralMedia.h"
#include "StructuralComposition.h"

StructuralEntity::StructuralEntity(StructuralEntity* parent)
  : QObject(parent), QGraphicsItem(parent)
{
  setLocalUid((QString) QUuid::createUuid().toString());
  setLocalType(Structural::NoType);
  setLocalName(Structural::NoName);

  setLocalParent(parent);

  setMoveable(true);
  setResizable(true);
  setSelectable(true);
  setHoverable(true);
  setDraggable(true);

  setMoving(false);
  setHidden(false);
  setResizing(false);
  setSelected(false);
  setDragging(false);
  setHovering(false);

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

  setzIndex(0);

  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsFocusable, true);

  setAcceptHoverEvents(true);

  // ----------------------
  hover = false;
  menu = new StructuralMenu();
  connect(menu, SIGNAL(insert(Structural::EntityName)),SLOT(performInsert(Structural::EntityName)));

  menu->insertMenu->setEnabled(true);

  _draggable = false;
  hasError = false;
  enableMouseHoverHighlight = true;
  // ----------------------
}

StructuralEntity::~StructuralEntity()
{
  // TODO
}

QString StructuralEntity::getLocalUid() const
{
  return _uid;
}

void StructuralEntity::setLocalUid(const QString &uid)
{
  _uid = uid;

  _properties["LOCAL:UID"] = uid;
}

QString StructuralEntity::getLocalId() const
{
  return _id;
}

void StructuralEntity::setLocalId(const QString &id)
{
  _id = id;

  _properties["LOCAL:ID"] = id;

  // --------------------
  updateToolTip();
  // --------------------
}

LocalType StructuralEntity::getLocalType() const
{
  return _type;
}

void StructuralEntity::setLocalType(const LocalType type)
{
  _type = type;

  _properties["LOCAL:TYPE"] = QString::number(type);
}

LocalName StructuralEntity::getLocalName() const
{
  return _name;
}

void StructuralEntity::setLocalName(const LocalName subtype)
{
  _name = subtype;

  _properties["LOCAL:NAME"] = StructuralUtil::getStrFromNstType(subtype);
}

QMap<QString, QString> StructuralEntity::getLocalProperties() const
{
  return _properties;
}

void StructuralEntity::setLocalProperties(const QMap<QString, QString> &properties)
{
  // TODO

  foreach (QString name, properties.keys())
  {
    setLocalProperty(name, properties[name]);
  }
}

QString StructuralEntity::getLocalProperty(const QString &name)
{
  return _properties.value(name,"");
}

void StructuralEntity::setLocalProperty(const QString &name, const QString &value)
{
  // TODO

  _properties.insert(name, value);


  if (name == "LOCAL:TOP")
  {
    setTop(value.toDouble());
  }
  else if (name == "LOCAL:LEFT")
  {
    setLeft(value.toDouble());
  }
  else if (name == "LOCAL:WIDTH")
  {
    setWidth(value.toDouble());
  }
  else if (name == "LOCAL:HEIGHT")
  {
    setHeight(value.toDouble());
  }
  else if (name == "LOCAL:ZINDEX")
  {
    setzIndex(value.toInt());
  }
  else if (name == "LOCAL:ID")
  {
    setLocalId(value);
  }
  else if (name == "LOCAL:UID")
  {
    setLocalUid(value);
  }
  else if (name == "hidden")
  {
    if (value == "true")
      setVisible(false);
    else
      setVisible(true);
  }
}

bool StructuralEntity::isMoveable() const
{
  return _moveable;
}

void StructuralEntity::setMoveable(bool moveable)
{
  _moveable = moveable;

  _properties["LOCAL:MOVEABLE"] = (moveable ? "TRUE" : "FALSE");
}

bool StructuralEntity::isSelectable() const
{
  return _selectable;
}

void StructuralEntity::setSelectable(bool selectable)
{
  _selectable = selectable;

  _properties["LOCAL:SELECTABLE"] = (selectable ? "TRUE" : "FALSE");
}

bool StructuralEntity::isHoverable() const
{
  return _hoverable;
}

void StructuralEntity::setHoverable(bool hoverable)
{
  _hoverable = hoverable;

  _properties["LOCAL:HOVERABLE"] = (hoverable ? "TRUE" : "FALSE");
}

bool StructuralEntity::isResizable() const
{
  return _resizable;
}

void StructuralEntity::setResizable(bool resizable)
{
  _resizable = resizable;

  _properties["LOCAL:RESIZABLE"] = (resizable ? "TRUE" : "FALSE");
}

bool StructuralEntity::isDraggable()
{
  return _draggable;
}

void StructuralEntity::setDraggable(bool draggable)
{
  _draggable = draggable;

  _properties["LOCAL:DRAGGABLE"] = (draggable ? "TRUE" : "FALSE");
}

bool StructuralEntity::isMoving() const
{
  return _moving;
}

void StructuralEntity::setMoving(bool moving)
{
  _moving = moving;

  _properties["LOCAL:MOVING"] = (moving ? "TRUE" : "FALSE");
}

bool StructuralEntity::isHidden() const
{
  return _hidden;
}

void StructuralEntity::setHidden(bool hidden)
{
  // TODO
  _hidden = hidden;

  _properties["LOCAL:HIDDEN"] = (hidden ? "TRUE" : "FALSE");

  setVisible(!hidden);
}

bool StructuralEntity::isResizing() const
{
  return _resizing;
}

void StructuralEntity::setResizing(bool resizing)
{
  _resizing = resizing;

  _properties["LOCAL:RESIZING"] = (resizing ? "TRUE" : "FALSE");
}

bool StructuralEntity::isHovering() const
{
  return _hovering;
}

void StructuralEntity::setHovering(bool hovering)
{
  _hovering = hovering;

  _properties["LOCAL:HOVERING"] = (hovering ? "TRUE" : "FALSE");
}

bool StructuralEntity::isDragging() const
{
  return _dragging;
}

void StructuralEntity::setDragging(bool dragging)
{
  _dragging = dragging;

  _properties["LOCAL:DRAGGING"] = (dragging ? "TRUE" : "FALSE");
}

bool StructuralEntity::isSelected() const
{
  return _selected;
}

void StructuralEntity::setSelected(bool selected)
{
  _selected = selected;

  _properties["LOCAL:SELECTED"] = (selected ? "TRUE" : "FALSE");
}

qreal StructuralEntity::getTop() const
{
  return _top;
}

void StructuralEntity::setTop(qreal top)
{
  _top = top;

  _properties["LOCAL:TOP"] = QString::number(top);

  // TODO
  setY(top-4);
}

qreal StructuralEntity::getMoveTop() const
{
  return _moveTop;
}

void StructuralEntity::setMoveTop(qreal moveTop)
{
  _moveTop = moveTop;

  _properties["LOCAL:MOVETOP"] = QString::number(moveTop);
}

qreal StructuralEntity::getPressTop() const
{
  return _pressTop;
}

void StructuralEntity::setPressTop(qreal pressTop)
{
  _pressTop = pressTop;

  _properties["LOCAL:PRESSTOP"] = QString::number(pressTop);
}

qreal StructuralEntity::getResizeTop() const
{
  return _resizeTop;
}

void StructuralEntity::setResizeTop(qreal resizeTop)
{
  _resizeTop = resizeTop;

  _properties["LOCAL:RESIZETOP"] = QString::number(resizeTop);
}

qreal StructuralEntity::getLeft() const
{
  return _left;
}

void StructuralEntity::setLeft(qreal left)
{
  _left = left;

  _properties["LOCAL:LEFT"] = QString::number(left);

  // TODO
  setX(left-4);
}

qreal StructuralEntity::getMoveLeft() const
{
  return _moveLeft;
}

void StructuralEntity::setMoveLeft(qreal moveLeft)
{
  _moveLeft = moveLeft;

  _properties["LOCAL:MOVELEFT"] = QString::number(moveLeft);
}

qreal StructuralEntity::getPressLeft() const
{
  return _pressLeft;
}

void StructuralEntity::setPressLeft(qreal pressLeft)
{
  _pressLeft = pressLeft;

  _properties["LOCAL:PRESSLEFT"] = QString::number(pressLeft);
}

qreal StructuralEntity::getResizeLeft() const
{
  return _resizeLeft;
}

void StructuralEntity::setResizeLeft(qreal resizeLeft)
{
  _resizeLeft = resizeLeft;

  _properties["LOCAL:RESIZELEFT"] = QString::number(resizeLeft);
}

qreal StructuralEntity::getWidth() const
{
  return _width;
}

void StructuralEntity::setWidth(qreal width)
{
  _width = width;

  _properties["LOCAL:WIDTH"] = QString::number(width);
}

qreal StructuralEntity::getPressWidth() const
{
  return _pressWidth;
}

void StructuralEntity::setPressWidth(qreal pressWidth)
{
  _pressWidth = pressWidth;

  _properties["LOCAL:PRESSWIDTH"] = QString::number(pressWidth);
}

qreal StructuralEntity::getResizeWidth() const
{
  return _resizeWidth;
}

void StructuralEntity::setResizeWidth(qreal resizeWidth)
{
  _resizeWidth = resizeWidth;

  _properties["LOCAL:RESIZEWIDTH"] = QString::number(resizeWidth);
}

qreal StructuralEntity::getHeight() const
{
  return _height;
}

void StructuralEntity::setHeight(qreal height)
{
  _height = height;

  _properties["LOCAL:HEIGHT"] = QString::number(height);
}

qreal StructuralEntity::getPressHeight() const
{
  return _pressHeight;
}

void StructuralEntity::setPressHeight(qreal pressHeight)
{
  _pressHeight = pressHeight;

  _properties["LOCAL:PRESSHEIGHT"] = QString::number(pressHeight);
}

qreal StructuralEntity::getResizeHeight() const
{
  return _resizeHeight;
}

void StructuralEntity::setResizeHeight(qreal resizeHeight)
{
  _resizeHeight = resizeHeight;

  _properties["LOCAL:RESIZEHEIGHT"] = QString::number(resizeHeight);
}

qreal StructuralEntity::getzIndex() const
{
  return _zIndex;
}

void StructuralEntity::setzIndex(qreal zIndex)
{
  // TODO
  _zIndex = zIndex;

  _properties["LOCAL:ZINDEX"] = QString::number(zIndex);

  setZValue(zIndex);
}

StructuralEntity* StructuralEntity::getLocalParent() const
{
  return _parent;
}

void StructuralEntity::setLocalParent(StructuralEntity* parent)
{
  // TODO
  _parent = parent;

  setParent(parent);
  setParentItem(parent);

  if (parent !=NULL)
    parent->insertLocalChild(this);
}

QVector<StructuralEntity*> StructuralEntity::getLocalChildren()
{
  return _children;
}

void StructuralEntity::insertLocalChild(StructuralEntity* child)
{
  if (child != NULL)
  {
  // \fixme This is only an additional check. Maybe, the better way to do
  // that could be using a set instead of a vector to entities.
  if(!_children.contains(child))
  {
    _children.append(child);

    // connect(entity, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
    // connect(entity, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

  }
  else
  {
    qWarning() << "[QNST] Warning! You are adding the same entity twice as \
                  child of " << QString().sprintf("%p", this) << __FILE__ << __LINE__;
  }
  }
  }

void StructuralEntity::removeLocalChild(StructuralEntity* child)
{
  if (child != NULL)
  {
    int index = _children.indexOf(child);

    if (index >= 0)
    _children.remove(index);

    child->setLocalParent(NULL);
  }
}

void StructuralEntity::newChild(Structural::EntityName type)
{
  QString uid = QUuid::createUuid().toString();
  QString parent = getLocalUid();
  QMap<QString, QString> properties;
  properties["LOCAL:NAME"] = QString::number(type);

  QMap<QString, QString> settings;
  settings["UNDO"] = "1";
  settings["NOTIFY"] = "1";
  settings["CODE"] = QUuid::createUuid().toString();

  emit inserted(uid, parent, properties, settings);
}

LocalResize StructuralEntity::getLocalResize() const
{
  return _resize;
}

void StructuralEntity::setLocalResize(const LocalResize resize)
{
  _resize = resize;
}

QPainterPath StructuralEntity::shape() const
{
  QPainterPath painter;

  delineate(&painter);

  if (_selectable && _selected && _resizable){
    painter.setFillRule(Qt::WindingFill);

    qreal W = DEFAULT_ANCHOR_WIDTH;
    qreal H = DEFAULT_ANCHOR_HEIGHT;

    painter.addRect(0,0,W,H);                                // topleft
    painter.addRect((_width+W)/2 - W/2,0,W,H);               // top
    painter.addRect((_width+W) - W,0,W,H);                   // topright
    painter.addRect((_width+W) - W,(_height+H)/2 - H/2,W,H); // right
    painter.addRect((_width+W) - W,(_height+H) - H,W,H);     // bottomright
    painter.addRect((_width+W)/2 - W/2,(_height+H) - H,W,H); // bottom
    painter.addRect(0,(_height+H) - H,W,H);                  // bottomleft
    painter.addRect(0,(_height+H)/2 - H/2,W,H);              // left
  }

  return painter;
}

QRectF StructuralEntity::boundingRect() const
{
  QRectF bounds;

  bounds.setX(0);
  bounds.setY(0);
  bounds.setWidth(_width+DEFAULT_ANCHOR_WIDTH);
  bounds.setHeight(_height+DEFAULT_ANCHOR_HEIGHT);

  return bounds;
}

void StructuralEntity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  draw(painter);

  if (_selectable && _selected){
    painter->setRenderHint(QPainter::Antialiasing,false);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black), 0, Qt::DashLine));

    painter->drawRect(DEFAULT_ANCHOR_WIDTH/2, DEFAULT_ANCHOR_HEIGHT/2, getWidth()-1, getHeight()-1);

    if (_resizable){
      painter->setBrush(QBrush(Qt::white));
      painter->setPen(QPen(QBrush(Qt::black), 0));

      qreal W = DEFAULT_ANCHOR_WIDTH;
      qreal H = DEFAULT_ANCHOR_HEIGHT;

      painter->drawRect(0,0,W,H);                                // topleft
      painter->drawRect((_width+W)/2-H/2-1,0,W,H);               // top
      painter->drawRect((_width+W)-W-1,0,W,H);                   // topright
      painter->drawRect((_width+W)-W-1,(_height+H)/2-H/2-1,W,H); // right
      painter->drawRect((_width+W)-W-1,(_height+H)-H-1,W,H);     // bottomright
      painter->drawRect((_width+W)/2-W/2-1,(_height+H)-H-1,W,H); // bottom
      painter->drawRect(0,(_height+H)-H-1,W,H);                  // bottomleft
      painter->drawRect(0,(_height+H)/2-H/2-1,W,H);              // left
    }
  }
}

void StructuralEntity::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  if (_moveable && _moving){
    move(event);

  } else if (_resizable && _resizing){
    resize(event);
  }
}

void StructuralEntity::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  if (event->button() == Qt::RightButton){
    event->ignore();

  }else if (event->button() == Qt::LeftButton){
    if (_selectable && !_selected){
      setSelected(true); QMap<QString, QString> settings; emit selected(getLocalUid(), settings);
    }

    setPressTop(event->pos().y());
    setPressLeft(event->pos().x());
    setPressWidth(_width);
    setPressHeight(_height);

    // avoid flickering on first move
    if (_moveable){
      setMoveTop(_top);
      setMoveLeft(_left);
    }

    // avoid flickering on first resize
    if (_resizable){
      setResizeTop(_top);
      setResizeLeft(_left);
      setResizeWidth(_width);
      setResizeHeight(_height);
    }

    qreal W = DEFAULT_ANCHOR_WIDTH;
    qreal H = DEFAULT_ANCHOR_HEIGHT;

    if (_resizable){
      // if over TOPLEFT resize region
      if (QRectF(0,0,W,H).contains(event->pos())){
        setLocalResize(Structural::TopLeft);
        setResizing(true);

        // if over TOP resize region
      }else if (QRectF((_width+W)/2 - W/2,0,W,H).contains(event->pos())){
        setLocalResize(Structural::Top);
        setResizing(true);

        // if over TOPRIGHT resize region
      }else if (QRectF((_width+W) - W,0,W,H).contains(event->pos())){
        setLocalResize(Structural::TopRight);
        setResizing(true);

        // if over RIGHT resize region
      }else if (QRectF((_width+W) - W,(_height+H)/2 - H/2,W,H).contains(event->pos())){
        setLocalResize(Structural::Right);
        setResizing(true);

        // if over BOTTOMRIGHT resize region
      }else if (QRectF((_width+W) - W,(_height+H) - H,W,H).contains(event->pos())){
        setLocalResize(Structural::BottomRight);
        setResizing(true);

        // if over BOTTOM resize region
      }else if (QRectF((_width+W)/2 - W/2,(_height+H) - H,W,H).contains(event->pos())){
        setLocalResize(Structural::Bottom);
        setResizing(true);

        // if over BOTTOMLEFT resize region
      }else if (QRectF(0,(_height+H) - H,W,H).contains(event->pos())){
        setLocalResize(Structural::BottomLeft);
        setResizing(true);

        // if over LEFT resize region
      }else if (QRectF(0,(_height+H)/2 - H/2,W,H).contains(event->pos())){
        setLocalResize(Structural::Left);
        setResizing(true);

        // if not over any resize region
      }else if (_moveable){
        setMoving(true);
      }

    }else if (_moveable){
      setMoving(true);
    }
  }

  QGraphicsItem::mousePressEvent(event);
}

void StructuralEntity::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
{
  if (_moving){
    setMoving(false);

    if ((_top != _moveTop || _left != _moveLeft)){

      /**********************/
      QMap<QString, QString> previous = getLocalProperties();

      //            emit entityAboutToChange(properties);

      setTop(_moveTop);
      setLeft(_moveLeft);

      adjust();

      QMap<QString, QString> properties = getLocalProperties();
      //            properties["top"] = QString::number(moveTop);
      //            properties["left"] = QString::number(moveLeft);

      QMap<QString, QString> settings;
      settings["UNDO"] = "1";
      settings["ADJUST"] = "0";
      settings["CODE"] = QUuid::createUuid().toString();

      emit changed(getLocalUid(), properties, previous,settings);


      //            emit entityChanged();


      /**************/

    }else{
      adjust();
    }

  }else if (_resizing){
    setResizing(false);

    QMap<QString, QString> previous = getLocalProperties();

    if ((_top != _resizeTop || _left != _resizeLeft || _width != _resizeWidth || _height != _resizeHeight)){
      if (_resizeTop > _top + _height){
        setResizeHeight(_resizeTop - (_top + _height));
        setResizeTop(_top + _height);
      }

      if (_resizeLeft > _left + _width){
        setResizeWidth(_resizeLeft - (_left + _width));
        setResizeLeft(_left + _width);
      }

      if (_resizeWidth < 0){
        setResizeLeft(_resizeLeft + _resizeWidth);
        setResizeWidth(-_resizeWidth);
      }

      if (_resizeHeight < 0){
        setResizeTop(_resizeTop + _resizeHeight);
        setResizeHeight(-_resizeHeight);
      }

      foreach(StructuralEntity* entity, _children){
        entity->setTop(entity->getTop() - (_resizeTop - _top));
        entity->setLeft(entity->getLeft() - (_resizeLeft - _left));
      }

      //            QMap<QString, QString> properties;

      //            properties["top"] = QString::number(moveTop);
      //            properties["left"] = QString::number(moveLeft);
      //            properties["width"] = QString::number(resizeWidth);
      //            properties["height"] = QString::number(resizeHeight);

      //            emit entityAboutToChange(properties);

      setTop(_resizeTop);
      setLeft(_resizeLeft);
      setWidth(_resizeWidth);
      setHeight(_resizeHeight);

      adjust();

      QMap<QString, QString> properties = getLocalProperties();

      //            emit entityChanged();

      QMap<QString, QString> settings;
      settings["UNDO"] = "1";
      settings["ADJUST"] = "0";
      settings["CODE"] = QUuid::createUuid().toString();

      emit changed(getLocalUid(), properties, previous,settings);

    }else{
      adjust();
    }
  }

  QGraphicsItem::mouseReleaseEvent(event);
}

void StructuralEntity::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
  if (_selected){
    if (_resizable){
      qreal W = DEFAULT_ANCHOR_WIDTH;
      qreal H = DEFAULT_ANCHOR_HEIGHT;

      // if over TOPLEFT resize region
      if (QRectF(0,0,W,H).contains(event->pos())){
        setCursor(Qt::SizeFDiagCursor);

        // if over TOP resize region
      }else if (QRectF((_width+W)/2 - W/2,0,W,H).contains(event->pos())){
        setCursor(Qt::SizeVerCursor);

        // if over TOPRIGHT resize region
      }else if (QRectF((_width+W) - W,0,W,H).contains(event->pos())){
        setCursor(Qt::SizeBDiagCursor);

        // if over RIGHT resize region
      }else if (QRectF((_width+W) - W,(_height+H)/2 - H/2,W,H).contains(event->pos())){
        setCursor(Qt::SizeHorCursor);

        // if over BOTTOMRIGHT resize region
      }else if (QRectF((_width+W) - W,(_height+H) - H,W,H).contains(event->pos())){
        setCursor(Qt::SizeFDiagCursor);

        // if over BOTTOM resize region
      }else if (QRectF((_width+W)/2 - W/2,(_height+H) - H,W,H).contains(event->pos())){
        setCursor(Qt::SizeVerCursor);

        // if over BOTTOMLEFT resize region
      }else if (QRectF(0,(_height+H) - H,W,H).contains(event->pos())){
        setCursor(Qt::SizeBDiagCursor);

        // if over LEFT resize region
      }else if (QRectF(0,(_height+H)/2 - H/2,W,H).contains(event->pos())){
        setCursor(Qt::SizeHorCursor);

        // if not over any resize region
      }else if (_moveable){
        setCursor(Qt::SizeAllCursor);
      }

    }else if (_moveable){
      setCursor(Qt::SizeAllCursor);

    }else{
      setCursor(Qt::ArrowCursor);
    }
  }else{
    setCursor(Qt::ArrowCursor);
  }

  QGraphicsItem::hoverEnterEvent(event);
}


void StructuralEntity::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
  QGraphicsItem::contextMenuEvent(event);

  if (!event->isAccepted())
  {
    if (!isSelected())
    {
      setSelected(true);
      //emit entitySelected(this);
      QMap<QString, QString> settings; emit selected(getLocalUid(), settings);
    }

    _insertPoint = event->pos();

    if (menu != NULL)
    {
      switch (_name) {
        case Structural::Media:
          menu->mediaAction->setEnabled(false);
          menu->contextAction->setEnabled(false);
          menu->switchAction->setEnabled(false);
          menu->bodyAction->setEnabled(false);
          menu->areaAction->setEnabled(true);
          menu->propertyAction->setEnabled(true);
          menu->portAction->setEnabled(false);
          menu->switchPortAction->setEnabled(false);
          break;

        case Structural::Context:
          menu->mediaAction->setEnabled(true);
          menu->contextAction->setEnabled(true);
          menu->switchAction->setEnabled(true);
          menu->bodyAction->setEnabled(false);
          menu->areaAction->setEnabled(false);
          menu->propertyAction->setEnabled(true);
          menu->portAction->setEnabled(true);
          menu->switchPortAction->setEnabled(false);
          break;

        case Structural::Switch:
          menu->mediaAction->setEnabled(true);
          menu->contextAction->setEnabled(true);
          menu->switchAction->setEnabled(true);
          menu->bodyAction->setEnabled(false);
          menu->areaAction->setEnabled(false);
          menu->propertyAction->setEnabled(true);
          menu->portAction->setEnabled(false);
          menu->switchPortAction->setEnabled(true);
          break;

        case Structural::Body:
          menu->mediaAction->setEnabled(true);
          menu->contextAction->setEnabled(true);
          menu->switchAction->setEnabled(true);
          menu->bodyAction->setEnabled(false);
          menu->areaAction->setEnabled(false);
          menu->propertyAction->setEnabled(true);
          menu->portAction->setEnabled(true);
          menu->switchPortAction->setEnabled(false);
          break;

        case Structural::Area:
        case Structural::Property:
        case Structural::Port:
        case Structural::SwitchPort:
          menu->mediaAction->setEnabled(false);
          menu->contextAction->setEnabled(false);
          menu->switchAction->setEnabled(false);
          menu->bodyAction->setEnabled(false);
          menu->areaAction->setEnabled(false);
          menu->propertyAction->setEnabled(false);
          menu->portAction->setEnabled(false);
          menu->switchPortAction->setEnabled(false);
          break;

        default:
          break;
      }

      menu->exec(event->screenPos());
    }

    event->accept();
  }
}

bool StructuralEntity::hasMouseHover()
{
  return hover;
}

void StructuralEntity::setMouseHover(bool hover)
{
  this->hover = hover;
}

void StructuralEntity::setMouseHoverHighlight(bool enable)
{
  this->enableMouseHoverHighlight = enable;
}

void StructuralEntity::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
  QGraphicsItem::hoverEnterEvent(event);

  hover = true;
}

void StructuralEntity::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
  QGraphicsItem::hoverLeaveEvent(event);

  hover = false;
}

void StructuralEntity::setError(bool hasError)
{
  this->hasError = hasError;
}

void StructuralEntity::setErrorMsg(QString erroMsg)
{
  this->erroMsg = erroMsg;
}

void StructuralEntity::drawMouseHoverHighlight(QPainter *painter)
{
  if(enableMouseHoverHighlight)
  {
    // Draw MouseHover rectangle
    if (!isSelected() && hasMouseHover())
    {
      painter->setBrush(Qt::NoBrush);
      painter->setPen(QPen(QBrush(QColor("#999999")), 0, Qt::DashLine)); // 0px = cosmetic border

      painter->drawRect(DEFAULT_ANCHOR_WIDTH/2, DEFAULT_ANCHOR_HEIGHT/2, getWidth(), getHeight());
    }
  }
}

void StructuralEntity::performInsert(Structural::EntityName name)
{

  QMap<QString, QString> properties;
  properties["LOCAL:NAME"] = QString::number(name);

  switch (name) {
    case Structural::Body:
    {
      properties["LOCAL:TOP"] = QString::number(_insertPoint.y() - DEFAULT_BODY_HEIGHT/2);
      properties["LOCAL:LEFT"] = QString::number(_insertPoint.x() - DEFAULT_BODY_WIDTH/2);
      properties["LOCAL:HEIGHT"] = QString::number(DEFAULT_BODY_HEIGHT);
      properties["LOCAL:WIDTH"] = QString::number(DEFAULT_BODY_WIDTH);

      break;
    }
    case Structural::Context:
    case Structural::Switch:
    {

      properties["LOCAL:TOP"] = QString::number(_insertPoint.y() - DEFAULT_CONTEXT_HEIGHT/2);
      properties["LOCAL:LEFT"] = QString::number(_insertPoint.x() - DEFAULT_CONTEXT_WIDTH/2);
      properties["LOCAL:HEIGHT"] = QString::number(DEFAULT_CONTEXT_HEIGHT);
      properties["LOCAL:WIDTH"] = QString::number(DEFAULT_CONTEXT_WIDTH);

      break;
    }

    case Structural::Media:
    {

      properties["LOCAL:TOP"] = QString::number(_insertPoint.y() - DEFAULT_MEDIA_HEIGHT/2);
      properties["LOCAL:LEFT"] = QString::number(_insertPoint.x() - DEFAULT_MEDIA_WIDTH/2);
      properties["LOCAL:HEIGHT"] = QString::number(DEFAULT_MEDIA_HEIGHT);
      properties["LOCAL:WIDTH"] = QString::number(DEFAULT_MEDIA_WIDTH);

      break;
    }

    default:
      break;
  }

  QMap<QString, QString> settings;
  settings["UNDO"] = "1";
  settings["NOTIFY"] = "1";
  settings["CODE"] = QUuid::createUuid().toString();

  inserted(QUuid::createUuid().toString(),getLocalUid(), properties, settings);
}

