#ifndef QNSTGRAPHICSCONTENT_H
#define QNSTGRAPHICSCONTENT_H

#include "qnstgraphicsnode.h"

class QnstGraphicsContent : public QnstGraphicsNode
{
public:
    QnstGraphicsContent(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsContent();
};

#endif // QNSTGRAPHICSCONTENT_H
