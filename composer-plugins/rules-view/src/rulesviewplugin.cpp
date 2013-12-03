#include "rulesviewplugin.h"

#include <QDebug>
#include <QAction>

#include "ruleitem.h"
#include "compositeruleitem.h"

RulesViewPlugin::RulesViewPlugin() :
  IPlugin ()
{
  _rulesTable = new RulesTreeWidget;
  _rulesTable->setColumnCount(4);
  connect(_rulesTable, SIGNAL(entityChanged(QTreeWidgetItem*)),
          this, SLOT(updateValue(QTreeWidgetItem*)));

  QStringList headers;
  headers << "id" << "Variable" << "Comparator" << "Value";

  _rulesTable->setHeaderLabels(headers);

  _ruleBaseEntity = 0;

  QStringList listenFilter;
  listenFilter << "ruleBase" << "rule" << "compositeRule" << "media";

  emit setListenFilter(listenFilter);
}

void RulesViewPlugin::onEntityAdded(QString ID, Entity *entity)
{
  if (!entity) return;

  qDebug () << entity->getType();
}

void RulesViewPlugin::init()
{
  Project * project = getProject();
  if (!project) return;

  QList <Entity *> ruleBaseList = project->getEntitiesbyType("ruleBase");

  if (ruleBaseList.count() > 0)
  {
    _ruleBaseEntity = ruleBaseList.at(0);
    findAllRules(_ruleBaseEntity);
  }

  connect (_rulesTable, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this,
           SLOT(updateValue(QTreeWidgetItem*)));

}

void RulesViewPlugin::findAllRules(Entity *root)
{
  if (!root) return;

  foreach (Entity *entity, root->getChildren())
  {
    QString type = entity->getType();
    if (type != "rule" && type != "compositeRule") continue;

    addRule (entity);

    if (type == "compositeRule")
      findAllRules(entity);
  }
}


void RulesViewPlugin::addRule(Entity *entity)
{
  if (!entity) return;

  QStringList itemLabels;

  if (entity->getType() != "rule")
  {
    itemLabels << entity->getAttribute("id") +
                  "(" + entity->getAttribute("operator") + ")";
  }

  QTreeWidgetItem *item = 0;
  QTreeWidgetItem *parent = 0;

  if (entity->getParent()->getType() != "ruleBase")
  {
    QList<QTreeWidgetItem *> treeItems = _items.keys();
    foreach (QTreeWidgetItem *treeItem, treeItems)
    {
      if (_items.value(treeItem) == entity->getParent())
      {
        parent = treeItem;
        break;
      }
    }
  }

  if (entity->getType() == "rule")
  {
    if (parent)
      item = new RuleItem ( parent,
                            entity->getAttribute("id"),
                            entity->getAttribute("var"),
                            entity->getAttribute("comparator"),
                            entity->getAttribute("value"),
                            RULE_TYPE, _rulesTable);
    else
      item = new RuleItem ( _rulesTable,
                            entity->getAttribute("id"),
                            entity->getAttribute("var"),
                            entity->getAttribute("comparator"),
                            entity->getAttribute("value"),
                            RULE_TYPE, _rulesTable);
  }
  else
  {
    if (parent)
      item = new CompositeRuleItem (parent,
                                    entity->getAttribute("id"),
                                    entity->getAttribute("operator"),
                                    COMPOSITE_TYPE, _rulesTable);
    else{
      item = new CompositeRuleItem (_rulesTable,
                                    entity->getAttribute("id"),
                                    entity->getAttribute("operator"),
                                    COMPOSITE_TYPE, _rulesTable);
    }

  }

  item->setExpanded(true);
  _items.insert(item, entity);
}

void RulesViewPlugin::updateValue(QTreeWidgetItem* item)
{
  if (item)
  {
    QMap <QString, QString> attr;
    RuleItem *ruleItem = dynamic_cast <RuleItem *> (item);
    if (ruleItem)
    {
      attr.insert("id", ruleItem->id());
      attr.insert("var", ruleItem->var());
      attr.insert("comparator", ruleItem->comparator());
      attr.insert("value", ruleItem->value());

      Entity *entity = _items.value(item);
      if (entity)
        emit setAttributes(entity, attr, true);
    }
    else
    {
      CompositeRuleItem *compositeRuleItem = dynamic_cast <CompositeRuleItem *>
          (item);
      if (compositeRuleItem)
      {
        attr.insert("id", compositeRuleItem->id());
        attr.insert("operator", compositeRuleItem->getOperator());

        Entity *entity = _items.value(item);
        if (entity)
          emit setAttributes(entity, attr, true);
      }
    }
  }
}

RulesViewPlugin::~RulesViewPlugin()
{
  delete _rulesTable;
}
