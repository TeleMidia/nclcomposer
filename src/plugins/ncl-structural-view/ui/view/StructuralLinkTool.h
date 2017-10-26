#ifndef STRUCTURALLINKTOOL_H
#define STRUCTURALLINKTOOL_H

#include "StructuralEdge.h"

class StructuralLinkTool : public StructuralEdge
{
public:
  StructuralLinkTool (StructuralEntity *parent = 0);
  virtual ~StructuralLinkTool ();

  QLineF getLine () const;
  void setLine (const QLineF &_line);

private:
  QLineF _line;
};

#endif // STRUCTURALLINKTOOL_H
