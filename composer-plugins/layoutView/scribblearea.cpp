#include "scribblearea.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <math.h>
#include <iostream>

ScribbleArea::ScribbleArea (QWidget *parent)
        : QWidget (parent)
{

    setAttribute(Qt::WA_StaticContents);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    myPenColor = Qt::darkRed;
    clear();
    resize (400, 400);

    count = 0;
    init ();
}

void ScribbleArea::init()
{
    accept = false;
    currentRect = NULL;
    clicked = false;
    selectedRegion = NULL;
    resizeLeft = false;
    resizeRigth = false;
    resizeBottom = false;
    resizeTop = false;
    oldWidth = 0;
    oldHeight = 0;
}

void ScribbleArea::setPenColor(QColor color)
{
    myPenColor = color;
}

void ScribbleArea::resize (int w, int h)
{
    width = w;
    height = h;
    QImage newImage(QSize(w, h), QImage::Format_RGB32);
    newImage.fill(qRgb(192,192,192));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), background);
    background = newImage;
}

void ScribbleArea::clear()
{
    resize (width, height);
    background.fill(qRgb(192,192,192));
    list.clear();
    currentRect = NULL;
    update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    lastPointClicked = event->pos();
    if (event->buttons() == Qt::LeftButton){
        for (int i= list.size() - 1; i >= 0 ; i--) {
            if (list[i]->contains(lastPointClicked)) {
              if (accept) {
                    QString s = "rg";
                    QString a;
                    a.setNum(count);
                    s.append(a);
                    currentRect = new NCLRegion (s,  width, height, list[i]->getChildrenWhithPoint(lastPointClicked));
                    currentRect->setLeft(lastPointClicked.x());
                    currentRect->setTop(lastPointClicked.y());
                    return;
                }

              currentRect = list[i]->getChildrenWhithPoint(lastPointClicked);
                if ( labs (lastPointClicked.x() - currentRect->left() ) <= TOL)
                    resizeLeft = true;
                else if (labs( lastPointClicked.x() - (currentRect->left() + currentRect->width())) <= TOL) {
                    resizeRigth = true;

                }
                else if (labs (lastPointClicked.y() - currentRect->top() ) <= TOL)
                    resizeTop = true;
                else if (labs (lastPointClicked.y() - (currentRect->top() + currentRect->height())) <= TOL) {

                    resizeBottom = true;
                }
                
                oldTop = currentRect->top();
                oldLeft = currentRect->left();

                oldWidth = currentRect->width();
                oldHeight = currentRect->height();

                clicked = true;
                currentRect->notifyChildren(0, 0, true);


                emit regionSelected(currentRect);
                update();
                return;
            }
        }

        if (accept) {
            QString s = "rg";
            QString a;
            a.setNum(count);
            s.append(a);
            currentRect = new NCLRegion (s, width, height);
            currentRect->setLeft(lastPointClicked.x());
            currentRect->setTop(lastPointClicked.y());
            return;
        }

        init();
        emit regionSelected(currentRect);
        update();
    }

}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::RightButton || currentRect == NULL) return;

    QPoint point = event->pos();
    int x = point.x() - lastPointClicked.x();
    int y = point.y() - lastPointClicked.y();

    NCLRegion *parent = currentRect->getParent();
    if (clicked){
        if (resizeLeft) {
            int left = point.x();
            if (parent != NULL) {
                if (left < parent->left())
                    left = parent->left();
                else if (left > parent->width() + parent->left())
                    left = parent->width() + parent->left();

                if (left > oldLeft + oldWidth)
                    left = oldLeft + oldWidth;

            }
            else {
                left = left < 0 ? 0 : left;
                left = left > oldLeft + oldWidth ? oldLeft + oldWidth : left;
            }
            currentRect->setLeft(left);
        }
        else if (resizeRigth){
            int width = oldWidth + x;
            if (parent != NULL) {
                if (width + currentRect->left() > parent->left() + parent->width())
                    width = parent->left() + parent->width() - currentRect->left();
                else if (width + currentRect->left() < parent->left())
                    width = parent->left() - currentRect->left();
            }
            width = width < 0 ? 0 : width;
            width = currentRect->left() + width > background.width() ?
                    width - (currentRect->left() + width - background.width()) : width;
            currentRect->setWidth(width);

        }
        else if (resizeTop) {
            int top = point.y();
            if (parent != NULL) {
                if (top < parent->top())
                    top = parent->top();
                else if (top > parent->height() + parent->top())
                    top = parent->height() + parent->top();

                if (top > oldTop + oldHeight)
                    top = oldTop + oldHeight;
            }
            else {
                top = top < 0 ? 0 : top;
                top = top > oldTop + oldHeight ? oldTop + oldHeight : top;
            }

            currentRect->setTop(top);
        }
        else if (resizeBottom){
            int height = oldHeight + y;
            if (parent != NULL){
                if (height + currentRect->top() > parent->top() + parent->height())
                    height = parent->top() + parent->height() - currentRect->top();
            else if (height + currentRect->top() < parent->top())
                    height = parent->top() - currentRect->top();
            }
            height = height < 0 ? 0 : height;
            height = currentRect->top() + height > background.height () ?
                    height - (currentRect->top() + height - background.height()) : height;

            currentRect->setHeight(height);
        }
        else {
           // if (!currentRect->contains(event->pos())) return;

            int dx = oldLeft + x;
            int dy = oldTop + y;

            int modX;
            int modY;

            if (parent!=NULL){
                if (dx < parent->left()) {
                    modX = parent->left() - dx;
                    dx = parent->left();
                    currentRect->setWidth( oldWidth - modX);
                  }

                if (dy < parent->top()) {
                    modY = parent->top() - dy;
                    dy = parent->top();
                    currentRect->setHeight ( oldHeight - modY);
                }

                if (oldWidth + dx > parent->left() + parent->width()) {
                    modX = ( dx  + oldWidth) - (parent->left() + parent->width());
                    currentRect->setWidth  ( oldWidth - modX);
                }

                if (oldHeight + dy > parent->top() + parent->height()){
                    modY = (dy + oldHeight) - (parent->top() + parent->height());
                    currentRect->setHeight ( oldHeight - modY);
                }

                if (currentRect->width() + dx < parent->left()){
                    currentRect->setWidth(0);
                }

                if (currentRect->height() + dy < parent->top()){
                    currentRect->setHeight(0);
                }

                if ( dy >  parent->top() + parent->height()){
                   dy = parent->top() + parent->height();
                   currentRect->setHeight(0);
               }

               if ( dx >  parent->left() + parent->width()){
                   dx = parent->left() + parent->width();
                   currentRect->setWidth(0);
               }

            }

            else {
                if (dx < 0) {
                    modX = - dx;
                    dx = 0;
                    currentRect->setWidth( oldWidth - modX);
                  }

                if (dy < 0) {
                    modY = - dy;
                    dy = 0;
                    currentRect->setHeight ( oldHeight - modY);
                }

                if (oldWidth + dx > background.width()) {
                    modX = ( dx  + oldWidth) - background.width();
                    currentRect->setWidth  ( oldWidth - modX);
                }

                if (oldHeight + dy > background.height()){
                    modY = (dy + oldHeight) - background.height();
                    currentRect->setHeight ( oldHeight - modY);
                }

                if (currentRect->width() + dx < 0){
                    currentRect->setWidth(0);
                }

                if (currentRect->height() + dy < 0){
                    currentRect->setHeight(0);
                }

                if ( dy >  background.height()){
                   dy = background.height();
                   currentRect->setHeight(0);
               }

               if ( dx >  background.width()){
                   dx = background.width();
                   currentRect->setWidth(0);
               }

            }


            currentRect->setRect(dx, dy,
                                 currentRect->width(), currentRect->height());



            currentRect->notifyChildren(x, y, false);
        }
        currentRect->adjustChildren();
        update();

    }
    else if (accept){
        int dx = x;
        int dy = y;
        int modX = 0;
        int modY = 0;

        if (parent != NULL) {

            if ( currentRect->top() + dy > parent->top() + parent->height())
                modY = currentRect->top() + dy - (parent->top() + parent->height());

            if ( currentRect->left() + dx > parent->left() + parent->width() )
                modX = currentRect->left() + dx - (parent->left() + parent->width());

            if ( currentRect->top() + dy < parent->top())
                modY = currentRect->top() + dy - parent->top();

            if ( currentRect->left() + dx < parent->left())
                modX = currentRect->left() + dx - parent->left();

        }

        int width = dx - modX;
        width = width < 0 ? 0 : width;
        width -= currentRect->left() + width > background.width() ?
                 currentRect->left() + width  - background.width() : 0;

        int height = dy - modY;
        height = height < 0 ? 0 : height;
        height -= currentRect->top() + height > background.height() ?
                  currentRect->top() + height - background.height() : 0;

        currentRect->setWidth(width);
        currentRect->setHeight(height);
    }
    update();
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    if (event->pos() == lastPointClicked) return;
    if (currentRect!= NULL)
        if (!clicked){
            list.append(currentRect);
            emit regionAdded(currentRect);
            emit regionSelected(currentRect);
        }
        else
            emit regionChanged(currentRect);

    init();
    update();
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter (this);
    QRect r;
    painter.drawImage(QPoint (0,0), background);
    painter.setPen(QPen(Qt::SolidLine));
    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));

    for (int i=0; i < list.size(); i++) {
        painter.drawRect(*list[i]);
        if (list[i]->width() > 20 && list[i]->height() > 15)
            painter.drawText( list[i]->left() + 10, list[i]->top() + 15, list[i]->getId());
    }
    paintSelectedRegion(); 
}

