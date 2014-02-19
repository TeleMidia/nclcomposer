#ifndef QNSTGRAPHICSLINK_H
#define QNSTGRAPHICSLINK_H

#include "StructuralNode.h"
#include "StructuralBind.h"

class StructuralBind;

// \todo The qnstgraphicsaggregator and qnstlink must be part of this class.
class StructuralLink : public StructuralNode
{
public:
  StructuralLink(StructuralEntity* parent = 0);

  ~StructuralLink();

  QString getxConnector() const;

  void setxConnector(QString xconnector);

  QString getxConnectorUID() const;

  void setxConnectorUID(QString xconnectorUID);

  QMap<QString, StructuralBind*> getActions() const;

  void addAction(StructuralBind* action);

  void removeAction(StructuralBind* action);

  QMap<QString, StructuralBind*> getConditions() const;

  void addCondition(StructuralBind* condition);

  void removeCondition(StructuralBind* condition);

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

private:
  void createObjects();

  void createConnections();

  QString xconnector;

  QString xconnectorUID;

  QMap<QString, StructuralBind*> actions;

  QMap<QString, StructuralBind*> conditions;
};

#endif // QNSTGRAPHICSLINK_H
