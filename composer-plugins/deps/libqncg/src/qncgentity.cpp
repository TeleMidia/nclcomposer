#include "qncgentity.h"

QncgEntity::QncgEntity(QncgEntity* parent)
    : QGraphicsItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    setLinkable(true);
    setMoveable(true);
    setResizable(true);
    setFocusable(true);
    setSelectable(true);

    setMoving(false);
    setInvalid(false);
    setLinking(false);
    setResizing(false);
    setSelected(false);
}

QncgEntity::~QncgEntity()
{

}

void QncgEntity::setMoveable(bool moveable)
{
    this->moveable = moveable;
}

bool QncgEntity::isMoveable() const
{
    return moveable;
}

void QncgEntity::setSelectable(bool selected)
{
    this->selectable = selected;
}

bool QncgEntity::isSelectable() const
{
    return selectable;
}

void QncgEntity::setFocusable(bool focusable)
{
    this->focusable = focusable;
}

bool QncgEntity::isFocusable() const
{
    return focusable;
}

void QncgEntity::setResizable(bool resizable)
{
    this->resizable = resizable;
}

bool QncgEntity::isResizable() const
{
    return resizable;
}

void QncgEntity::setLinkable(bool linkable)
{
    this->linkable = linkable;
}

bool QncgEntity::isLinkable() const
{
    return linkable;
}

void QncgEntity::setMoving(bool moving)
{
    this->moving = moving;
}

bool QncgEntity::isMoving() const
{
    return moving;
}

void QncgEntity::setLinking(bool linking)
{
    this->linking = linking;
}

bool QncgEntity::isLinking() const
{
    return linking;
}

void QncgEntity::setResizing(bool resizing)
{
    this->resizing = resizing;
}

bool QncgEntity::isResizing() const
{
    return resizing;
}

void QncgEntity::setInvalid(bool invalid)
{
    this->invalid = invalid;
}

bool QncgEntity::isInvalid() const
{
    return invalid;
}

void QncgEntity::setSelected(bool selected)
{
    this->selected = selected;
}

bool QncgEntity::isSelected() const
{
    return selected;
}

qreal QncgEntity::getTop() const
{
    return top;
}

void QncgEntity::setTop(qreal top)
{
    this->top = top;

    setY(top-4);
}

qreal QncgEntity::getMoveTop() const
{
    return moveTop;
}

void QncgEntity::setMoveTop(qreal moveTop)
{
    this->moveTop = moveTop;
}

qreal QncgEntity::getPressTop() const
{
    return pressTop;
}

void QncgEntity::setPressTop(qreal pressTop)
{
    this->pressTop = pressTop;
}

qreal QncgEntity::getResizeTop() const
{
    return resizeTop;
}

void QncgEntity::setResizeTop(qreal resizeTop)
{
    this->resizeTop = resizeTop;
}

qreal QncgEntity::getRelativeTop() const
{
    return relativeTop;
}

void QncgEntity::setRelativeTop(qreal relativeTop)
{
    this->relativeTop = relativeTop;
}

qreal QncgEntity::getLeft() const
{
    return left;
}

void QncgEntity::setLeft(qreal left)
{
    this->left = left;

    setX(left-4);
}

qreal QncgEntity::getMoveLeft() const
{
    return moveLeft;
}

void QncgEntity::setMoveLeft(qreal moveLeft)
{
    this->moveLeft = moveLeft;
}

qreal QncgEntity::getPressLeft() const
{
    return pressLeft;
}

void QncgEntity::setPressLeft(qreal pressLeft)
{
    this->pressLeft = pressLeft;
}

qreal QncgEntity::getResizeLeft() const
{
    return resizeLeft;
}

void QncgEntity::setResizeLeft(qreal resizeLeft)
{
    this->resizeLeft = resizeLeft;
}

qreal QncgEntity::getRelativeLeft() const
{
    return relativeLeft;
}

void QncgEntity::setRelativeLeft(qreal relativeLeft)
{
    this->relativeLeft = relativeLeft;
}

qreal QncgEntity::getRight() const
{
    return right;
}

void QncgEntity::setRight(qreal right)
{
    this->right = right;
}