void ScribbleArea::paintSelectedRegion()
{
    QPainter painter (this);
    NCLRegion r ("temp");

    if (currentRect == NULL) return;
    r = *currentRect;
    QPen pen (Qt::DashDotLine);
    pen.setWidth(3);
    painter.setPen (pen);
    painter.drawRect(*currentRect);

    NCLRegion *parent = r.getParent();
    int minW = 0;
    int minH = 0;
    if (parent != NULL){
        minW = parent->left();
        minH = parent->top();
    }

    if (r.left() != minW && r.top() != minH && r.height() > 15 && r.width()>20)
    painter.drawText( r.left() + 10, r.top() + 15, currentRect->getId());
}


void ScribbleArea::setSelectedList(NCLRegion * regions)
{
    currentRect = regions;
    update();
}

void ScribbleArea::modify(QString id, QString attribute, int value)
{
    for (int i=0; i<list.size(); i++)
        if (list[i]->getId() == id){
            if (attribute == "left")
                list[i]->setLeft(value);
        }
    update();
}

void ScribbleArea::Delete(NCLRegion *deletedRegion)
{
    if (deletedRegion == NULL) return;
    QList <NCLRegion *> aux = deletedRegion->getChildren();
    for (int i=0; i<aux.size(); i++)
        Delete(aux[i]);

    deletedRegion->clearChildrenList();

    NCLRegion *parent = deletedRegion->getParent();
    if (parent) parent->removeChild(deletedRegion);

    list.removeAll(deletedRegion);

    free (deletedRegion);

    emit regionSelected(NULL);
    init();
    update();
    return;
}
