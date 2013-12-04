#ifndef QNSTGRAPHICSREFERENCE_H
#define QNSTGRAPHICSREFERENCE_H

#include "qnstedge.h"

class QnstReference : public QnstEdge
{
public:
  QnstReference(QnstEntity* parent = 0);

  ~QnstReference();

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;
};

#endif // QNSTGRAPHICSREFERENCE_H
