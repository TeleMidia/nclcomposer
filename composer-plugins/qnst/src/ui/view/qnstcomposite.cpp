#include "qnstcomposite.h"

QnstComposite::QnstComposite(QnstNode* parent)
    : QnstNode(parent)
{
    bodyAction->setEnabled(false);

    portAction->setEnabled(true);
    propertyAction->setEnabled(true);
    contextAction->setEnabled(true);
    switchAction->setEnabled(true);
    mediaAction->setEnabled(true);
}

QnstComposite::~QnstComposite()
{

}
