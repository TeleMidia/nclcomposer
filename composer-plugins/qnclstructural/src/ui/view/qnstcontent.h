#ifndef QNSTCONTENT_H
#define QNSTCONTENT_H

#include "qnstnode.h"


class QnstContent : public QnstNode
{
public:
    QnstContent(QnstNode* parent = 0);

    ~QnstContent();
};

#endif // QNSTCONTENT_H
