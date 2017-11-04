#ifndef STRUCTURALSEDGE_H
#define STRUCTURALSEDGE_H

#define PI 3.14159265

#include "StructuralEntity.h"

class StructuralEdge : public StructuralEntity
{
public:
  StructuralEdge (StructuralEntity *parent = 0);
  virtual ~StructuralEdge () {}

  qreal getAngle () const;
  void setAngle (qreal angle);

  StructuralEntity *getOrigin () const;
  void setOrigin (StructuralEntity *orig);
  bool hasOrigin () const;

  StructuralEntity *getDestination () const;
  void setDestination (StructuralEntity *dest);
  bool hasDestination () const;

  virtual void adjust (bool collision = false, bool recursion = true);

protected:
  qreal getAlfa () const;
  void setAlfa (qreal alfa);

  qreal getOrigTop () const;
  void setOrigTop (qreal tailTop);

  qreal getOrigLeft () const;
  void setOrigLeft (qreal tailLeft);

  qreal getDestTop () const;
  void setDestTop (qreal destTop);

  qreal getDestLeft () const;
  void setDestLeft (qreal headLeft);

  void adjustBox (QLineF line);
  void adjustExtreme (StructuralEntity *extreme, QLineF line, qreal index,
                      qreal step, qreal angle);

  QLineF getDrawLine (qreal padding) const;

  virtual void draw (QPainter *painter);
  virtual void delineate (QPainterPath *painter) const;

private:
  qreal _alfa, _angle;

  StructuralEntity *_orig;
  qreal _origTop, _origLeft;

  StructuralEntity *_dest;
  qreal _destTop, _destLeft;
};

#endif // STRUCTURALSEDGE_H
