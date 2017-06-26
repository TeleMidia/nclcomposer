#ifndef RULESTREEWIDGET_H
#define RULESTREEWIDGET_H

#include <QTreeWidget>

enum
{
  RULEBASE_TYPE = QTreeWidgetItem::UserType,
  RULE_TYPE,
  COMPOSITERULE_TYPE
};

class RulesTreeWidget : public QTreeWidget
{
  Q_OBJECT
public:
  explicit RulesTreeWidget (QWidget *parent = 0);

protected:
  void mousePressEvent (QMouseEvent *event);

signals:
  void removeEntityRequested (QTreeWidgetItem *);
  void addRuleRequested (QTreeWidgetItem *, int);

private slots:
  void onCustomContextMenuRequested (const QPoint &pos);
  void showContextMenu (QTreeWidgetItem *item, const QPoint &globalPos);
  void editItem (QTreeWidgetItem *item, const int &column);
};

#endif // RULESTREEWIDGET_H
