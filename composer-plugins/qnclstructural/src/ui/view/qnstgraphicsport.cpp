#include "qnstgraphicsport.h"

QnstGraphicsPort::QnstGraphicsPort(QnstGraphicsNode* parent)
    : QnstGraphicsInterface(parent)
{
    setnstType(Qnst::Port);

    setIcon(":/icon/port");
}

QnstGraphicsPort::~QnstGraphicsPort()
{

}

