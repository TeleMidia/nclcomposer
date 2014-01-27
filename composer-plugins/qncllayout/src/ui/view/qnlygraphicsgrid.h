#ifndef QNLYGRAPHICSGRID_H
#define QNLYGRAPHICSGRID_H

#include <QGraphicsRectItem>
#include <QPainter>
#include <QBrush>
#include <QPen>

class QnlyGraphicsGrid : public QGraphicsRectItem
{
public:
  explicit QnlyGraphicsGrid( QGraphicsItem* parent = 0,
                             QGraphicsScene* scene = 0 );
  ~QnlyGraphicsGrid();

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
