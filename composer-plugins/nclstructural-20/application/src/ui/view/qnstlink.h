#ifndef QNSTGRAPHICSLINK_H
#define QNSTGRAPHICSLINK_H

#include "qnstnode.h"
#include "qnstbind.h"

class QnstBind;

// \todo The qnstgraphicsaggregator and qnstlink must be part of this class.
class QnstLink : public QnstNode
{
public:
  QnstLink(QnstEntity* parent = 0);

  ~QnstLink();

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

  QString xconnector;

  QString xconnectorUID;

  QMap<QString, QnstBind*> actions;

  QMap<QString, QnstBind*> conditions;
};

#endif // QNSTGRAPHICSLINK_H
