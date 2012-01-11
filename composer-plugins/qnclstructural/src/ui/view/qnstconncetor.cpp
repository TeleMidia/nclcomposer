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

QMap<QString, QString> QnstConncetor::getConditions()
{
    return conditions;
}

void QnstConncetor::addCondition(QString uid, QString condition)
{
    conditions[uid] = condition;
}

void QnstConncetor::removeCondition(QString uid)
{
    conditions.remove(uid);
}

QMap<QString, QString> QnstConncetor::getActions()
{
    return actions;
}

void QnstConncetor::addAction(QString uid, QString action)
{
    actions[uid] = action;
}

void QnstConncetor::removeAction(QString uid)
{
    actions.remove(uid);
}
