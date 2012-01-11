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

void QnstLink::setAggregatorUID(QString aggregatorUID) const
{
    this->aggregatorUID = aggregatorUID;
}

QString QnstLink::getxConnector() const
{
    return xconnector;
}

void QnstLink::setxConnector(QString xconnector) const
{
    this->xconnector = xconnector;
}

QString QnstLink::getxConnectorUID() const
{
    return xconnectorUID;
}

void QnstLink::setxConnectorUID(QString xconnectorUID) const
{
    this->xconnectorUID = xconnectorUID;
}

QVector<QnstBind*> QnstLink::getActions() const
{
    return actions;
}

void QnstLink::addAction(QnstBind* action)
{
    actions.append(action);
}

void QnstLink::removeAction(QnstBind* action)
{
    int index = actions.indexOf(action);

    if (index >= 0){
        actions.remove(index);
    }
}

QVector<QnstBind*> QnstLink::getConditions() const
{
    return conditions;
}

void QnstLink::addCondition(QnstBind* condition)
{
    conditions.append(condition);
}

void QnstLink::removeCondition(QnstBind* condition)
{
    int index = conditions.indexOf(condition);

    if (index >= 0){
        conditions.remove(index);
    }
}
