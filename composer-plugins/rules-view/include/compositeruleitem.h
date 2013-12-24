#ifndef COMPOSITERULEITEM_H
#define COMPOSITERULEITEM_H

#include <QTreeWidgetItem>
#include "util.h"

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

  QString id () const { return text (1); }
  QString getOperator () const { return _operator; }

  inline void setId (QString id) { setText(ID_COLUMN, id); }
  void setOperator (int op);
  void setOperator (QString op);


private:
  void init (QString id, QString op);

  QString _operator;
};

#endif // COMPOSITERULEITEM_H
