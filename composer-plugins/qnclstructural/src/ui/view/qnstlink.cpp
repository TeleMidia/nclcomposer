#include "qnstlink.h"

QnstLink::QnstLink()
{
    setnstType(Qnst::Link);
}

QnstLink::~QnstLink()
{

}

QString QnstLink::getAggregatorUID() const
{
    return aggregatorUID;
}

void QnstLink::setAggregatorUID(QString aggregatorUID)
{
    this->aggregatorUID = aggregatorUID;
}

QString QnstLink::getxConnector() const
{
    return xconnector;
}

void QnstLink::setxConnector(QString xconnector)
{
    this->xconnector = xconnector;
}

QString QnstLink::getxConnectorUID() const
{
    return xconnectorUID;
}

void QnstLink::setxConnectorUID(QString xconnectorUID)
{
    this->xconnectorUID = xconnectorUID;
}

QMap<QString, QnstBind*> QnstLink::getActions() const
{
    return actions;
}

void QnstLink::addAction(QnstBind* action)
{
    if (action != NULL){
        actions.insert(action->getnstUid(), action);
    }
}

void QnstLink::removeAction(QnstBind* action)
{
    if (action != NULL){
        actions.remove(action->getnstUid());
    }
}

QMap<QString, QnstBind*> QnstLink::getConditions() const
{
    return conditions;
}

void QnstLink::addCondition(QnstBind* condition)
{
    if (condition != NULL){
        conditions.insert(condition->getnstUid(), condition);
    }
}

void QnstLink::removeCondition(QnstBind* condition)
{
    if (condition != NULL){
        conditions.remove(condition->getnstUid());
    }
}