qreal QncgEntity::getRelativeRight() const
{
    return relativeRight;
}

void QncgEntity::setRelativeRight(qreal relativeRight)
{
    this->relativeRight = relativeRight;
}

qreal QncgEntity::getBottom() const
{
    return bottom;
}

void QncgEntity::setBottom(qreal bottom)
{
    this->bottom = bottom;
}

qreal QncgEntity::getRelativeBottom() const
{
    return relativeBottom;
}

void QncgEntity::setRelativeBottom(qreal relativeBottom)
{
    this->relativeBottom = relativeBottom;
}

qreal QncgEntity::getWidth() const
{
    return width;
}

void QncgEntity::setWidth(qreal width)
{
    this->width = width;
}

qreal QncgEntity::getPressWidth() const
{
    return pressWidth;
}

void QncgEntity::setPressWidth(qreal pressWidth)
{
    this->pressWidth = pressWidth;
}

qreal QncgEntity::getResizeWidth() const
{
    return resizeWidth;
}

void QncgEntity::setResizeWidth(qreal resizeWidth)
{
    this->resizeWidth = resizeWidth;
}

qreal QncgEntity::getRelativeWidth() const
{
    return relativeWidth;
}

void QncgEntity::setRelativeWidth(qreal relativeWidth)
{
    this->relativeWidth = relativeWidth;
}

int QncgEntity::getEntityType() const
{
    return entityType;
}

void QncgEntity::setEntityType(int entityType)
{
    this->entityType = entityType;
}

qreal QncgEntity::getHeight() const
{
    return height;
}

void QncgEntity::setHeight(qreal height)
{
    this->height = height;
}

qreal QncgEntity::getPressHeight() const
{
    return pressHeight;
}

void QncgEntity::setPressHeight(qreal pressHeight)
{
    this->pressHeight = pressHeight;
}

qreal QncgEntity::getResizeHeight() const
{
    return resizeHeight;
}

void QncgEntity::setResizeHeight(qreal resizeHeight)
{
    this->resizeHeight = resizeHeight;
}

qreal QncgEntity::getRelativeHeight() const
{
   return relativeHeight;
}

void QncgEntity::setRelativeHeight(qreal relativeHeight)
{
     this->relativeHeight = relativeHeight;
}

int QncgEntity::getResizeType() const
{
    return resizeType;
}

void QncgEntity::setResizeType(int resizeType)
{
    this->resizeType = resizeType;
}

QPainterPath QncgEntity::shape() const
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

QRectF QncgEntity::boundingRect() const
{
    QRectF bounds;

    bounds.setX(0);
    bounds.setY(0);
    bounds.setWidth(width+8);
    bounds.setHeight(height+8);

    return bounds;
}

void QncgEntity::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing,true);

    draw(painter);

    if (selectable && selected && resizable){
        painter->setRenderHint(QPainter::Antialiasing,false);

        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0, Qt::DashLine));
        painter->drawRect(4, 4, getWidth()-1, getHeight()-1);

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

void QncgEntity::keyPressEvent(QKeyEvent *event)
{
    QGraphicsItem::keyPressEvent(event);
}

void QncgEntity::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsItem::keyReleaseEvent(event);
}

void QncgEntity::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (moveable && moving){
        move(event);

    }else if (resizable && resizing){
        resize(event);

    }else if (linkable && linking){

        sceneEdge->setLine(QLineF(sceneEdge->line().p1(), event->scenePos()));

        scene()->update();
    }

    // QGraphicsItem::mouseMoveEvent(event);
}

