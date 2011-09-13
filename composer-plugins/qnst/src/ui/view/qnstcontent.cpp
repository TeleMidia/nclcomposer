#include "qnstcontent.h"

QnstContent::QnstContent(QnstNode* parent)
    : QnstNode(parent)
{
    bodyAction->setEnabled(false);

    portAction->setEnabled(true);
    propertyAction->setEnabled(true);
    areaAction->setEnabled(true);
}

QnstContent::~QnstContent()
{

}
