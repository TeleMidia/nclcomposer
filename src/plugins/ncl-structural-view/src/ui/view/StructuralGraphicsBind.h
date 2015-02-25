#ifndef QNSTGRAPHICSBIND_H
#define QNSTGRAPHICSBIND_H

#include "StructuralGraphicsEdge.h"
#include "StructuralConnector.h"
#include "StructuralGraphicsLink.h"
#include "StructuralGraphicsBindDialog.h"

#include <QDebug>

class StructuralGraphicsLink;

class StructuralGraphicsBind : public StructuralGraphicsEdge
{
  Q_OBJECT

public:
  StructuralGraphicsBind(StructuralGraphicsEntity* parent = 0);

  ~StructuralGraphicsBind();

  QString getRole() const;

  void setRole(QString role);

  QString getComponent() const;

  void setComponent(QString component);

  QString getComponentUid() const;

  void setComponentUid(QString componentUid);

  QString getInterface() const;

  void setInterface(QString interface);

  QString getInterfaceUid() const;

  void setInterfaceUid(QString interfaceUid);

  QMap<QString, QString> getParams();

  void setParams(QMap<QString, QString> params);

  QMap<QString, QString> getNameUIDs();

  void setNamesUIDs(QMap<QString, QString> name_uids);

  void setConn(StructuralConnector* conn);

  void addParam(QString uid, QString name, QString value);

  void setParam(QString name, QString value);

  void removeParam(QString name);

  void removeUId(QString uid);

  void setLink(StructuralGraphicsLink *link);

  StructuralGraphicsLink* getLink();

  void setTarget(StructuralGraphicsEntity *target);

  StructuralGraphicsEntity *getTarget();

  virtual void setProperties(const QMap<QString, QString> &properties);

  virtual void getProperties(QMap<QString, QString> &properties);

  virtual void updateToolTip();

  void setType(Structural::BindType type);
  Structural::BindType getType();
  QString getIcon();

  bool isAction() const;
  bool isCondition() const;

  //void adjust();

  // \todo This two functions must be merged
  void adjust_action();
  void adjust_condition();

  void aux_adjust(QPointF pointa, QPointF pointb);

  // \fixme This method should not be here!!

signals:
  void bindParamAdded(QString uid, QString parent,
                      QMap<QString, QString> properties);

  void bindParamUpdated(QString bindUid, QMap<QString, QString> params,
                        QMap<QString, QString> name_uids);

protected:
  virtual void draw(QPainter* painter);

  // \todo this two functions must be merged
  virtual void draw_action(QPainter* painter);
  virtual void draw_condition(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

  // \todo this two functions must be merged
  virtual void delineate_action(QPainterPath* painter) const;
  virtual void delineate_condition(QPainterPath* painter) const;

  virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
  Structural::BindType type;

  QString icon;

  StructuralConnector* conn;

  QMap <QString, QString> names_uids;

  QMap<QString, QString> params;

  StructuralGraphicsBindDialog* dialog;

  QString role;

  QString component;

  QString componentUID;

  QString interface;

  QString interfaceUID;

  StructuralGraphicsLink *link;

};

#endif // QNSTGRAPHICSBIND_H
