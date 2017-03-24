#ifndef QNLYGRAPHICSGRID_H
#define QNLYGRAPHICSGRID_H

#include <QGraphicsRectItem>
#include <QPainter>
#include <QBrush>
#include <QPen>

class LayoutGrid : public QGraphicsRectItem
{
public:
  explicit LayoutGrid( QGraphicsItem* parent = 0,
                       QGraphicsScene* scene = 0 );
  virtual ~LayoutGrid();

  void setStep(qreal step);
  qreal getStep();

  void setPen(QPen pen);
  QPen getPen();

protected:
  virtual void paint( QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget);
private:
  qreal _step;
  QPen _pen;

};

#endif // QNLYGRAPHICSGRID_H
