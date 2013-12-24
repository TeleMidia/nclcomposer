#include "include/compositeruleitem.h"

#include <QDebug>

CompositeRuleItem::CompositeRuleItem(QTreeWidget *view, QString id,
                                     QString op, int type, QWidget *parent) :
  QObject (parent), QTreeWidgetItem (view, type)
{
  init (id, op);
}

CompositeRuleItem::CompositeRuleItem(QTreeWidgetItem *item, QString id,
                                     QString op, int type, QWidget *parent) :
  QObject (parent), QTreeWidgetItem (item, type)
{
  init (id, op);
}

void CompositeRuleItem::init(QString id, QString op)
{
  setText(ID_COLUMN, id);
  setOperator(op);

  setFlags(flags() | Qt::ItemIsEditable);
}

void CompositeRuleItem::setOperator(int op)
{
  QString operatorLabel = "";
  switch (op) {
    case AND_OP:
      operatorLabel = "and";
      break;
    case OR_OP:
      operatorLabel = "or";
      break;
    default:
      qDebug () << "Invalid operator.";
      return;
  }

  setOperator(operatorLabel);
}

void CompositeRuleItem::setOperator(QString op)
{
  _operator = op;

  QString label = "compositeRule";
  if (_operator != "")
    label += "(" + _operator + ")";

  setText(ELEMENT_COLUMN, label);
}
