#ifndef QNSTGRAPHICSREFERENCE_H
#define QNSTGRAPHICSREFERENCE_H

#include "qnstgraphicsedge.h"

class QnstGraphicsReference : public QnstGraphicsEdge
{
public:
  QnstGraphicsReference(QnstGraphicsEntity* parent = 0);

  virtual ~QnstGraphicsReference();

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;
};

#endif // QNSTGRAPHICSREFERENCE_H
