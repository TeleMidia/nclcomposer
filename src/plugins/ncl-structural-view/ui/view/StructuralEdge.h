#ifndef STRUCTURALSEDGE_H
#define STRUCTURALSEDGE_H

#define PI 3.14159265

#include "StructuralEntity.h"

class StructuralEdge : public StructuralEntity
{
public:
  StructuralEdge (StructuralEntity *parent = 0);
  virtual ~StructuralEdge ();

  qreal getAngle () const;
  void setAngle (qreal angle);

  StructuralEntity *getTail () const;
  void setTail (StructuralEntity *tail);
  bool hasTail () const;

  StructuralEntity *getHead () const;
  void setHead (StructuralEntity *head);
  bool hasHead () const;

  virtual void adjust (bool collision = false, bool recursion = true);

protected:
  qreal getAlfa () const;
  void setAlfa (qreal alfa);

  qreal getTailTop () const;
  void setTailTop (qreal tailTop);

  qreal getTailLeft () const;
  void setTailLeft (qreal tailLeft);

  qreal getHeadTop () const;
  void setHeadTop (qreal headTop);

  qreal getHeadLeft () const;
  void setHeadLeft (qreal headLeft);

  void adjustBox (QLineF line);
  void adjustExtreme (StructuralEntity *extreme, QLineF line, qreal index,
                      qreal step, qreal angle);

  QLineF getDrawLine (qreal padding) const;

  virtual void draw (QPainter *painter);
  virtual void delineate (QPainterPath *painter) const;

private:
  qreal _alfa;
  qreal _angle;

  qreal _tailTop;
  qreal _tailLeft;

  qreal _headTop;
  qreal _headLeft;

  StructuralEntity *_tail;
  StructuralEntity *_head;
};

#endif // STRUCTURALSEDGE_H
