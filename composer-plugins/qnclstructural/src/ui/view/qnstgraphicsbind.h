#ifndef QNSTGRAPHICSBIND_H
#define QNSTGRAPHICSBIND_H

#include "qnstgraphicsedge.h"
#include "qnstconnector.h"
#include "qnstgraphicsbinddialog.h"

class QnstGraphicsBind : public QnstGraphicsEdge
{
  Q_OBJECT

public:
  QnstGraphicsBind(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsBind();

  /* FROM ACTION */
  Qnst::BindType getAction();

  void setAction(Qnst::BindType action);

  QString getActionIcon();

  bool isAction() const;

  /* FROM CONDITION */
  Qnst::BindType getCondition();

  void setCondition(Qnst::BindType condition);

  QString getConditionIcon();

  bool isCondition() const;

  void adjust();

  // \todo This two functions must be merged
  void adjust_action();
  void adjust_condition();

  void aux_adjust(QPointF pointa, QPointF pointb);

  void setConn(QnstConnector* conn);

  void setNameUids(QMap<QString, QString> nameUids);

  void setParams(QMap<QString, QString> params);

  void addParam(QString uid, QString name, QString value);

  void setParam(QString name, QString value);

  void removeParam(QString name);

  void removeUId(QString uid);

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

  QMap<QString, QString> name_uid;

  QMap<QString, QString> params;

  QnstGraphicsBindDialog* dialog;
};

#endif // QNSTGRAPHICSBIND_H
