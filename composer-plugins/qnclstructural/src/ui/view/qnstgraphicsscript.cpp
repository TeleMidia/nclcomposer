#include "qnstgraphicsscript.h"

QnstGraphicsScript::QnstGraphicsScript(QnstGraphicsNode* parent)
    : QnstGraphicsMedia(parent)
{
    setnstType(Qnst::Script);

    setIcon(":/icon/script");
}

QnstGraphicsScript::~QnstGraphicsScript()
{

}
