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
  explicit CompositeRuleItem(QTreeWidget *view, const QString &id,
                             const QString &op, const int &type,
                             QWidget *parent);

  explicit CompositeRuleItem(QTreeWidgetItem *item, QString id,
                             QString op, int type, QWidget *parent);

  inline QString getId () const
  { return text (1); }

  inline QString getOperator () const
  { return _operator; }

  inline void setId (const QString &id)
  { setText(ID_COLUMN, id); }

  void setOperator (const int &op);
  void setOperator (const QString &op);


private:
  void init (const QString &id, const QString &op);

  QString _operator;
};

#endif // COMPOSITERULEITEM_H
