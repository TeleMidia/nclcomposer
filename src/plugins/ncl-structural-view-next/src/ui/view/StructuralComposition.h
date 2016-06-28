#ifndef STRUCTURALCOMPOSITION_H
#define STRUCTURALCOMPOSITION_H

#include "Structural.h"
#include "StructuralNode.h"

#include <cmath>

#include <QGraphicsSceneMouseEvent>

// Spring Algoritms Params
#define SPRING_LENGTH 125
#define SPRING_CONSTANT 0.1
#define SPRING_DAMPING 0.5

#define SPRING_INTERATION 50

class StructuralComposition : public StructuralNode
{
  Q_OBJECT

public:
  StructuralComposition(StructuralEntity* parent = 0);

  ~StructuralComposition();

  virtual void setStructuralType(const StructuralType _subtype);
  virtual void setStructuralProperty(const QString &name, const QString &value);

  QString getColor() const;

  void setColor(QString color);

  qreal getLastW();

  qreal getLastH();

  void setLastW(qreal lastW);

  void setLastH(qreal lastH);

  void collapse(bool notify = false);

  virtual void refresh();

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

  QString color;

  qreal lastw;

  qreal lasth;

  StructuralComposition* tmp;
};

#endif // STRUCTURALCOMPOSITION_H
