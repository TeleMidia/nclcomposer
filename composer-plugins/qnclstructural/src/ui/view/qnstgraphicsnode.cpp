#include "qnstgraphicsnode.h"

QnstGraphicsNode::QnstGraphicsNode(QnstGraphicsNode* parent)
    : QncgGraphicsNode(parent), QnstEntity(parent)
{
    setnstType(Qnst::Node);

    connect(this, SIGNAL(entityChanged()), SLOT(requestEntityChange()));
    connect(this, SIGNAL(entitySelected()), SLOT(requestEntitySelection()));
}

QnstGraphicsNode::~QnstGraphicsNode()
{

}

void QnstGraphicsNode::requestEntityChange()
{
    emit entityChanged(this);
}

void QnstGraphicsNode::requestEntitySelection()
{
    emit entitySelected(this);
}
