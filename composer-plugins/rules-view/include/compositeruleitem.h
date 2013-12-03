#ifndef COMPOSITERULEITEM_H
#define COMPOSITERULEITEM_H

#include <QTreeWidgetItem>

static const int AND_OP = 0;
static const int OR_OP = 1;

class CompositeRuleItem : public QObject, public QTreeWidgetItem
{
  Q_OBJECT
public:
  explicit CompositeRuleItem(QTreeWidget *view, QString id,
                             QString op, int type, QWidget *parent);

  explicit CompositeRuleItem(QTreeWidgetItem *item, QString id,
                             QString op, int type, QWidget *parent);

  QString id () const { return _id; }
  QString getOperator () const { return _operator; }

  void setOperator (int op);

private:
  void init (QString id, QString op);

  QString _id;
  QString _operator;
};

#endif // COMPOSITERULEITEM_H
