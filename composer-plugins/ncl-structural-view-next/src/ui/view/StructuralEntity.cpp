#include "StructuralEntity.h"
#include "StructuralMedia.h"
#include "StructuralComposition.h"

StructuralEntity::StructuralEntity(StructuralEntity* parent)
  : QObject(parent), QGraphicsItem(parent)
{
  setnstUid((QString) QUuid::createUuid().toString());
  setnstType(Structural::NoType);
  setnstSubtype(Structural::NoSubtype);

  setnstParent(parent);

  setMoveable(true);
  setResizable(true);
  setSelectable(true);
  setHoverable(true);
  setDraggable(true);

  setMoving(false);
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

  setResizeAnchorWidth(8);
  setResizeAnchorHeight(8);

  setzIndex(0);

  setFlag(QGraphicsItem::ItemIsMovable, true);
  setFlag(QGraphicsItem::ItemIsFocusable, true);

  setAcceptHoverEvents(true);

  //  connect(this, SIGNAL(entitySelected()), SLOT(requestEntitySelection()));
  //  connect(this, SIGNAL(entityAboutToChange(QMap<QString,QString>)),
  //                SLOT(requestEntityPreparation(QMap<QString,QString>)));


  hover = false;
  menu = new StructuralMenu();
  connect(menu, SIGNAL(insert(Structural::EntitySubtype)),SLOT(performInsert(Structural::EntitySubtype)));

  draggable = false;
  hasError = false;
  enableMouseHoverHighlight = true;
}

StructuralEntity::~StructuralEntity()
{

}

QString StructuralEntity::getnstUid() const
{
  return uid;
}

void StructuralEntity::setnstUid(const QString &uid)
{
  this->uid = uid;

  properties[":nst:uid"] = uid;
}


QnstType StructuralEntity::getnstType() const
{
  return type;
}

void StructuralEntity::setnstType(const QnstType type)
{
  this->type = type;

  properties[":nst:type"] = QString::number(type);
}

QnstSubtype StructuralEntity::getnstSubtype() const
{
  return subtype;
}

void StructuralEntity::setnstSubtype(const QnstSubtype subtype)
{
  this->subtype = subtype;

  properties[":nst:subtype"] = QString::number(subtype);
}

QMap<QString, QString> StructuralEntity::getnstProperties() const
{
  return properties;
}

void StructuralEntity::setnstProperties(const QMap<QString, QString> &properties)
{
  foreach (QString name, properties.keys())
  {
    setnstProperty(name, properties[name]);
  }
}

QString StructuralEntity::getnstProperty(const QString &name)
{
  return properties.value(name,"");
}

void StructuralEntity::setnstProperty(const QString &name, const QString &value)
{
  properties.insert(name, value);


  if (name == ":nst:top")
  {
    setTop(value.toDouble());
  }
  else if (name == ":nst:left")
  {
    setLeft(value.toDouble());
  }
  else if (name == ":nst:width")
  {
    setWidth(value.toDouble());
  }
  else if (name == ":nst:height")
  {
    setHeight(value.toDouble());
  }
  else if (name == ":nst:zIndex")
  {
    setzIndex(value.toInt());
  }
  else if (name == ":nst:id")
  {
    setnstId(value);
  }
}


void StructuralEntity::setnstId(const QString &id)
{
  properties[":nst:id"] = id;
  updateToolTip();
}

void StructuralEntity::setMoveable(bool moveable)
{
  this->moveable = moveable;
}

bool StructuralEntity::isMoveable() const
{
  return moveable;
}

void StructuralEntity::setSelectable(bool selected)
{
  this->selectable = selected;
}

bool StructuralEntity::isSelectable() const
{
  return selectable;
}

bool StructuralEntity::isHoverable() const
{
  return hoverable;
}

void StructuralEntity::setHoverable(bool hoverable)
{
  this->hoverable = hoverable;
}

void StructuralEntity::setResizable(bool resizable)
{
  this->resizable = resizable;
}

bool StructuralEntity::isResizable() const
{
  return resizable;
}

void StructuralEntity::setMoving(bool moving)
{
  this->moving = moving;
}

bool StructuralEntity::isMoving() const
{
  return moving;
}

void StructuralEntity::setResizing(bool resizing)
{
  this->resizing = resizing;
}

bool StructuralEntity::isResizing() const
{
  return resizing;
}

bool StructuralEntity::isHovering() const
{
  return hovering;
}

void StructuralEntity::setHovering(bool hovering)
{
  this->hovering = hovering;
}

bool StructuralEntity::isDragging() const
{
  return dragging;
}

