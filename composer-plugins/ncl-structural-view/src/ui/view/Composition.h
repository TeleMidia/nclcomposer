#ifndef QNSTGRAPHICSCOMPOSITION_H
#define QNSTGRAPHICSCOMPOSITION_H

#include "Globals.h"
#include "Node.h"

#include <cmath>

#include <QGraphicsSceneMouseEvent>

// Spring Algoritms Params
#define SPRING_LENGTH 125
#define SPRING_CONSTANT 0.1
#define SPRING_DAMPING 0.5

#define SPRING_INTERATION 50

class QnstComposition : public QnstNode
{
  Q_OBJECT

public:
  QnstComposition(QnstEntity* parent = 0);

  ~QnstComposition();

  virtual void setnstSubtype(const QnstSubtype subtype);

  QString getColor() const;

  void setColor(QString color);

  void setCollapsed(bool collapsed);

  qreal getLastW();

  qreal getLastH();

  void setLastW(qreal lastW);

  void setLastH(qreal lastH);

  bool isCollapsed();

  void collapse();

  virtual void updateToolTip();

private:
  void createObjects();

  void createConnections();

protected slots:
  void adjustWithSpring();

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

  virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

  virtual void dropEvent(QGraphicsSceneDragDropEvent *event);

  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
  bool collapsed;

  QString color;

  qreal lastw;

  qreal lasth;

  QnstNode* tmp;
};

#endif // QNSTGRAPHICSCOMPOSITION_H
