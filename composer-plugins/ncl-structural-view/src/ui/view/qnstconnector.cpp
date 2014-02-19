#include "qnstconnector.h"

QnstConnector::QnstConnector()
{

}

QnstConnector::~QnstConnector()
{

}

QString QnstConnector::getName()
{
  return name;
}

void QnstConnector::setName(QString name)
{
  this->name = name;
}

QMap<QString, QString> QnstConnector::getConditions()
{
  return conditions;
}

void QnstConnector::addCondition(QString uid, QString condition)
{
  conditions[uid] = condition;
}

void QnstConnector::removeCondition(QString uid)
{
  if (conditions.remove(uid))
  {
    QPair<QString, QString> p;

    foreach(p, conditionParams.keys())
    {
      if (p.first == uid)
        conditionParams.remove(p);
    }
  }
}

bool QnstConnector::hasCondition(QString uid)
{
  return conditions.contains(uid);
}

bool QnstConnector::hasAction(QString uid)
{
  return actions.contains(uid);
}

QMap<QString, QString> QnstConnector::getActions()
{
  return actions;
}

void QnstConnector::addAction(QString uid, QString action)
{
  actions[uid] = action;
}

void QnstConnector::removeAction(QString uid)
{
  if (actions.remove(uid))
  {
    QPair<QString, QString> p;

    foreach(p, actionParams.keys())
    {
      if (p.first == uid)
        actionParams.remove(p);
    }
  }
}

QMap<QString, QString> QnstConnector::getParams()
{
  return params;
}

void QnstConnector::addParam(QString uid, QString value)
{
  params[uid] = value;
}

void QnstConnector::removeParam(QString uid)
{
  params.remove(uid);
}

QMap<QPair<QString,QString>, QString> QnstConnector::getConditionParams()
{
  return conditionParams;
}

void QnstConnector::addConditionParam(QString uid, QString name, QString value)
{
  QPair<QString, QString> key(uid, name);

  conditionParams.insert(key, value);
}

void QnstConnector::removeConditionParam(QString uid, QString name)
{
  QPair<QString, QString> key(uid, name);

  if (conditionParams.contains(key))
  {
    conditionParams.remove(key);
  }
}

QMap<QPair<QString,QString>, QString> QnstConnector::getActionParams()
{
  return actionParams;
}

void QnstConnector::addActionParam(QString uid, QString name, QString value)
{
  QPair<QString, QString> key(uid, name);

  actionParams.insert(key, value);
}

void QnstConnector::removeActionParam(QString uid, QString name)
{
  QPair<QString, QString> key(uid, name);

  if (actionParams.contains(key))
  {
    actionParams.remove(key);
  }
}