void QncgEntity::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->button() == Qt::RightButton){
        event->ignore();

    }else if (event->button() == Qt::LeftButton){
        if (linking){

            sceneEdge = new QGraphicsLineItem();
            sceneEdge->setLine(QLineF(event->scenePos(),event->scenePos()));

            scene()->addItem(sceneEdge);

        }else{
            if (selectable){
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
                    setResizeType(Qncg::TopLeft);
                    setResizing(true);

                // if over TOP resize region
                }else if (QRectF((width+8)/2 - 4,0,8,8).contains(event->pos())){
                    setResizeType(Qncg::Top);
                    setResizing(true);

                // if over TOPRIGHT resize region
                }else if (QRectF((width+8) - 8,0,8,8).contains(event->pos())){
                    setResizeType(Qncg::TopRight);
                    setResizing(true);

                // if over RIGHT resize region
                }else if (QRectF((width+8) - 8,(height+8)/2 - 4,8,8).contains(event->pos())){
                    setResizeType(Qncg::Right);
                    setResizing(true);

                // if over BOTTOMRIGHT resize region
                }else if (QRectF((width+8) - 8,(height+8) - 8,8,8).contains(event->pos())){
                    setResizeType(Qncg::BottomRight);
                    setResizing(true);

                // if over BOTTOM resize region
                }else if (QRectF((width+8)/2 - 4,(height+8) - 8,8,8).contains(event->pos())){
                    setResizeType(Qncg::Bottom);
                    setResizing(true);

                // if over BOTTOMLEFT resize region
                }else if (QRectF(0,(height+8) - 8,8,8).contains(event->pos())){
                    setResizeType(Qncg::BottomLeft);
                    setResizing(true);

                // if over LEFT resize region
                }else if (QRectF(0,(height+8)/2 - 4,8,8).contains(event->pos())){
                    setResizeType(Qncg::Left);
                    setResizing(true);

                // if not over any resize region
                }else if (moveable){
                    setMoving(true);
                }

            }else if (moveable){
                setMoving(true);
            }
        }
    }

    QGraphicsItem::mousePressEvent(event);
}

void QncgEntity::mouseReleaseEvent(QGraphicsSceneMouseEvent*event)
{
    if (linking){


        link(event);

        delete(sceneEdge);

        sceneEdge = NULL;

    }else{
        if (moving){
            setMoving(false);

            if ((top != moveTop || left != moveLeft)){
                setTop(moveTop);
                setLeft(moveLeft);
    /*
                if (parentItem() != NULL){
                    QnlyGraphicsRegion* item = (QnlyGraphicsRegion*) parentItem();

                    setRelativeTop((top-4)/item->getHeight());
                    setRelativeLeft((left-4)/item->getWidth());

                    setRelativeWidth(width/item->getWidth());
                    setRelativeHeight(height/item->getHeight());

                    setRelativeRight(1 - (relativeLeft+relativeWidth));
                    setRelativeBottom(1 - (relativeTop+relativeHeight));

                }else{
                    setRelativeTop(top/scene()->height());
                    setRelativeLeft(left/scene()->width());

                    setRelativeWidth(width/scene()->width());
                    setRelativeHeight(height/scene()->height());

                    setRelativeRight(1 - (relativeLeft+relativeWidth));
                    setRelativeBottom(1 - (relativeTop+relativeHeight));
                }
    */
            }
        }

        if (resizing){
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

                setTop(resizeTop);
                setLeft(resizeLeft);
                setWidth(resizeWidth);
                setHeight(resizeHeight);
    /*
                if (parentItem() != NULL){
                    QnlyGraphicsRegion* item = (QnlyGraphicsRegion*) parentItem();

                    setRelativeTop((top-4)/item->getHeight());
                    setRelativeLeft((left-4)/item->getWidth());
                    setRelativeWidth(width/item->getWidth());
                    setRelativeHeight(height/item->getHeight());

                    setRelativeRight(1 - (relativeLeft+relativeWidth));
                    setRelativeBottom(1 - (relativeTop+relativeHeight));

                }else{
                    setRelativeTop(top/scene()->height());
                    setRelativeLeft(left/scene()->width());
                    setRelativeWidth(width/scene()->width());
                    setRelativeHeight(height/scene()->height());

                    setRelativeRight(1 - (relativeLeft+relativeWidth));
                    setRelativeBottom(1 - (relativeTop+relativeHeight));
                }
    */

            }
        }



    //    scene()->update();
        adjust();

        foreach (QGraphicsItem* item, childItems()){
            ((QncgEntity*) item)->adjust();
        }
    }




    QGraphicsItem::mouseReleaseEvent(event);
}

QGraphicsLineItem* QncgEntity::getSceneEdge() const
{
    return sceneEdge;
}
