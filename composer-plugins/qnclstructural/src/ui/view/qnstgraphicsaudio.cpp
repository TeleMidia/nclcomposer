#include "qnstgraphicsaudio.h"

QnstGraphicsAudio::QnstGraphicsAudio(QnstGraphicsNode* parent)
    : QnstGraphicsMedia(parent)
{
    setnstType(Qnst::Audio);

    setIcon(":/icon/audio");
}

QnstGraphicsAudio::~QnstGraphicsAudio()
{

}
