#ifndef QNSTLINK_H
#define QNSTLINK_H

#include <QMap>

#include "qnstentity.h"
#include "qnstbind.h"

class QnstLink : public QnstEntity
{
public:
  QnstLink();

  virtual ~QnstLink();

  QString getAggregatorUID() const;

  void setAggregatorUID(QString aggregatorUID);

  QString getxConnector() const;

  void setxConnector(QString xconnector);

  QString getxConnectorUID() const;

  void setxConnectorUID(QString xconnectorUID);

  QMap<QString, QnstBind*> getActions() const;

  void addAction(QnstBind* action);

  void removeAction(QnstBind* action);

  QMap<QString, QnstBind*> getConditions() const;

  void addCondition(QnstBind* condition);

  void removeCondition(QnstBind* condition);

private:
  QString aggregatorUID;

  QString xconnector;

  QString xconnectorUID;

  QMap<QString, QnstBind*> actions;

  QMap<QString, QnstBind*> conditions;
};

#endif // QNSTLINK_H
