#include "qnstgraphicslink.h"

QnstGraphicsLink::QnstGraphicsLink(QnstGraphicsEntity* parent)
    : QnstGraphicsEdge(parent)
{
    setnstType(Qnst::Link);
}

QnstGraphicsLink::~QnstGraphicsLink()
{

}
