#ifndef QNSTGRAPHICSCONTEXT_H
#define QNSTGRAPHICSCONTEXT_H

#include "qnstgraphicscomposition.h"

#include "qnstgraphicsswitch.h"
#include "qnstgraphicsmedia.h"
#include "qnstgraphicsport.h"

class QnstGraphicsContext : public QnstGraphicsComposition
{
  Q_OBJECT

public:
  QnstGraphicsContext(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsContext();

private:
  void createObjects();

  void createConnections();
};

#endif // QNSTGRAPHICSCONTEXT_H
