#ifndef QNSTGRAPHICSPORT_H
#define QNSTGRAPHICSPORT_H

#include "qnstgraphicsinterface.h"

class QnstGraphicsPort : public QnstGraphicsInterface
{
public:
    QnstGraphicsPort(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsPort();
};

#endif // QNSTGRAPHICSPORT_H
