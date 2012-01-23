#ifndef QNSTGRAPHICSCOMPOSITION_H
#define QNSTGRAPHICSCOMPOSITION_H

#include "qnstgraphicsnode.h"

class QnstGraphicsComposition : public QnstGraphicsNode
{
public:
    QnstGraphicsComposition(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsComposition();

    QString getColor() const;

    void setColor(QString color);

    void setnstId(QString id);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

private:
    QString color;
};

#endif // QNSTGRAPHICSCOMPOSITION_H
