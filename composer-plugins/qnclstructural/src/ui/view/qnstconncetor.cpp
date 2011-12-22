#include "qnstconncetor.h"

QnstConncetor::QnstConncetor()
{

}

QnstConncetor::~QnstConncetor()
{

}

QString QnstConncetor::getName()
{
    return name;
}

void QnstConncetor::setName(QString name)
{
    this->name = name;
}

QVector<QString> QnstConncetor::getConditions()
{
    return conditions;
}

void QnstConncetor::addCondition(QString condition)
{
    if (!conditions.contains(condition)){
        conditions.append(condition);
    }
}

void QnstConncetor::removeCondition(QString condition)
{
    if (conditions.contains(condition)){
        int index = conditions.indexOf(condition);

        if (index >= 0){
            conditions.remove(index);
        }
    }
}

QVector<QString> QnstConncetor::getActions()
{
    return actions;
}

void QnstConncetor::addAction(QString action)
{
    if (!actions.contains(action)){
        actions.append(action);
    }
}

void QnstConncetor::removeAction(QString action)
{
    if (actions.contains(action)){
        int index = actions.indexOf(action);

        if (index >= 0){
            actions.remove(index);
        }
    }
}
