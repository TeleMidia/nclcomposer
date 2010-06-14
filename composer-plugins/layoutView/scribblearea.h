#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QWidget>
#include <QColor>
#include <QPoint>
#include <QImage>
#include <QSize>
#include <map>
#include "nclregion.h"
#define TOL 5

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea (QWidget *parent = 0);

    void setPenColor (QColor color);
    void resize (int w, int h);

    void setSelectedList (NCLRegion * regions);
    void modify (QString id, QString attribute, int value);

    QColor penColor () const { return myPenColor; }
    QSize sizeHint () const { return QSize(width, height); }

signals:
    void regionSelected (NCLRegion *region);
    void regionAdded (NCLRegion *region);
    void regionChanged (NCLRegion *region);

public slots:
    void clear ();
    void Delete (NCLRegion *);
    void Accept () { init();
                     this->accept = true;
                     setCursor(Qt::CrossCursor);
                     count++;
                     }
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);


private:
    void init ();
    void paintSelectedRegion ();

    QPoint  lastPointClicked;
    QList <NCLRegion *> list;
    NCLRegion *selectedRegion;
    NCLRegion *currentRect;
    bool clicked;
    QColor myPenColor;
    QImage background;
    QCursor *myCursor;
    int width;
    int height;
    int count;
    bool resizeLeft;
    bool resizeRigth;
    int oldWidth;
    bool resizeTop;
    bool resizeBottom;
    int oldHeight;
    bool accept;
    int oldTop;
    int oldLeft;

};

#endif // SCRIBBLEAREA_H
