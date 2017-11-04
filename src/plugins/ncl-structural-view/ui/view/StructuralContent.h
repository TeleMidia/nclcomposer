#ifndef STRUCTURALCONTENT_H
#define STRUCTURALCONTENT_H

#include "StructuralNode.h"

class StructuralContent : public StructuralNode
{
public:
  StructuralContent (StructuralEntity *structuralParent = 0);
  virtual ~StructuralContent () {}

  StructuralMimeType getMimeType () const;
  void setMimeType (StructuralMimeType mimetype);

  virtual void adjust (bool collision = false, bool recursion = true);

protected:
  virtual void draw (QPainter *painter);

private:
  StructuralMimeType _mimetype;
};

#endif // STRUCTURALCONTENT_H
