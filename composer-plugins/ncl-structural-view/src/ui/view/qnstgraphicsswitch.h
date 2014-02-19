#ifndef QNSTGRAPHICSSWITCH_H
#define QNSTGRAPHICSSWITCH_H

#include "qnstgraphicscomposition.h"

#include "qnstgraphicscontext.h"
#include "qnstgraphicsmedia.h"
#include "qnstgraphicsport.h"
#include "qnstgraphicsswitchport.h"

class QnstGraphicsSwitch : public QnstGraphicsComposition
{
  Q_OBJECT

public:
  QnstGraphicsSwitch(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsSwitch();

private:
  void createObjects();

  void createConnections();
};


#endif // QNSTGRAPHICSSWITCH_H
