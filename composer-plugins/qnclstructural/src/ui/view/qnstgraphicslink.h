#ifndef QNSTGRAPHICSLINK_H
#define QNSTGRAPHICSLINK_H

#include "qnstgraphicsnode.h"
#include "qnstbind.h"

// \todo The qnstgraphicsaggregator and qnstlink must be part of this class.
class QnstGraphicsLink : public QnstGraphicsNode
{
public:
  QnstGraphicsLink(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsLink();

  // \fixme The following two methods must be removed.
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

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

private:
  void createObjects();

  void createConnections();

  QString aggregatorUID;

  QString xconnector;

  QString xconnectorUID;

  QMap<QString, QnstBind*> actions;

  QMap<QString, QnstBind*> conditions;
};

#endif // QNSTGRAPHICSLINK_H
