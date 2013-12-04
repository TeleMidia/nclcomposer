#include "qnstentity.h"
#include "qnstmedia.h"
#include "qnstcomposition.h"

QnstEntity::QnstEntity(QnstEntity* parent)
  : QObject(parent), QGraphicsItem(parent)
{
  setnstUid((QString) QUuid::createUuid().toString());
  setnstType(Qnst::NoType);
  setnstSubtype(Qnst::NoSubtype);

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

  if(parent)
    parent->insertChild(this);

  //  connect(this, SIGNAL(entitySelected()), SLOT(requestEntitySelection()));
  //  connect(this, SIGNAL(entityAboutToChange(QMap<QString,QString>)),
  //                SLOT(requestEntityPreparation(QMap<QString,QString>)));

  hover = false;
  menu = NULL;
  draggable = false;
  hasError = false;
  enableMouseHoverHighlight = true;
}

QnstEntity::~QnstEntity()
{

}

QString QnstEntity::getnstUid() const
{
  return uid;
}

void QnstEntity::setnstUid(const QString &uid)
{
  this->uid = uid;

  properties[":nst:uid"] = uid;
}


QnstType QnstEntity::getnstType() const
{
  return type;
}

void QnstEntity::setnstType(const QnstType type)
{
  this->type = type;

  properties[":nst:uid"] = QString::number(type);
}

QnstSubtype QnstEntity::getnstSubtype() const
{
  return subtype;
}

void QnstEntity::setnstSubtype(const QnstSubtype subtype)
{
  this->subtype = subtype;

  properties[":nst:subtype"] = QString::number(subtype);
}

QMap<QString, QString> QnstEntity::getnstProperties() const
{
  return properties;
}

void QnstEntity::setnstProperties(const QMap<QString, QString> &properties)
{
  foreach (QString name, properties.keys())
  {
    setnstProperty(name, properties[name]);
  }
}

QString QnstEntity::getnstProperty(const QString &name)
{
  return properties.value(name,"");
}

void QnstEntity::setnstProperty(const QString &name, const QString &value)
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
}


void QnstEntity::setnstId(const QString &id)
{
  properties[":nst:id"] = id;
  updateToolTip();
}

void QnstEntity::setMoveable(bool moveable)
{
  this->moveable = moveable;
}

bool QnstEntity::isMoveable() const
{
  return moveable;
}

void QnstEntity::setSelectable(bool selected)
{
  this->selectable = selected;
}

bool QnstEntity::isSelectable() const
{
  return selectable;
}

bool QnstEntity::isHoverable() const
{
  return hoverable;
}

void QnstEntity::setHoverable(bool hoverable)
{
  this->hoverable = hoverable;
}

void QnstEntity::setResizable(bool resizable)
{
  this->resizable = resizable;
}

bool QnstEntity::isResizable() const
{
  return resizable;
}

void QnstEntity::setMoving(bool moving)
{
  this->moving = moving;
}

bool QnstEntity::isMoving() const
{
  return moving;
}

void QnstEntity::setResizing(bool resizing)
{
  this->resizing = resizing;
}

bool QnstEntity::isResizing() const
{
  return resizing;
}

bool QnstEntity::isHovering() const
{
  return hovering;
}

void QnstEntity::setHovering(bool hovering)
{
  this->hovering = hovering;
}

bool QnstEntity::isDragging() const
{
  return dragging;
}

void QnstEntity::setDragging(bool dragging)
{
  this->dragging = dragging;
}

void QnstEntity::setSelected(bool selected)
{
  this->selectedd = selected;
}

bool QnstEntity::isSelected() const
{
  return selectedd;
}

qreal QnstEntity::getTop() const
{
  return top;
}

void QnstEntity::setTop(qreal top)
{
  this->top = top;

  setY(top-4);
  properties[":nst:top"] = QString::number(top);
}

qreal QnstEntity::getMoveTop() const
{
  return moveTop;
}

void QnstEntity::setMoveTop(qreal moveTop)
{
  this->moveTop = moveTop;
}

qreal QnstEntity::getPressTop() const
{
  return pressTop;
}

