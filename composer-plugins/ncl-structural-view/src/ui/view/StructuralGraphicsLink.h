#ifndef QNSTGRAPHICSLINK_H
#define QNSTGRAPHICSLINK_H

#include "StructuralGraphicsNode.h"
#include "StructuralGraphicsBind.h"

class StructuralGraphicsBind;

// \todo The qnstgraphicsaggregator and qnstlink must be part of this class.
class StructuralGraphicsLink : public StructuralGraphicsNode
{
public:
  StructuralGraphicsLink(StructuralGraphicsEntity* parent = 0);

  ~StructuralGraphicsLink();

  QString getxConnector() const;

  void setxConnector(QString xconnector);

  QString getxConnectorUID() const;

  void setxConnectorUID(QString xconnectorUID);

  QMap<QString, StructuralGraphicsBind*> getActions() const;

  void addAction(StructuralGraphicsBind* action);

  void removeAction(StructuralGraphicsBind* action);

  QMap<QString, StructuralGraphicsBind*> getConditions() const;

  void addCondition(StructuralGraphicsBind* condition);

  void removeCondition(StructuralGraphicsBind* condition);

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

  QMap<QString, StructuralGraphicsBind*> actions;

  QMap<QString, StructuralGraphicsBind*> conditions;
};

#endif // QNSTGRAPHICSLINK_H
