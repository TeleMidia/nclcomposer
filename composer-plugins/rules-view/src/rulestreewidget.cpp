#include "include/rulestreewidget.h"

#include <QMenu>
#include <QDebug>
#include <QIcon>

#include "compositeruleitem.h"

RulesTreeWidget::RulesTreeWidget(QWidget *parent) :
  QTreeWidget(parent)
{
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
          SLOT(onCustomContextMenuRequested(const QPoint&)));

  connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
          SLOT(editItem(QTreeWidgetItem*,int)));

  setEditTriggers(NoEditTriggers);
}

void RulesTreeWidget::onCustomContextMenuRequested(const QPoint &pos)
{
  QTreeWidgetItem* item = itemAt(pos);

  if (!item) return;

  showContextMenu(item, pos);
}

void RulesTreeWidget::showContextMenu(QTreeWidgetItem* item,
                                      const QPoint& globalPos)
{
  QMenu menu;

  QAction *addRuleAction = 0;
  QAction *removeRuleAction = 0;
  QAction *andOpAction = 0;
  QAction *orOpAction = 0;

  removeRuleAction = menu.addAction (QIcon(":icon/delete"),
                                     "Remove Element");

  switch (item->type()) {
    case RULE_TYPE:
      break;

    case COMPOSITE_TYPE:
      menu.addSeparator();
      QMenu *opMenu = menu.addMenu("Operator");
      andOpAction = opMenu->addAction("and");
      orOpAction = opMenu->addAction("or");
      break;
  }

  QAction *choosenAction = menu.exec(mapToGlobal(globalPos));
  if (choosenAction)
  {
    if (choosenAction == andOpAction || choosenAction == orOpAction)
    {
      CompositeRuleItem *compositeRule = dynamic_cast <CompositeRuleItem *>
          (item);
      if (choosenAction == andOpAction)
        compositeRule->setOperator (AND_OP);
      else
        compositeRule->setOperator(OR_OP);
    }
    else if (choosenAction == removeRuleAction)
    {
      emit removeEntityRequested (item);
    }
  }
}

void RulesTreeWidget::editItem(QTreeWidgetItem *item, int column)
{
  bool isColumnEditable = false;
  if (item->type() == RULEBASE_TYPE || item->type() == COMPOSITE_TYPE)
  {
    if (column == 1)
      isColumnEditable = true;

  }
  else if (item->type() == RULE_TYPE)
  {
    if (column > 0)
      isColumnEditable = true;
  }

  if (isColumnEditable)
    QTreeWidget::editItem(item, column);
}