void QnstEntity::setPressTop(qreal pressTop)
{
  this->pressTop = pressTop;
}

qreal QnstEntity::getResizeTop() const
{
  return resizeTop;
}

void QnstEntity::setResizeTop(qreal resizeTop)
{
  this->resizeTop = resizeTop;
}

qreal QnstEntity::getLeft() const
{
  return left;
}

void QnstEntity::setLeft(qreal left)
{
  this->left = left;

  setX(left-4);

  properties[":nst:left"] = QString::number(left);
}

qreal QnstEntity::getMoveLeft() const
{
  return moveLeft;
}

void QnstEntity::setMoveLeft(qreal moveLeft)
{
  this->moveLeft = moveLeft;
}

qreal QnstEntity::getPressLeft() const
{
  return pressLeft;
}

void QnstEntity::setPressLeft(qreal pressLeft)
{
  this->pressLeft = pressLeft;
}

qreal QnstEntity::getResizeLeft() const
{
  return resizeLeft;
}

void QnstEntity::setResizeLeft(qreal resizeLeft)
{
  this->resizeLeft = resizeLeft;
}

qreal QnstEntity::getWidth() const
{
  return width;
}

void QnstEntity::setWidth(qreal width)
{
  this->width = width;

  properties[":nst:width"] = QString::number(width);
}

qreal QnstEntity::getPressWidth() const
{
  return pressWidth;
}

void QnstEntity::setPressWidth(qreal pressWidth)
{
  this->pressWidth = pressWidth;
}

qreal QnstEntity::getResizeWidth() const
{
  return resizeWidth;
}


void QnstEntity::setResizeWidth(qreal resizeWidth)
{
  this->resizeWidth = resizeWidth;
}

qreal QnstEntity::getHeight() const
{
  return height;
}

void QnstEntity::setHeight(qreal height)
{
  this->height = height;

  properties[":nst:height"] = QString::number(height);
}

qreal QnstEntity::getPressHeight() const
{
  return pressHeight;
}

void QnstEntity::setPressHeight(qreal pressHeight)
{
  this->pressHeight = pressHeight;
}

qreal QnstEntity::getResizeHeight() const
{
  return resizeHeight;
}

void QnstEntity::setResizeHeight(qreal resizeHeight)
{
  this->resizeHeight = resizeHeight;

}

qreal QnstEntity::getzIndex() const
{
  return zIndex;
}

void QnstEntity::setzIndex(qreal zIndex)
{
  this->zIndex = zIndex;

  setZValue(zIndex);

  properties[":nst:zIndex"] = QString::number(zIndex);
}

QnstResizeType QnstEntity::getResizeType() const
{
  return resizeType;
}

void QnstEntity::setResizeType(QnstResizeType resizeType)
{
  this->resizeType = resizeType;
}

QnstEntity* QnstEntity::getnstParent() const
{
  return parent;
}

void QnstEntity::setnstParent(QnstEntity* parent)
{
  this->parent = parent;

  setParent(parent);
  setParentItem(parent);
}

QVector<QnstEntity*> QnstEntity::getnstChildren()
{
  return children;
}

bool QnstEntity::isDraggable()
{
  return draggable;
}

void QnstEntity::setDraggable(bool isDraggable)
{
  this->draggable = isDraggable;
}

qreal QnstEntity::getResizeAnchorWidth() const
{
  return resizeAnchorWidth;
}

void QnstEntity::setResizeAnchorWidth(qreal resizeAnchorWidth)
{
  this->resizeAnchorWidth = resizeAnchorWidth;
}

qreal QnstEntity::getResizeAnchorHeight() const
{
  return resizeAnchorHeight;
}

void QnstEntity::setResizeAnchorHeight(qreal resizeAnchorHeight)
{
  this->resizeAnchorHeight = resizeAnchorHeight;
}

