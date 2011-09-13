#include "qnstmedia.h"

QnstMedia::QnstMedia(QnstNode* parent)
    : QnstContent(parent)
{
    setColor("#FF6347");
    setBordertColor("#8E2323");

    createConnection();

    setnstType(Qnst::Media);

    portAction->setEnabled(false);
    propertyAction->setEnabled(false);
}

QnstMedia::~QnstMedia()
{

}

void QnstMedia::addArea()
{
    QnstArea* area = new QnstArea(this);
    area->setParentItem(this);
    area->setTop(0);
    area->setLeft(0);
    area->setWidth(35);
    area->setHeight(35);
    area->setnstParent(this);
    area->adjust();

    connect(area,SIGNAL(entitySelected(QnstEntity*)),
                 SIGNAL(entitySelected(QnstEntity*)));

    connect(area,SIGNAL(entityAdded(QnstEntity*)),
            SIGNAL(entityAdded(QnstEntity*)));

    emit entityAdded(area);
}

void QnstMedia::createConnection()
{
    connect(areaAction, SIGNAL(triggered()), SLOT(addArea()));
}
