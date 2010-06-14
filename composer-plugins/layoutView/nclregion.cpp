#include "nclregion.h"
#include <QApplication>
#include <QDesktopWidget>
#include <iostream>


NCLRegion::NCLRegion (QString id, NCLRegion *parent)
        : QRect ()
{
    scrennResolution = QApplication::desktop()->screenGeometry();
    this->parent = parent;
    if (parent != NULL){
        parent->setChild(this);
    }
    Id = id;
    Top = 0;
    Left = 0;
    Width = 0;
    Height = 0;
    Right  = -1;
    Bottom = -1;
    Title = "title";
    ZIndex = 0;

    relativeTop = 0;
    relativeLeft = 0;
    
}

NCLRegion::NCLRegion (QString  id, int width, int height, NCLRegion *parent)
        : QRect ()
{
    relativeHeight = height;
    relativeWidth = width;
    relativeTop = 0;
    relativeLeft = 0;

    scrennResolution = QApplication::desktop()->screenGeometry();
    this->parent = parent;
    if (parent != NULL) {
        parent->setChild(this);
        relativeHeight = parent->height();
        relativeWidth = parent->width();
        relativeTop = parent->top();
        relativeLeft = parent->left();
    }
    Id = id;
    Top = 0;
    Left = 0;
    Width = 0;
    Height = 0;
    Right  = -1;
    Bottom = -1;
    Title = "title";
    ZIndex = 0;

    canvasHeight = height;
    canvasWidth = width;
}

void NCLRegion::setRelativeWidth(int limit)
{
    relativeWidth = limit;
}

void NCLRegion::setRelativeHeight (int limit)
{
    relativeHeight = limit;
}

void NCLRegion::setRelativeLeft (int limit)
{
    relativeLeft = limit;
}

void NCLRegion::setRelativeTop (int limit)
{
    relativeTop = limit;
}

void NCLRegion::notifyChildren(int desX, int desY, bool flag)
{
    for (int i=0; i<children.size(); i++){
        children[i]->modify(desX, desY, flag);
        children[i]->notifyChildren(desX, desY, flag);
    }
}


void NCLRegion::adjustChildren()
{
    for (int i=0; i < children.size(); i++)
        children[i]->autoAdjust ();
}

void NCLRegion::autoAdjust()
{
    adjustChildren();

    if (left() < parent->left()){
        int dx = parent->left() - left();
        adjust(dx, 0, dx, 0);
    }

    if (top() < parent->top()){
        int dy = parent->top() - top();
        adjust (0, dy, 0, dy);
    }

    if (left() + width() > parent->left() + parent->width()){
        int mod = parent->left() + parent->width() - left();
        if (mod < 0) {
            setHeight(0);
            setWidth(0);
        }
        else
            setWidth(mod);
    }

    if (top() + height() > parent->top() + parent->height()){
        int mod = parent->top() + parent->height() - top();
        if (mod < 0) {
            setHeight(0);
            setWidth(0);
        }
        else
            setHeight(mod);
    }
}

void NCLRegion::modify(int desX, int desY, bool flag)
{
    if (parent == NULL) return;
    if (flag) {
        oldLeft = left();
        oldTop = top();
        oldWidth = width();
        oldHeight = height();
    }

    int dx = oldLeft + desX;
    int dy = oldTop + desY;

    int modX;
    int modY;
    if (parent != NULL){

        if (dx < parent->left()) {
            modX = parent->left() - dx;
            dx = parent->left();
            setWidth( oldWidth - modX);
          }
        else if (oldWidth + dx > parent->left() + parent->width()) {
            modX = ( dx  + oldWidth) - (parent->left() + parent->width());
            setWidth  ( oldWidth - modX);
        }

        if (dy < parent->top()) {
            modY = parent->top() - dy;
            dy = parent->top();
            setHeight ( oldHeight - modY);
        }
        else if (oldHeight + dy > parent->top() + parent->height()){
            modY = (dy + oldHeight) - (parent->height() + parent->top());
            setHeight ( oldHeight - modY);
        }

        if (dx + oldWidth < parent->left()) {
            modX = parent->left() - (dx + oldWidth);
            setWidth(oldWidth - modX);
        }

        if (width() + dx < parent->left()){
            setWidth(0);
        }

        if (height() + dy < parent->top()){
            setHeight(0);
        }

        if ( dy >  parent->top() + parent->height()){
           dy = parent->top() + parent->height();
           setHeight(0);
       }

       if ( dx >  parent->left() + parent->width()){
           dx = parent->left() + parent->width();
           setWidth(0);
       }
    }

    setRect(dx, dy, width(), height());
}

NCLRegion * NCLRegion::getChildrenWhithPoint (QPoint point)
{
    NCLRegion * aux = NULL;
    for (int i=0; i< children.size(); i++)
        if (children[i]->contains(point)) {
            aux = children[i]->getChildrenWhithPoint(point);
            if (aux == NULL)
                return children[i];
            return aux;
        }
    return this;
}

void NCLRegion::setParent(NCLRegion *parent)
{
    this->parent = parent;
    for (int i=0; i<children.size(); i++)
        children[i]->setParent(this);
}

double NCLRegion::rightAdjusted() const
{
    return Right;
}

double NCLRegion::bottomAdjusted() const
{
    return Bottom;
}

int NCLRegion::zIndex() const
{
    return ZIndex;
}

double NCLRegion::topAdjusted() const
{
    double proporcao = (double) QRect::top()/canvasHeight;
    //return scrennResolution.height() * proporcao;
    return proporcao;
}

double NCLRegion::leftAdjusted() const
{
    double proporcao = (double) QRect::left()/canvasWidth;
    //return scrennResolution.width() * proporcao;
    return proporcao;
}

double NCLRegion::widthAdjusted() const
{
    double proporcao = (double) QRect::width()/canvasWidth;
    //return scrennResolution.width() * proporcao;
    return proporcao;
}

double NCLRegion::heightAdjusted() const
{
    double proporcao = (double) QRect::height()/canvasHeight;
    //return scrennResolution.height() * proporcao;
    return proporcao;
}

QString NCLRegion::title() const
{
    return Title;
}

void NCLRegion::clearChildrenList()
{
    for (int i=0; i < children.size(); i++)
        children[i]->clearChildrenList();
    children.clear();
}