void QnstEntity::insertChild(QnstEntity* child)
{
  if (child != NULL)
  {
    // \fixme This is only an additional check. Maybe, the better way to do
    // that could be using a set instead of a vector to entities.
    if(!children.contains(child))
    {
      children.push_back(child);

      // connect(entity, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
      // connect(entity, SIGNAL(redoRequested()), SIGNAL(redoRequested()));



      if(child)
        child->setnstParent(this);
    }
    else
    {
      qWarning() << "[QNST] Warning! You are adding the same entity twice as \
                    child of " << QString().sprintf("%p", this) << __FILE__ << __LINE__;
    }
    }
    }

                    void QnstEntity::removeChild(QnstEntity* child)
      {
                    if (child != NULL)
      {
                    int index = children.indexOf(child);

                    if (index >= 0)
                    children.remove(index);

                    child->setnstParent(NULL);
    }
    }

                    void QnstEntity::newChild(Qnst::EntitySubtype type)
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
                    properties[":nst:subtype"] = QnstUtil::getStrFromNstType(type);

      QMap<QString, QString> settings;
      settings["UNDO"] = "1";
      settings["NOTIFY"] = "1";
      settings["CODE"] = QUuid::createUuid().toString();

      emit inserted(uid, parent, properties, settings);

      /*****************************************************/


    }

    QPainterPath QnstEntity::shape() const
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

    QRectF QnstEntity::boundingRect() const
    {
      QRectF bounds;

      bounds.setX(0);
      bounds.setY(0);
      bounds.setWidth(width+8);
      bounds.setHeight(height+8);

      return bounds;
    }

    void QnstEntity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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

    void QnstEntity::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
    {
      if (moveable && moving){
        move(event);

      } else if (resizable && resizing){
        resize(event);
      }
    }

    void QnstEntity::mousePressEvent(QGraphicsSceneMouseEvent* event)
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
            setResizeType(Qnst::TopLeft);
            setResizing(true);

            // if over TOP resize region
          }else if (QRectF((width+8)/2 - 4,0,8,8).contains(event->pos())){
            setResizeType(Qnst::Top);
            setResizing(true);

            // if over TOPRIGHT resize region
          }else if (QRectF((width+8) - 8,0,8,8).contains(event->pos())){
            setResizeType(Qnst::TopRight);
            setResizing(true);

            // if over RIGHT resize region
          }else if (QRectF((width+8) - 8,(height+8)/2 - 4,8,8).contains(event->pos())){
            setResizeType(Qnst::Right);
            setResizing(true);

            // if over BOTTOMRIGHT resize region
          }else if (QRectF((width+8) - 8,(height+8) - 8,8,8).contains(event->pos())){
            setResizeType(Qnst::BottomRight);
            setResizing(true);

            // if over BOTTOM resize region
          }else if (QRectF((width+8)/2 - 4,(height+8) - 8,8,8).contains(event->pos())){
            setResizeType(Qnst::Bottom);
            setResizing(true);

            // if over BOTTOMLEFT resize region
          }else if (QRectF(0,(height+8) - 8,8,8).contains(event->pos())){
            setResizeType(Qnst::BottomLeft);
            setResizing(true);

            // if over LEFT resize region
          }else if (QRectF(0,(height+8)/2 - 4,8,8).contains(event->pos())){
            setResizeType(Qnst::Left);
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

    void QnstEntity::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
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

          foreach(QnstEntity* entity, children){
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

    void QnstEntity::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
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


    void QnstEntity::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
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

        if (menu != NULL)
          menu->exec(event->screenPos());

        event->accept();
      }
    }

    bool QnstEntity::hasMouseHover()
    {
      return hover;
    }

    void QnstEntity::setMouseHover(bool hover)
    {
      this->hover = hover;
    }

    void QnstEntity::setMouseHoverHighlight(bool enable)
    {
      this->enableMouseHoverHighlight = enable;
    }

    void QnstEntity::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
    {
      QGraphicsItem::hoverEnterEvent(event);

      hover = true;
    }

    void QnstEntity::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
    {
      QGraphicsItem::hoverLeaveEvent(event);

      hover = false;
    }

    void QnstEntity::setError(bool hasError)
    {
      this->hasError = hasError;
    }

    void QnstEntity::setErrorMsg(QString erroMsg)
    {
      this->erroMsg = erroMsg;
    }

    void QnstEntity::drawMouseHoverHighlight(QPainter *painter)
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

