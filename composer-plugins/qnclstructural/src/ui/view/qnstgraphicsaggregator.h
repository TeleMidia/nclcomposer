#ifndef QNSTGRAPHICSAGGREGATOR_H
#define QNSTGRAPHICSAGGREGATOR_H


#include "qnstgraphicscontent.h"

class QnstGraphicsAggregator : public QnstGraphicsContent
{
public:
    QnstGraphicsAggregator(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsAggregator();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

private:
    void createObjects();

    void createConnections();
};

#endif // QNSTGRAPHICSAGGREGATOR_H
