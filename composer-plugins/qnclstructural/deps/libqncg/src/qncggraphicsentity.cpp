#include "qncggraphicsentity.h"


QncgGraphicsEntity::QncgGraphicsEntity(QncgGraphicsEntity* parent)
    : QObject(parent), QGraphicsItem(parent)
{
    setncgGraphicsParent(parent);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    setMoveable(true);
    setResizable(true);
    setSelectable(true);

    setMoving(false);
    setResizing(false);
    setSelected(false);

    setTop(0);
    setLeft(0);
    setWidth(0);
    setHeight(0);

    setMoveTop(0);
    setMoveLeft(0);

    setResizeTop(0);
    setResizeLeft(0);
    setResizeWidth(0);
    setResizeHeight(0);

    setPressTop(0);
    setPressLeft(0);
    setPressWidth(0);
    setPressHeight(0);
}

QncgGraphicsEntity::~QncgGraphicsEntity()
{

}

void QncgGraphicsEntity::setMoveable(bool moveable)
{
    this->moveable = moveable;
}

bool QncgGraphicsEntity::isMoveable() const
{
    return moveable;
}

void QncgGraphicsEntity::setSelectable(bool selected)
{
    this->selectable = selected;
}

bool QncgGraphicsEntity::isSelectable() const
{
    return selectable;
}

void QncgGraphicsEntity::setResizable(bool resizable)
{
    this->resizable = resizable;
}

bool QncgGraphicsEntity::isResizable() const
{
    return resizable;
}

void QncgGraphicsEntity::setMoving(bool moving)
{
    this->moving = moving;
}

bool QncgGraphicsEntity::isMoving() const
{
    return moving;
}

void QncgGraphicsEntity::setResizing(bool resizing)
{
    this->resizing = resizing;
}

bool QncgGraphicsEntity::isResizing() const
{
    return resizing;
}

void QncgGraphicsEntity::setSelected(bool selected)
{
    this->selected = selected;
}

bool QncgGraphicsEntity::isSelected() const
{
    return selected;
}

qreal QncgGraphicsEntity::getTop() const
{
    return top;
}

void QncgGraphicsEntity::setTop(qreal top)
{
    this->top = top;

    setY(top-4);
}

qreal QncgGraphicsEntity::getMoveTop() const
{
    return moveTop;
}

void QncgGraphicsEntity::setMoveTop(qreal moveTop)
{
    this->moveTop = moveTop;
}

qreal QncgGraphicsEntity::getPressTop() const
{
    return pressTop;
}

void QncgGraphicsEntity::setPressTop(qreal pressTop)
{
    this->pressTop = pressTop;
}

qreal QncgGraphicsEntity::getResizeTop() const
{
    return resizeTop;
}

void QncgGraphicsEntity::setResizeTop(qreal resizeTop)
{
    this->resizeTop = resizeTop;
}

qreal QncgGraphicsEntity::getLeft() const
{
    return left;
}

void QncgGraphicsEntity::setLeft(qreal left)
{
    this->left = left;

    setX(left-4);
}

qreal QncgGraphicsEntity::getMoveLeft() const
{
    return moveLeft;
}

void QncgGraphicsEntity::setMoveLeft(qreal moveLeft)
{
    this->moveLeft = moveLeft;
}

qreal QncgGraphicsEntity::getPressLeft() const
{
    return pressLeft;
}

void QncgGraphicsEntity::setPressLeft(qreal pressLeft)
{
    this->pressLeft = pressLeft;
}

qreal QncgGraphicsEntity::getResizeLeft() const
{
    return resizeLeft;
}

void QncgGraphicsEntity::setResizeLeft(qreal resizeLeft)
{
    this->resizeLeft = resizeLeft;
}

qreal QncgGraphicsEntity::getWidth() const
{
    return width;
}

void QncgGraphicsEntity::setWidth(qreal width)
{
    this->width = width;
}

qreal QncgGraphicsEntity::getPressWidth() const
{
    return pressWidth;
}

void QncgGraphicsEntity::setPressWidth(qreal pressWidth)
{
    this->pressWidth = pressWidth;
}

qreal QncgGraphicsEntity::getResizeWidth() const
{
    return resizeWidth;
}

void QncgGraphicsEntity::setResizeWidth(qreal resizeWidth)
{
    this->resizeWidth = resizeWidth;
}

qreal QncgGraphicsEntity::getHeight() const
{
    return height;
}

void QncgGraphicsEntity::setHeight(qreal height)
{
    this->height = height;
}

qreal QncgGraphicsEntity::getPressHeight() const
{
    return pressHeight;
}

void QncgGraphicsEntity::setPressHeight(qreal pressHeight)
{
    this->pressHeight = pressHeight;
}

