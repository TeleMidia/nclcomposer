#ifndef QNSTGRAPHICSLINK_H
#define QNSTGRAPHICSLINK_H

#include "qnstgraphicsnode.h"
#include "qnstgraphicsbind.h"

class QnstGraphicsBind;

// \todo The qnstgraphicsaggregator and qnstlink must be part of this class.
class QnstGraphicsLink : public QnstGraphicsNode
{
public:
  QnstGraphicsLink(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsLink();

  QString getxConnector() const;

  void setxConnector(QString xconnector);

  QString getxConnectorUID() const;

  void setxConnectorUID(QString xconnectorUID);

  QMap<QString, QnstGraphicsBind*> getActions() const;

  void addAction(QnstGraphicsBind* action);

  void removeAction(QnstGraphicsBind* action);

  QMap<QString, QnstGraphicsBind*> getConditions() const;

  void addCondition(QnstGraphicsBind* condition);

  void removeCondition(QnstGraphicsBind* condition);

  virtual void setProperties(const QMap<QString, QString> &properties);

  virtual void getProperties(QMap<QString, QString> &properties);

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

private:
  void createObjects();

  void createConnections();

  QString xconnector;

  QString xconnectorUID;

  QMap<QString, QnstGraphicsBind*> actions;

  QMap<QString, QnstGraphicsBind*> conditions;
};

#endif // QNSTGRAPHICSLINK_H
