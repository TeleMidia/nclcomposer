#include "StructuralConnector.h"

StructuralConnector::StructuralConnector()
{

}

StructuralConnector::~StructuralConnector()
{

}

QString StructuralConnector::getName()
{
  return name;
}

void StructuralConnector::setName(QString name)
{
  this->name = name;
}

QMap<QString, QString> StructuralConnector::getConditions()
{
  return conditions;
}

void StructuralConnector::addCondition(QString uid, QString condition)
{
  conditions[uid] = condition;
}

void StructuralConnector::removeCondition(QString uid)
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

bool StructuralConnector::hasCondition(QString uid)
{
  return conditions.contains(uid);
}

bool StructuralConnector::hasAction(QString uid)
{
  return actions.contains(uid);
}

QMap<QString, QString> StructuralConnector::getActions()
{
  return actions;
}

void StructuralConnector::addAction(QString uid, QString action)
{
  actions[uid] = action;
}

void StructuralConnector::removeAction(QString uid)
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

QMap<QString, QString> StructuralConnector::getParams()
{
  return params;
}

void StructuralConnector::addParam(QString uid, QString value)
{
  params[uid] = value;
}

void StructuralConnector::removeParam(QString uid)
{
  params.remove(uid);
}

QMap<QPair<QString,QString>, QString> StructuralConnector::getConditionParams()
{
  return conditionParams;
}

void StructuralConnector::addConditionParam(QString uid, QString name, QString value)
{
  QPair<QString, QString> key(uid, name);

  conditionParams.insert(key, value);
}

void StructuralConnector::removeConditionParam(QString uid, QString name)
{
  QPair<QString, QString> key(uid, name);

  if (conditionParams.contains(key))
  {
    conditionParams.remove(key);
  }
}

QMap<QPair<QString,QString>, QString> StructuralConnector::getActionParams()
{
  return actionParams;
}

void StructuralConnector::addActionParam(QString uid, QString name, QString value)
{
  QPair<QString, QString> key(uid, name);

  actionParams.insert(key, value);
}

void StructuralConnector::removeActionParam(QString uid, QString name)
{
  QPair<QString, QString> key(uid, name);

  if (actionParams.contains(key))
  {
    actionParams.remove(key);
  }
}
