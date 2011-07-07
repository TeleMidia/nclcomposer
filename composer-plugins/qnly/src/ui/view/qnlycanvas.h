#ifndef QNLYCANVAS_H
#define QNLYCANVAS_H

#include <QGraphicsView>
#include <QResizeEvent>

#include "qnlygraphicsregion.h"

class QnlyCanvas : public QGraphicsView
{
public:
    QnlyCanvas(QWidget* parent = 0);

    ~QnlyCanvas();

protected:
    virtual void resizeEvent(QResizeEvent* event);
};

#endif // QNLYCANVAS_H
