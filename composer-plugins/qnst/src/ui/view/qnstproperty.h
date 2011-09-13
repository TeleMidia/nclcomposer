#ifndef QNSTPROPERTY_H
#define QNSTPROPERTY_H

#include "qnstinterface.h"

class QnstProperty : public QnstInterface
{
public:
    QnstProperty(QnstNode* parent = 0);

    ~QnstProperty();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;
};

#endif // QNSTPROPERTY_H
