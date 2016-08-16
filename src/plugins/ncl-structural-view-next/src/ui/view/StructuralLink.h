#ifndef QNSTGRAPHICSLINK_H
#define QNSTGRAPHICSLINK_H

#include "StructuralNode.h"
#include "StructuralBind.h"

class StructuralBind;

// \todo The qnstgraphicsaggregator and qnstlink must be part of this class.
class StructuralLink : public StructuralNode
{
  Q_OBJECT
public:
  StructuralLink(StructuralEntity* parent = 0);

  ~StructuralLink();

  virtual void refresh();

signals:
  void showLinkEditDialog(StructuralLink* entity);

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

  virtual void 	mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // QNSTGRAPHICSLINK_H