void StructuralEntity::setDragging(bool dragging)
{
  this->dragging = dragging;
}

void StructuralEntity::setSelected(bool selected)
{
  this->selectedd = selected;
}

bool StructuralEntity::isSelected() const
{
  return selectedd;
}

qreal StructuralEntity::getTop() const
{
  return top;
}

void StructuralEntity::setTop(qreal top)
{
  this->top = top;

  setY(top-4);
  properties[":nst:top"] = QString::number(top);
}

qreal StructuralEntity::getMoveTop() const
{
  return moveTop;
}

void StructuralEntity::setMoveTop(qreal moveTop)
{
  this->moveTop = moveTop;
}

qreal StructuralEntity::getPressTop() const
{
  return pressTop;
}

void StructuralEntity::setPressTop(qreal pressTop)
{
  this->pressTop = pressTop;
}

qreal StructuralEntity::getResizeTop() const
{
  return resizeTop;
}

void StructuralEntity::setResizeTop(qreal resizeTop)
{
  this->resizeTop = resizeTop;
}

qreal StructuralEntity::getLeft() const
{
  return left;
}

void StructuralEntity::setLeft(qreal left)
{
  this->left = left;

  setX(left-4);

  properties[":nst:left"] = QString::number(left);
}

qreal StructuralEntity::getMoveLeft() const
{
  return moveLeft;
}

void StructuralEntity::setMoveLeft(qreal moveLeft)
{
  this->moveLeft = moveLeft;
}

qreal StructuralEntity::getPressLeft() const
{
  return pressLeft;
}

void StructuralEntity::setPressLeft(qreal pressLeft)
{
  this->pressLeft = pressLeft;
}

qreal StructuralEntity::getResizeLeft() const
{
  return resizeLeft;
}

void StructuralEntity::setResizeLeft(qreal resizeLeft)
{
  this->resizeLeft = resizeLeft;
}

qreal StructuralEntity::getWidth() const
{
  return width;
}

void StructuralEntity::setWidth(qreal width)
{
  this->width = width;

  properties[":nst:width"] = QString::number(width);
}

qreal StructuralEntity::getPressWidth() const
{
  return pressWidth;
}

void StructuralEntity::setPressWidth(qreal pressWidth)
{
  this->pressWidth = pressWidth;
}

qreal StructuralEntity::getResizeWidth() const
{
  return resizeWidth;
}


void StructuralEntity::setResizeWidth(qreal resizeWidth)
{
  this->resizeWidth = resizeWidth;
}

qreal StructuralEntity::getHeight() const
{
  return height;
}

void StructuralEntity::setHeight(qreal height)
{
  this->height = height;

  properties[":nst:height"] = QString::number(height);
}

qreal StructuralEntity::getPressHeight() const
{
  return pressHeight;
}

void StructuralEntity::setPressHeight(qreal pressHeight)
{
  this->pressHeight = pressHeight;
}

qreal StructuralEntity::getResizeHeight() const
{
  return resizeHeight;
}

void StructuralEntity::setResizeHeight(qreal resizeHeight)
{
  this->resizeHeight = resizeHeight;

}

qreal StructuralEntity::getzIndex() const
{
  return zIndex;
}

void StructuralEntity::setzIndex(qreal zIndex)
{
  this->zIndex = zIndex;

  setZValue(zIndex);

  properties[":nst:zIndex"] = QString::number(zIndex);
}

QnstResizeType StructuralEntity::getResizeType() const
{
  return resizeType;
}

void StructuralEntity::setResizeType(QnstResizeType resizeType)
{
  this->resizeType = resizeType;
}

StructuralEntity* StructuralEntity::getnstParent() const
{
  return parent;
}

void StructuralEntity::setnstParent(StructuralEntity* parent)
{
  this->parent = parent;

  setParent(parent);
  setParentItem(parent);

  if (parent !=NULL)
    parent->insertChild(this);
}

QVector<StructuralEntity*> StructuralEntity::getnstChildren()
{
  return children;
}

bool StructuralEntity::isDraggable()
{
  return draggable;
}

void StructuralEntity::setDraggable(bool isDraggable)
{
  this->draggable = isDraggable;
}

qreal StructuralEntity::getResizeAnchorWidth() const
{
  return resizeAnchorWidth;
}

void StructuralEntity::setResizeAnchorWidth(qreal resizeAnchorWidth)
{
  this->resizeAnchorWidth = resizeAnchorWidth;
}

qreal StructuralEntity::getResizeAnchorHeight() const
{
  return resizeAnchorHeight;
}

void StructuralEntity::setResizeAnchorHeight(qreal resizeAnchorHeight)
{
  this->resizeAnchorHeight = resizeAnchorHeight;
}

