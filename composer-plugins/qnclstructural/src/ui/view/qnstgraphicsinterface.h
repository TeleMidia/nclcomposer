#ifndef QNSTGRAPHICSINTERFACE_H
#define QNSTGRAPHICSINTERFACE_H

#include "qnstgraphicsentity.h"
#include "qnstgraphicsedge.h"

class QnstGraphicsInterface : public QnstGraphicsEntityWithEdges
{
public:
  QnstGraphicsInterface(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsInterface();

  virtual void adjust(bool avoidCollision = true);

  bool isRefer();

  void setRefer(bool refer);

  virtual void updateToolTip();

  virtual void setProperties(const QMap<QString, QString> &props);

protected:
  virtual void draw(QPainter* painter) = 0;

  virtual void delineate(QPainterPath* painter) const = 0;

  virtual void move(QGraphicsSceneMouseEvent* event);

  virtual void resize(QGraphicsSceneMouseEvent* event);

private:
  void adjustToBorder();

  bool _isRefer;
};

#endif // QNSTGRAPHICSINTERFACE_H
