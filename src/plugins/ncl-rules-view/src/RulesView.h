#ifndef RULESVIEW_H
#define RULESVIEW_H

#include <QWidget>
#include <QTreeView>
#include <QTreeWidget>

class RulesView : public QWidget
{
  Q_OBJECT
public:
  explicit RulesView(QWidget *parent = 0);

private:
  QTreeWidget *_table;
};

#endif // RULESVIEW_H