void StructuralEntity::insertChild(StructuralEntity* child)
{
  if (child != NULL)
  {
    // \fixme This is only an additional check. Maybe, the better way to do
    // that could be using a set instead of a vector to entities.
    if(!children.contains(child))
    {
      children.append(child);

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

                    void StructuralEntity::removeChild(StructuralEntity* child)
      {
                    if (child != NULL)
      {
                    int index = children.indexOf(child);

                    if (index >= 0)
                    children.remove(index);

                    child->setnstParent(NULL);
    }
    }

                    void StructuralEntity::newChild(Structural::EntitySubtype type)
      {
                    /****************************************************
                              // \todo Check if type is an media type allowed to me!
                              QnstGraphicsEntity *entity = QnstUtil::makeGraphicsEntity(type, this);

                              if(entity == NULL) return false;

                              QnstGraphicsMedia *content  = dynamic_cast<QnstGraphicsMedia*>(entity);

                              if(content != NULL) // If the Entity is a Media content
                              {
                                content->adjust();

                                if (dropsrc != "") //if it is a drop we will keep the baseName as id
                                {
                                  content->setSource(dropsrc);
                                  QFileInfo file = QFileInfo(dropsrc);
                                  QString nstId = file.baseName();
                                  entity->setnstId(nstId);
                                  dropsrc = "";
                                }
                              }
                              else
                              {
                                QnstGraphicsComposition *composition =
                                    dynamic_cast<QnstGraphicsComposition*>(entity);

                                //If the Entity is a Composition (i.e. Body, Context or Switch)
                                if(composition != NULL)
                                {
                                  composition->adjust();

                                  composition->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());
                                }
                              }

                              entity->adjust();

                              emit entityAdded(entity);

                              *****************************************************/

                    /*****************************************************/

                    QString uid = QUuid::createUuid().toString();
                    QString parent = getnstUid();
                    QMap<QString, QString> properties;
                    properties[":nst:subtype"] = StructuralUtil::getStrFromNstType(type);

      QMap<QString, QString> settings;
      settings["UNDO"] = "1";
      settings["NOTIFY"] = "1";
      settings["CODE"] = QUuid::createUuid().toString();

      emit inserted(uid, parent, properties, settings);

      /*****************************************************/


    }

    QPainterPath StructuralEntity::shape() const
    {
      QPainterPath painter;

      delineate(&painter);

      if (selectable && selectedd && resizable){
        painter.setFillRule(Qt::WindingFill);

        painter.addRect(0,0,8,8);                               // topleft
        painter.addRect((width+8)/2 - 4,0,8,8);                 // top
        painter.addRect((width+8) - 8,0,8,8);                   // topright
        painter.addRect((width+8) - 8,(height+8)/2 - 4,8,8);    // right
        painter.addRect((width+8) - 8,(height+8) - 8,8,8);      // bottomright
        painter.addRect((width+8)/2 - 4,(height+8) - 8,8,8);    // bottom
        painter.addRect(0,(height+8) - 8,8,8);                  // bottomleft
        painter.addRect(0,(height+8)/2 - 4,8,8);                // left
      }

      return painter;
    }

    QRectF StructuralEntity::boundingRect() const
    {
      QRectF bounds;

      bounds.setX(0);
      bounds.setY(0);
      bounds.setWidth(width+8);
      bounds.setHeight(height+8);

      return bounds;
    }

    void StructuralEntity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {
      draw(painter);

      if (selectable && selectedd){
        painter->setRenderHint(QPainter::Antialiasing,false);

        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0, Qt::DashLine));

        painter->drawRect(4, 4, getWidth()-1, getHeight()-1);

        if (resizable){
          painter->setBrush(QBrush(Qt::white));
          painter->setPen(QPen(QBrush(Qt::black), 0));

          painter->drawRect(0,0,8,8);                             // topleft
          painter->drawRect((width+8)/2-4-1,0,8,8);               // top
          painter->drawRect((width+8)-8-1,0,8,8);                 // topright
          painter->drawRect((width+8)-8-1,(height+8)/2-4-1,8,8);  // right
          painter->drawRect((width+8)-8-1,(height+8)-8-1,8,8);    // bottomright
          painter->drawRect((width+8)/2-4-1,(height+8)-8-1,8,8);  // bottom
          painter->drawRect(0,(height+8)-8-1,8,8);                // bottomleft
          painter->drawRect(0,(height+8)/2-4-1,8,8);              // left
        }
      }
    }

    void StructuralEntity::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
    {
      if (moveable && moving){
        move(event);

      } else if (resizable && resizing){
        resize(event);
      }
    }

    void StructuralEntity::mousePressEvent(QGraphicsSceneMouseEvent* event)
    {
      if (event->button() == Qt::RightButton){
        event->ignore();

      }else if (event->button() == Qt::LeftButton){
        if (selectable && !selectedd){
          setSelected(true); emit selected(getnstUid(), QMap<QString, QString>());
        }

        setPressTop(event->pos().y());
        setPressLeft(event->pos().x());
        setPressWidth(width);
        setPressHeight(height);

        // avoid flickering on first move
        if (moveable){
          setMoveTop(top);
          setMoveLeft(left);
        }

        // avoid flickering on first resize
        if (resizable){
          setResizeTop(top);
          setResizeLeft(left);
          setResizeWidth(width);
          setResizeHeight(height);
        }

        if (resizable){
          // if over TOPLEFT resize region
          if (QRectF(0,0,8,8).contains(event->pos())){
            setResizeType(Structural::TopLeft);
            setResizing(true);

            // if over TOP resize region
          }else if (QRectF((width+8)/2 - 4,0,8,8).contains(event->pos())){
            setResizeType(Structural::Top);
            setResizing(true);

            // if over TOPRIGHT resize region
          }else if (QRectF((width+8) - 8,0,8,8).contains(event->pos())){
            setResizeType(Structural::TopRight);
            setResizing(true);

            // if over RIGHT resize region
          }else if (QRectF((width+8) - 8,(height+8)/2 - 4,8,8).contains(event->pos())){
            setResizeType(Structural::Right);
            setResizing(true);

            // if over BOTTOMRIGHT resize region
          }else if (QRectF((width+8) - 8,(height+8) - 8,8,8).contains(event->pos())){
            setResizeType(Structural::BottomRight);
            setResizing(true);

            // if over BOTTOM resize region
          }else if (QRectF((width+8)/2 - 4,(height+8) - 8,8,8).contains(event->pos())){
            setResizeType(Structural::Bottom);
            setResizing(true);

            // if over BOTTOMLEFT resize region
          }else if (QRectF(0,(height+8) - 8,8,8).contains(event->pos())){
            setResizeType(Structural::BottomLeft);
            setResizing(true);

            // if over LEFT resize region
          }else if (QRectF(0,(height+8)/2 - 4,8,8).contains(event->pos())){
            setResizeType(Structural::Left);
            setResizing(true);

            // if not over any resize region
          }else if (moveable){
            setMoving(true);
          }

        }else if (moveable){
          setMoving(true);
        }
      }

      QGraphicsItem::mousePressEvent(event);
    }

    void StructuralEntity::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
    {
      if (moving){
        setMoving(false);

        if ((top != moveTop || left != moveLeft)){

          /**********************/
          QMap<QString, QString> previous = getnstProperties();

          //            emit entityAboutToChange(properties);

          setTop(moveTop);
          setLeft(moveLeft);

          adjust();

          QMap<QString, QString> properties = getnstProperties();
          //            properties["top"] = QString::number(moveTop);
          //            properties["left"] = QString::number(moveLeft);

          QMap<QString, QString> settings;
          settings["UNDO"] = "1";
          settings["ADJUST"] = "0";
          settings["CODE"] = QUuid::createUuid().toString();

          emit changed(getnstUid(), properties, previous,settings);


          //            emit entityChanged();


          /**************/

        }else{
          adjust();
        }

      }else if (resizing){
        setResizing(false);

        QMap<QString, QString> previous = getnstProperties();

        if ((top != resizeTop || left != resizeLeft || width != resizeWidth || height != resizeHeight)){
          if (resizeTop > top + height){
            setResizeHeight(resizeTop - (top + height));
            setResizeTop(top + height);
          }

          if (resizeLeft > left + width){
            setResizeWidth(resizeLeft - (left + width));
            setResizeLeft(left + width);
          }

          if (resizeWidth < 0){
            setResizeLeft(resizeLeft + resizeWidth);
            setResizeWidth(-resizeWidth);
          }

          if (resizeHeight < 0){
            setResizeTop(resizeTop + resizeHeight);
            setResizeHeight(-resizeHeight);
          }

          foreach(StructuralEntity* entity, children){
            entity->setTop(entity->getTop() - (resizeTop - top));
            entity->setLeft(entity->getLeft() - (resizeLeft - left));
          }

          //            QMap<QString, QString> properties;

          //            properties["top"] = QString::number(moveTop);
          //            properties["left"] = QString::number(moveLeft);
          //            properties["width"] = QString::number(resizeWidth);
          //            properties["height"] = QString::number(resizeHeight);

          //            emit entityAboutToChange(properties);

          setTop(resizeTop);
          setLeft(resizeLeft);
          setWidth(resizeWidth);
          setHeight(resizeHeight);

          adjust();

          QMap<QString, QString> properties = getnstProperties();

          //            emit entityChanged();

          QMap<QString, QString> settings;
          settings["UNDO"] = "1";
          settings["ADJUST"] = "0";
          settings["CODE"] = QUuid::createUuid().toString();

          emit changed(getnstUid(), properties, previous,settings);

        }else{
          adjust();
        }
      }

      QGraphicsItem::mouseReleaseEvent(event);
    }

    void StructuralEntity::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
    {
      if (selectedd){
        if (resizable){
          // if over TOPLEFT resize region
          if (QRectF(0,0,8,8).contains(event->pos())){
            setCursor(Qt::SizeFDiagCursor);

            // if over TOP resize region
          }else if (QRectF((width+8)/2 - 4,0,8,8).contains(event->pos())){
            setCursor(Qt::SizeVerCursor);

            // if over TOPRIGHT resize region
          }else if (QRectF((width+8) - 8,0,8,8).contains(event->pos())){
            setCursor(Qt::SizeBDiagCursor);

            // if over RIGHT resize region
          }else if (QRectF((width+8) - 8,(height+8)/2 - 4,8,8).contains(event->pos())){
            setCursor(Qt::SizeHorCursor);

            // if over BOTTOMRIGHT resize region
          }else if (QRectF((width+8) - 8,(height+8) - 8,8,8).contains(event->pos())){
            setCursor(Qt::SizeFDiagCursor);

            // if over BOTTOM resize region
          }else if (QRectF((width+8)/2 - 4,(height+8) - 8,8,8).contains(event->pos())){
            setCursor(Qt::SizeVerCursor);

            // if over BOTTOMLEFT resize region
          }else if (QRectF(0,(height+8) - 8,8,8).contains(event->pos())){
            setCursor(Qt::SizeBDiagCursor);

            // if over LEFT resize region
          }else if (QRectF(0,(height+8)/2 - 4,8,8).contains(event->pos())){
            setCursor(Qt::SizeHorCursor);

            // if not over any resize region
          }else if (moveable){
            setCursor(Qt::SizeAllCursor);
          }

        }else if (moveable){
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
          emit selected(getnstUid(), QMap<QString, QString>());
        }

        _insertPoint = event->pos();

        if (menu != NULL)
        {

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

          painter->drawRect(4, 4, getWidth(), getHeight());
        }
      }
    }

  void StructuralEntity::performInsert(Structural::EntitySubtype name)
  {

    QMap<QString, QString> properties;
    properties[":nst:subtype"] = StructuralUtil::getStrFromNstType(name);

    switch (name) {
      case Structural::Body:
      {
        properties[":nst:top"] = QString::number(_insertPoint.y() - DEFAULT_BODY_HEIGHT/2);
        properties[":nst:left"] = QString::number(_insertPoint.x() - DEFAULT_BODY_WIDTH/2);
        properties[":nst:height"] = QString::number(DEFAULT_BODY_HEIGHT);
        properties[":nst:width"] = QString::number(DEFAULT_BODY_WIDTH);

        break;
      }
      case Structural::Context:
      case Structural::Switch:
      {

        properties[":nst:top"] = QString::number(_insertPoint.y() - DEFAULT_CONTEXT_HEIGHT/2);
        properties[":nst:left"] = QString::number(_insertPoint.x() - DEFAULT_CONTEXT_WIDTH/2);
        properties[":nst:height"] = QString::number(DEFAULT_CONTEXT_HEIGHT);
        properties[":nst:width"] = QString::number(DEFAULT_CONTEXT_WIDTH);

        break;
      }

      case Structural::Media:
      {

        properties[":nst:top"] = QString::number(_insertPoint.y() - DEFAULT_MEDIA_HEIGHT/2);
        properties[":nst:left"] = QString::number(_insertPoint.x() - DEFAULT_MEDIA_WIDTH/2);
        properties[":nst:height"] = QString::number(DEFAULT_MEDIA_HEIGHT);
        properties[":nst:width"] = QString::number(DEFAULT_MEDIA_WIDTH);

        break;
      }

      default:
        break;
    }

    QMap<QString, QString> settings;
    settings["UNDO"] = "1";
    settings["NOTIFY"] = "1";
    settings["CODE"] = QUuid::createUuid().toString();

    inserted(QUuid::createUuid().toString(),getnstUid(), properties, settings);
  }

