#ifndef QNSTGRAPHICSAREA_H
#define QNSTGRAPHICSAREA_H

#include "qnstgraphicsinterface.h"

class QnstGraphicsArea : public QnstGraphicsInterface
{
public:
  QnstGraphicsArea(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsArea();

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

};

#endif // QNSTGRAPHICSAREA_H
