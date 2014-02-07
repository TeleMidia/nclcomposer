#ifndef RULESTREEWIDGET_H
#define RULESTREEWIDGET_H

#include <QTreeWidget>

static const int RULEBASE_TYPE = QTreeWidgetItem::UserType;
static const int RULE_TYPE = QTreeWidgetItem::UserType + 1;
static const int COMPOSITERULE_TYPE = QTreeWidgetItem::UserType + 2;


class RulesTreeWidget : public QTreeWidget
{
  Q_OBJECT
public:
  explicit RulesTreeWidget(QWidget *parent = 0);

protected:
  void mousePressEvent(QMouseEvent *event);

signals:
  void removeEntityRequested (QTreeWidgetItem *);
  void addRuleRequested (QTreeWidgetItem*, int);

private slots:
  void onCustomContextMenuRequested(const QPoint &pos);
  void showContextMenu(QTreeWidgetItem* item, const QPoint& globalPos);
  void editItem (QTreeWidgetItem *item, const int &column);
};

#endif // RULESTREEWIDGET_H
