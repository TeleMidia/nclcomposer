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


  void setRole(const QString &qt_check_for_QOBJECT_macrorole);

  QString getComponent() const;

  void setComponent(const QString &component);

  QString getComponentUid() const;

  void setComponentUid(const QString &componentUid);

  QString getInterface() const;

  void setInterface(const QString &interface);

  QString getInterfaceUid() const;

  void setInterfaceUid(const QString &interfaceUid);

  QMap<QString, QString> getParams();

  void setParams(const QMap<QString, QString> &params);

  QMap<QString, QString> getNameUIDs();

  void setNamesUIDs(const QMap<QString, QString> &name_uids);

  //void setConn(QnstConnector* conn);

  void addParam(const QString &_uid, const QString &_name, const QString &value);

  void setParam(const QString &_name, const QString &value);

  void removeParam(const QString &_name);

  void removeUId(const QString &_uid);

  void setLink(StructuralLink *link);

  StructuralLink* getLink();

  virtual void setLocalProperty(const QString &name, const QString &value);

  void setTarget(StructuralEntity *target);

  StructuralEntity *getTarget();

  virtual void updateToolTip();

  void setType(StructuralRole _name);
  StructuralRole getType();
  QString getIcon();

  bool isAction() const;
  bool isCondition() const;

  virtual void adjust(bool avoidCollision = false, bool rec = true);

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

  void showEditBindDialog(StructuralBind* entity);

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
  StructuralRole _name;

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
