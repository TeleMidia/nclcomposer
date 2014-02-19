#ifndef QNSTGRAPHICSINTERFACE_H
#define QNSTGRAPHICSINTERFACE_H

#include "StructuralGraphicsEntity.h"
#include "StructuralGraphicsEdge.h"

class StructuralGraphicsInterface : public QnstGraphicsEntityWithEdges
{
public:
  StructuralGraphicsInterface(StructuralGraphicsEntity* parent = 0);

  ~StructuralGraphicsInterface();

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
