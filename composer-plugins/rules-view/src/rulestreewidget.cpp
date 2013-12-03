#include "include/rulestreewidget.h"

#include <QMenu>
#include <QDebug>

#include "compositeruleitem.h"

RulesTreeWidget::RulesTreeWidget(QWidget *parent) :
  QTreeWidget(parent)
{
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
          SLOT(onCustomContextMenuRequested(const QPoint&)));
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

  switch (item->type()) {
    case RULE_TYPE:
      break;

    case COMPOSITE_TYPE:
      QMenu *opMenu = menu.addMenu("operator");
      opMenu->addAction("and");
      opMenu->addAction("or");

      break;
  }

  if (item->Type != RULE_TYPE)
  {
    QAction *choosenAction = menu.exec(mapToGlobal(globalPos));
    if (choosenAction)
    {
      QString actionText = choosenAction->text();
      CompositeRuleItem *compositeRule = dynamic_cast <CompositeRuleItem *>
          (item);
      if (compositeRule)
      {
        if (actionText == "and")
          compositeRule->setOperator (AND_OP);
        else if (actionText == "or")
          compositeRule->setOperator(OR_OP);

        emit entityChanged(compositeRule);
      }
    }

  }
}