qreal QncgGraphicsEntity::getResizeHeight() const
{
    return resizeHeight;
}

void QncgGraphicsEntity::setResizeHeight(qreal resizeHeight)
{
    this->resizeHeight = resizeHeight;
}

QncgResizeType QncgGraphicsEntity::getncgResize() const
{
    return resizeType;
}

void QncgGraphicsEntity::setncgResize(QncgResizeType resizeType)
{
    this->resizeType = resizeType;
}

QncgEntityType QncgGraphicsEntity::getncgType() const
{
    return entityType;
}

void QncgGraphicsEntity::setncgType(QncgEntityType entityType)
{
    this->entityType = entityType;
}

QncgGraphicsEntity* QncgGraphicsEntity::getncgGraphicsParent() const
{
    return parent;
}

void QncgGraphicsEntity::setncgGraphicsParent(QncgGraphicsEntity* parent)
{
    this->parent = parent;

    setParent(parent);
    setParentItem(parent);
}

QVector<QncgGraphicsEntity*> QncgGraphicsEntity::getncgGraphicsEntities()
{
    return entities;
}

void QncgGraphicsEntity::addncgGraphicsEntity(QncgGraphicsEntity* entity)
{
    if (entity != NULL){
        entities.append(entity);
    }
}

void QncgGraphicsEntity::removencgGraphicsEntity(QncgGraphicsEntity* entity)
{
    if (entity != NULL){
        int index = entities.indexOf(entity);

        if (index >= 0){
            entities.remove(index);

            scene()->removeItem(entity);
        }
    }
}

QPainterPath QncgGraphicsEntity::shape() const
{
    QPainterPath painter;

    delineate(&painter);

    if (selectable && selected && resizable){
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

QRectF QncgGraphicsEntity::boundingRect() const
{
    QRectF bounds;

    bounds.setX(0);
    bounds.setY(0);
    bounds.setWidth(width+8);
    bounds.setHeight(height+8);

    return bounds;
}

void QncgGraphicsEntity::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    draw(painter);

    if (selectable && selected){
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

void QncgGraphicsEntity::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (moveable && moving){
        move(event);

    }else if (resizable && resizing){
        resize(event);
    }
}

void QncgGraphicsEntity::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton){
        event->ignore();

    }else if (event->button() == Qt::LeftButton){
        if (selectable && !selected){
            setSelected(true);

            emit entitySelected();
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
                setncgResize(Qncg::TopLeft);
                setResizing(true);

            // if over TOP resize region
            }else if (QRectF((width+8)/2 - 4,0,8,8).contains(event->pos())){
                setncgResize(Qncg::Top);
                setResizing(true);

            // if over TOPRIGHT resize region
            }else if (QRectF((width+8) - 8,0,8,8).contains(event->pos())){
                setncgResize(Qncg::TopRight);
                setResizing(true);

            // if over RIGHT resize region
            }else if (QRectF((width+8) - 8,(height+8)/2 - 4,8,8).contains(event->pos())){
                setncgResize(Qncg::Right);
                setResizing(true);

            // if over BOTTOMRIGHT resize region
            }else if (QRectF((width+8) - 8,(height+8) - 8,8,8).contains(event->pos())){
                setncgResize(Qncg::BottomRight);
                setResizing(true);

            // if over BOTTOM resize region
            }else if (QRectF((width+8)/2 - 4,(height+8) - 8,8,8).contains(event->pos())){
                setncgResize(Qncg::Bottom);
                setResizing(true);

            // if over BOTTOMLEFT resize region
            }else if (QRectF(0,(height+8) - 8,8,8).contains(event->pos())){
                setncgResize(Qncg::BottomLeft);
                setResizing(true);

            // if over LEFT resize region
            }else if (QRectF(0,(height+8)/2 - 4,8,8).contains(event->pos())){
                setncgResize(Qncg::Left);
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

void QncgGraphicsEntity::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
{
    if (moving){
        setMoving(false);

        if ((top != moveTop || left != moveLeft)){
            setTop(moveTop);
            setLeft(moveLeft);

            adjust();

            emit entityChanged();

        }else{
            adjust();
        }

    }else if (resizing){
        setResizing(false);

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

            foreach(QncgGraphicsEntity* entity, entities){
                entity->setTop(entity->getTop() - (resizeTop - top));
                entity->setLeft(entity->getLeft() - (resizeLeft - left));
            }

            setTop(resizeTop);
            setLeft(resizeLeft);
            setWidth(resizeWidth);
            setHeight(resizeHeight);

            adjust();

            emit entityChanged();

        }else{
            adjust();
        }
    }

    QGraphicsItem::mouseReleaseEvent(event);
}
