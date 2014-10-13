#ifndef QNSTGRAPHICSBIND_H
#define QNSTGRAPHICSBIND_H

#include "StructuralEdge.h"
#include "StructuralLink.h"

class StructuralLink;

class StructuralBind : public StructuralEdge
{
  Q_OBJECT

public:
  StructuralBind(StructuralEntity* parent = 0);

  ~StructuralBind();

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

  //void setConn(QnstConnector* conn);

  void addParam(QString _uid, QString _name, QString value);

  void setParam(QString _name, QString value);

  void removeParam(QString _name);

  void removeUId(QString _uid);

  void setLink(StructuralLink *link);

  StructuralLink* getLink();

  virtual void setLocalProperty(const QString &name, const QString &value);

  void setTarget(StructuralEntity *target);

  StructuralEntity *getTarget();

  virtual void updateToolTip();

  void setType(Structural::BindType _name);
  Structural::BindType getType();
  QString getIcon();

  bool isAction() const;
  bool isCondition() const;

  virtual void adjust(bool avoidCollision = false);

  // \todo This two functions must be merged
  void adjust_action();
  void adjust_condition();

  void aux_adjust(QPointF pointa, QPointF pointb);

  // \fixme This method should not be here!!

signals:
  void bindParamAdded(QString _uid, QString parent,
                      QMap<QString, QString> _properties);

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
  Structural::BindType _name;

  QString icon;

  //QnstConnector* conn;

  QMap <QString, QString> names_uids;

  QMap<QString, QString> params;


  QString role;

  QString component;

  QString componentUID;

  QString interface;

  QString interfaceUID;

  QPointF globlalPointA;
  QPointF globlalPointB;

  StructuralLink *link;

};

#endif // QNSTGRAPHICSBIND_H
