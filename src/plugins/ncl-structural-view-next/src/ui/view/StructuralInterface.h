#ifndef STRUCTURALINTERFACE_H
#define STRUCTURALINTERFACE_H

#include "StructuralEntity.h"
#include "StructuralEdge.h"

class StructuralInterface : public StructuralEntity
{
public:
  StructuralInterface(StructuralEntity* parent = 0);
  virtual ~StructuralInterface();

  virtual void adjust(bool avoidCollision = true,  bool rec = true);

protected:
  virtual void draw(QPainter* painter);
  virtual void delineate(QPainterPath* painter) const;
  virtual void move(QGraphicsSceneMouseEvent* event);
  virtual void resize(QGraphicsSceneMouseEvent* event);

private:
  void adjustToBorder();

  QPixmap icon;
};

#endif // STRUCTURALINTERFACE_H
