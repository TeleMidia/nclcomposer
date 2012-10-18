#ifndef QNSTGRAPHICSBODY_H
#define QNSTGRAPHICSBODY_H

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QFileInfo>

#include "qnstgraphicscomposition.h"

#include "qnstgraphicscontext.h"
#include "qnstgraphicsswitch.h"
#include "qnstgraphicsport.h"
#include "qnstgraphicsaggregator.h"

class QnstGraphicsBody : public QnstGraphicsComposition
{
  Q_OBJECT

public:
  QnstGraphicsBody(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsBody();

private:
  void createObjects();

  void createConnections();
};

#endif // QNSTGRAPHICSBODY_H
