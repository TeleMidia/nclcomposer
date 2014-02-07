#ifndef QNLYGRAPHICSGRID_H
#define QNLYGRAPHICSGRID_H

#include <QGraphicsRectItem>
#include <QPainter>
#include <QBrush>
#include <QPen>

class Grid : public QGraphicsRectItem
{
public:
  explicit Grid( QGraphicsItem* parent = 0,
                             QGraphicsScene* scene = 0 );
  virtual ~Grid();

  void setStep(qreal step);
  qreal getStep();

  void setPen(QPen pen);
  QPen getPen();

protected:
  virtual void paint( QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget);
private:
  qreal step;
  QPen pen;

};

#endif // QNLYGRAPHICSGRID_H
