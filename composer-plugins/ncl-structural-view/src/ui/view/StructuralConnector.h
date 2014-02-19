#ifndef QNSTCONNECTOR_H
#define QNSTCONNECTOR_H

#include <QMap>
#include <QDebug>

#include  "StructuralEntity.h"

class StructuralConnector : public StructuralEntity
{
public:
  StructuralConnector();

  ~StructuralConnector();

  QString getName();

  void setName(QString name);

  QMap<QString, QString> getConditions();
  bool hasCondition(QString uid);
  void addCondition(QString uid, QString condition);
  void removeCondition(QString uid);

  QMap<QString, QString> getActions();
  bool hasAction(QString uid);
  void addAction(QString uid, QString action);
  void removeAction(QString uid);

  QMap<QString, QString> getParams();
  void addParam(QString uid, QString name);
  void removeParam(QString uid);

  QMap<QPair<QString,QString>, QString> getConditionParams();
  void addConditionParam(QString uid, QString name, QString value);
  void removeConditionParam(QString uid, QString name);

  QMap<QPair<QString,QString>, QString> getActionParams();
  void addActionParam(QString uid, QString name, QString value);
  void removeActionParam(QString uid, QString name);

private:
  QString name;

  QMap<QString, QString> params;

  QMap<QPair<QString,QString>, QString> conditionParams;
  QMap<QPair<QString,QString>, QString> actionParams;

  QMap<QString, QString> conditions;

  QMap<QString, QString> actions;
};

#endif // QNSTCONNECTOR_H
