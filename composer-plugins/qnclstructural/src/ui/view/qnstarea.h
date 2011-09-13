#ifndef QNSTAREA_H
#define QNSTAREA_H

#include "qnstinterface.h"

class QnstArea : public QnstInterface
{
public:
    QnstArea(QnstNode* parent = 0);

    ~QnstArea();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;
};

#endif // QNSTAREA_H
