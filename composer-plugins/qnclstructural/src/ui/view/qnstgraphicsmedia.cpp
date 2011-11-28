#include "qnstgraphicsmedia.h"

QnstGraphicsMedia::QnstGraphicsMedia(QnstGraphicsNode* parent)
    : QnstGraphicsContent(parent)
{
    setnstType(Qnst::Media);

    setIcon(":/icon/media");
}

QnstGraphicsMedia::~QnstGraphicsMedia()
{

}
