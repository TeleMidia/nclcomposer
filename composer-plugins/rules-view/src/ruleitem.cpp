#include "include/ruleitem.h"

#include <core/extensions/NCLStructure.h>
#include <QDebug>


using namespace composer::language;

QStringList RuleItem::_varList;
QStringList RuleItem::_cmpList;

RuleItem::RuleItem(QTreeWidget *view, QString id, QString var,
                     QString comparator, QString value, int type,
                     QWidget *parent)
  :  QObject (parent), QTreeWidgetItem (view, type)
{
  init (id, var, comparator, value);
}

RuleItem::RuleItem(QTreeWidgetItem *item, QString id, QString var,
                     QString comparator, QString value, int type,
                     QWidget *parent)
  : QObject (parent), QTreeWidgetItem (item, type)
{
  init (id, var, comparator, value);
}

void RuleItem::init(QString id, QString var, QString comparator, QString value)
{
  _id = id;
  _var = var;
  _comparator = comparator;
  _value = value;

  NCLStructure *instance = NCLStructure::getInstance();

  if (_varList.isEmpty())
    _varList = instance->getDatatypeDefaultSuggestions
        (instance->getAttributeDatatype(RULE_LABEL, VAR_ATTR));

  if (_cmpList.isEmpty())
    _cmpList = instance->getDatatypeDefaultSuggestions
        (instance->getAttributeDatatype(RULE_LABEL, COMPARATOR_ATTR));

  _varCombo = new ComboBoxItem (this, VAR_COLUMN);
  _varCombo->addItems(_varList);
  _varCombo->setEditable(true);
  _varCombo->setEditText(_var);

  _comparatorCombo = new ComboBoxItem (this, COMP_COLUMN);
  _comparatorCombo->addItems(_cmpList);

  QTreeWidgetItem::setText(ELEMENT_COLUMN, RULE_LABEL);
  QTreeWidgetItem::setText(ID_COLUMN, id);
  QTreeWidgetItem::setText(VAR_COLUMN, var);
  QTreeWidgetItem::setText(COMP_COLUMN, comparator);
  QTreeWidgetItem::setText(VALUE_COLUMN, value);

  treeWidget()->setItemWidget(this, VAR_COLUMN, _varCombo);
  treeWidget()->setItemWidget(this, COMP_COLUMN, _comparatorCombo);

  setFlags(flags() | Qt::ItemIsEditable);
}
