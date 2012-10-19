#ifndef QNSTGRAPHICSLINK_H
#define QNSTGRAPHICSLINK_H

#include "qnstgraphicsedge.h"

// \todo The qnstgraphicsaggregator and qnstlink must be part of this class.
class QnstGraphicsLink : public QnstGraphicsEdge
{
public:
  QnstGraphicsLink(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsLink();
};

#endif // QNSTGRAPHICSLINK_H
