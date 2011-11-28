#include "qnstgraphicsvideo.h"

QnstGraphicsVideo::QnstGraphicsVideo(QnstGraphicsNode* parent)
    : QnstGraphicsMedia(parent)
{
    setnstType(Qnst::Video);

    setIcon(":/icon/video");
}

QnstGraphicsVideo::~QnstGraphicsVideo()
{

}
