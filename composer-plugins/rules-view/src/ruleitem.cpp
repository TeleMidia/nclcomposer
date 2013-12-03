#include "include/ruleitem.h"

#include <core/extensions/NCLStructure.h>

using namespace composer::language;

int RuleItem::ID = 0;
int RuleItem::VAR = 1;
int RuleItem::COMP = 2;
int RuleItem::VALUE = 3;

QStringList RuleItem::_varList;
QStringList RuleItem::_cmpList;

RuleItem::RuleItem(QTreeWidget *view, QString id, QString var,
                   QString comparator, QString value, int type, QWidget *parent) :
  QTreeWidgetItem(view, type), QObject (parent)
{
  init(id, var, comparator, value);
}

RuleItem::RuleItem(QTreeWidgetItem *item, QString id, QString var,
                   QString comparator, QString value, int type, QWidget *parent) :
  QTreeWidgetItem(item, type), QObject (parent)
{
  init(id, var, comparator, value);
}


void RuleItem::init(QString id, QString var, QString comparator, QString value)
{
  _id = id;
  _var = var;
  _comparator = comparator;
  _value = value;

  setText(ID, id);
  setText(VAR, var);
  setText(COMP, comparator);
  setText(VALUE, value);

  setFlags(flags() | Qt::ItemIsEditable);

  NCLStructure *instance = NCLStructure::getInstance();

  if (_varList.isEmpty())
    _varList = instance->getDatatypeDefaultSuggestions
        (instance->getAttributeDatatype("rule", "var"));

  if (_cmpList.isEmpty())
    _cmpList = instance->getDatatypeDefaultSuggestions
        (instance->getAttributeDatatype("rule", "comparator"));

  _varCombo = new ComboBoxItem (this, VAR);
  _varCombo->addItems(_varList);
  _varCombo->setEditable(true);
  _varCombo->setEditText(_var);

  _comparatorCombo = new ComboBoxItem (this, COMP);
  _comparatorCombo->addItems(_cmpList);

  treeWidget()->setItemWidget(this, VAR, _varCombo);
  treeWidget()->setItemWidget(this, COMP, _comparatorCombo);
}
