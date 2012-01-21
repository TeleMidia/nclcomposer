#include "qnstgraphicshtml.h"

QnstGraphicsHTML::QnstGraphicsHTML(QnstGraphicsNode* parent)
    : QnstGraphicsMedia(parent)
{
    setnstType(Qnst::Html);

    setIcon(":/icon/html");
}

QnstGraphicsHTML::~QnstGraphicsHTML()
{

}
