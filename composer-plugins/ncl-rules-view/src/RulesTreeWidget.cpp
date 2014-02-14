#include "RulesTreeWidget.h"

#include <QMenu>
#include <QDebug>
#include <QIcon>
#include <QMessageBox>
#include <QMouseEvent>

#include "CompositeRuleItem.h"

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
  QAction *addCompositeRuleAction = 0;
  QAction *removeRuleAction = 0;
  QAction *andOpAction = 0;
  QAction *orOpAction = 0;

  if (item->type() == COMPOSITERULE_TYPE || item->type() == RULEBASE_TYPE)
  {
    addRuleAction = menu.addAction("Add Rule");
    addCompositeRuleAction = menu.addAction("Add Composite Rule");
  }

  removeRuleAction = menu.addAction (QIcon(":icon/delete"),
                                     item->type() == RULEBASE_TYPE ?
                                       "Remove ruleBase" : "Remove Rule");

  removeRuleAction->setShortcut(QKeySequence::Delete);


  if (item->type() == COMPOSITERULE_TYPE)
  {
    menu.addSeparator();
    QMenu *opMenu = menu.addMenu("Operator");
    andOpAction = opMenu->addAction("and");
    orOpAction = opMenu->addAction("or");
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
      int button =
          QMessageBox::question(this, "Remove Element",
                                "Do you really want remove this element?",
                                QMessageBox::Yes, QMessageBox::No);

      if (button == QMessageBox::Yes)
        emit removeEntityRequested (item);
    }
    else if (choosenAction == addRuleAction)
      emit addRuleRequested(item, RULE_TYPE);
    else if (choosenAction == addCompositeRuleAction)
      emit addRuleRequested(item, COMPOSITERULE_TYPE);
  }
}

void RulesTreeWidget::editItem(QTreeWidgetItem *item, const int &column)
{
  bool isColumnEditable = false;
  if (item->type() == RULEBASE_TYPE || item->type() == COMPOSITERULE_TYPE)
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

void RulesTreeWidget::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::RightButton && topLevelItemCount() == 0)
  {
    QMenu menu;

    menu.addAction ("Add ruleBase");
    if (menu.exec(mapToGlobal(event->pos())))
      emit addRuleRequested(0, RULEBASE_TYPE);
  }
  QTreeWidget::mousePressEvent(event);
}
