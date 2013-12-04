#ifndef QNSTGRAPHICSINTERFACE_H
#define QNSTGRAPHICSINTERFACE_H

#include "qnstentity.h"
#include "qnstedge.h"

class QnstInterface : public QnstEntityWithEdges
{
public:
  QnstInterface(QnstEntity* parent = 0);

  ~QnstInterface();

  virtual void setnstSubtype(const QnstSubtype subtype);

  virtual void adjust(bool avoidCollision = true);

  bool isRefer();

  void setRefer(bool refer);

  void setHexColor(QString hexColor);
  void setHexBorderColor(QString hexBorderColor);

  virtual void updateToolTip();

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

  virtual void move(QGraphicsSceneMouseEvent* event);

  virtual void resize(QGraphicsSceneMouseEvent* event);

private:
  void adjustToBorder();

  bool _isRefer;

  QString hexColor;
  QString hexBorderColor;
};

#endif // QNSTGRAPHICSINTERFACE_H
