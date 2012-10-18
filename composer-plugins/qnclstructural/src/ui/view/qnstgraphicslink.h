#ifndef QNSTGRAPHICSLINK_H
#define QNSTGRAPHICSLINK_H

#include "qnstgraphicsedge.h"

typedef QString QnstActionIcon;
typedef Qnst::ActionType QnstAction;

typedef QString QnstConditionIcon;
typedef Qnst::ConditionType QnstCondition;

class QnstGraphicsLink : public QnstGraphicsEdge
{
public:
  QnstGraphicsLink(QnstGraphicsEntity* parent = 0);

  ~QnstGraphicsLink();

  QnstAction getAction();

  void setAction(QnstAction action);

  QnstActionIcon getActionIcon();

  void setActionIcon(QnstActionIcon actionIcon);

  QnstCondition getCondition();

  void setCondition(QnstCondition condition);

  QnstConditionIcon getConditionIcon();

  void setConditionIcon(QnstConditionIcon conditionIcon);

protected:
  virtual void draw(QPainter* painter);

  virtual void delineate(QPainterPath* painter) const;

private:
  QnstAction action;

  QnstActionIcon actionIcon;

  QnstCondition condition;

  QnstConditionIcon conditionIcon;
};

#endif // QNSTGRAPHICSLINK_H
