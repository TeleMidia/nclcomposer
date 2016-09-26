#include "StructuralLinkTool.h"

StructuralLinkTool::StructuralLinkTool(StructuralEntity* parent)
  : StructuralEdge(parent)
{

}

StructuralLinkTool::~StructuralLinkTool()
{

}

QLineF StructuralLinkTool::getLine() const
{
  return _line;
}

void StructuralLinkTool::setLine(const QLineF &line)
{
  _line = line; adjustBox(line);
}
