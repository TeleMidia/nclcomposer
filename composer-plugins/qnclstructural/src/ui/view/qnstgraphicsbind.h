#ifndef QNSTGRAPHICSBIND_H
#define QNSTGRAPHICSBIND_H

#include "qnstgraphicsedge.h"
#include "qnstconnector.h"
#include "qnstgraphicslink.h"
#include "qnstgraphicsbinddialog.h"

class QnstGraphicsLink;

class QnstGraphicsBind : public QnstGraphicsEdge
{
  Q_OBJECT

public:
  QnstGraphicsBind(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsBind();

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

  void setConn(QnstConnector* conn);

  void addParam(QString uid, QString name, QString value);

  void setParam(QString name, QString value);

  void removeParam(QString name);

  void removeUId(QString uid);

  void setLink(QnstGraphicsLink *link);

  QnstGraphicsLink* getLink();

  virtual void setProperties(const QMap<QString, QString> &properties);

  void setType(Qnst::BindType type);
  Qnst::BindType getType();
  QString getIcon();

  bool isAction() const;
  bool isCondition() const;

  void adjust();

  // \todo This two functions must be merged
  void adjust_action();
  void adjust_condition();

  void aux_adjust(QPointF pointa, QPointF pointb);

  // \fixme This method should not be here!!

signals:
  void bindParamAdded(QString uid, QString parent, QMap<QString, QString> properties);

  void bindParamUpdated(QString bindUid, QMap<QString, QString> params,  QMap<QString, QString> name_uids);

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
  Qnst::BindType type;

  QString icon;

  QnstConnector* conn;

  QMap <QString, QString> names_uids;

  QMap<QString, QString> params;

  QnstGraphicsBindDialog* dialog;

  QString role;

  QString component;

  QString componentUID;

  QString interface;

  QString interfaceUID;

  QnstGraphicsLink *link;

};

#endif // QNSTGRAPHICSBIND_H
