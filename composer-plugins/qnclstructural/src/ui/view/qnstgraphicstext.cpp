#include "qnstgraphicstext.h"

QnstGraphicsText::QnstGraphicsText(QnstGraphicsNode* parent)
    : QnstGraphicsMedia(parent)
{
    setnstType(Qnst::Text);

    setIcon(":/icon/text");
}

QnstGraphicsText::~QnstGraphicsText()
{

}
