#ifndef STRUCTURALCONTENT_H
#define STRUCTURALCONTENT_H

#include "StructuralNode.h"

class StructuralContent : public StructuralNode
{
public:
  StructuralContent(StructuralEntity* parent = 0);
  virtual ~StructuralContent();

  QString getLocation() const;
  void setLocation(const QString &_location);

  StructuralMimeType getMimeType() const;
  void setMimeType(StructuralMimeType mimetype);

  virtual void adjust(bool collision = false,  bool recursion = true);

protected:
  virtual void draw(QPainter* painter);

private:
  QString _location;

  StructuralMimeType _mimetype;
};

#endif // STRUCTURALCONTENT_H
