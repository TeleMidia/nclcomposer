#ifndef QNSTCOMPOSITE_H
#define QNSTCOMPOSITE_H

#include "qnstnode.h"

class QnstComposite : public QnstNode
{
public:
    QnstComposite(QnstNode* parent = 0);

    ~QnstComposite();
};

#endif // QNSTCOMPOSITE_H
