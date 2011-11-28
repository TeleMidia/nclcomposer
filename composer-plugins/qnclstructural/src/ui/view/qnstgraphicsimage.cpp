#include "qnstgraphicsimage.h"

QnstGraphicsImage::QnstGraphicsImage(QnstGraphicsNode* parent)
    : QnstGraphicsMedia(parent)
{
    setnstType(Qnst::Image);

    setIcon(":/icon/image");
}

QnstGraphicsImage::~QnstGraphicsImage()
{

}

