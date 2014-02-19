#ifndef QNSTGRAPHICSBODY_H
#define QNSTGRAPHICSBODY_H

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QFileInfo>

#include "StructuralGraphicsComposition.h"

#include "StructuralGraphicsContext.h"
#include "StructuralGraphicsSwitch.h"
#include "StructuralGraphicsPort.h"

class StructuralGraphicsBody : public StructuralGraphicsComposition
{
  Q_OBJECT

public:
  StructuralGraphicsBody(StructuralGraphicsEntity* parent = 0);

  ~StructuralGraphicsBody();

private:
  void createObjects();

  void createConnections();
};

#endif // QNSTGRAPHICSBODY_H
