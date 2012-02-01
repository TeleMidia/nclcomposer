#include "qnstgraphicsncl.h"

QnstGraphicsNCL::QnstGraphicsNCL(QnstGraphicsNode* parent)
    : QnstGraphicsMedia(parent)
{
    setnstType(Qnst::NCL);

    setIcon(":/icon/ncl");
}

QnstGraphicsNCL::~QnstGraphicsNCL()
{

}
