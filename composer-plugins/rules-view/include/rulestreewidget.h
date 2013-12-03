#ifndef RULESTREEWIDGET_H
#define RULESTREEWIDGET_H

#include <QTreeWidget>

static const int RULE_TYPE = QTreeWidgetItem::UserType;
static const int COMPOSITE_TYPE = QTreeWidgetItem::UserType + 1;


class RulesTreeWidget : public QTreeWidget
{
  Q_OBJECT
public:
  explicit RulesTreeWidget(QWidget *parent = 0);

signals:
  void entityChanged (QTreeWidgetItem *);

private slots:
    void onCustomContextMenuRequested(const QPoint &pos);

    void showContextMenu(QTreeWidgetItem* item, const QPoint& globalPos);
};

#endif // RULESTREEWIDGET_H
