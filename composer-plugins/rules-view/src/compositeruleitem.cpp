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
  _id = id;
  _operator = op;

  setText(0, _id + " (" + _operator + ")");

  //setFlags(flags() | Qt::ItemIsEditable);
}

void CompositeRuleItem::setOperator(int op)
{
  switch (op) {
    case AND_OP:
      _operator = "and";
      break;
    case OR_OP:
      _operator = "or";
      break;
    default:
      qDebug () << "Invalid operator.";
      return;
  }

  setText(0, _id + " (" + _operator + ")");
}
