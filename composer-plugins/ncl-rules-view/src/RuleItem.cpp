#include "RuleItem.h"

#include <extensions/NCLStructure.h>
#include <QDebug>


using namespace composer::language;

QStringList RuleItem::_varList;
QStringList RuleItem::_cmpList;

RuleItem::RuleItem(QTreeWidget *view, const QString &id, const QString &var,
                   const QString &comparator, const QString &value,
                   const int &type, QWidget *parent)
  :  QObject (parent), QTreeWidgetItem (view, type)
{
  init (id, var, comparator, value);
}

RuleItem::RuleItem(QTreeWidgetItem *item, const QString &id, const QString &var,
                   const QString &comparator, const QString &value,
                   const int &type, QWidget *parent)
  : QObject (parent), QTreeWidgetItem (item, type)
{
  init (id, var, comparator, value);
}

void RuleItem::init(const QString &id, const QString &var,
                    const QString &comparator, const QString &value)
{
  _comparator = comparator;

  NCLStructure *instance = NCLStructure::getInstance();

  if (_varList.isEmpty())
    _varList = instance->getDatatypeDefaultSuggestions
        (instance->getAttributeDatatype(RULE_LABEL, VAR_ATTR));

  if (_cmpList.isEmpty())
    _cmpList.append(instance->getDatatypeDefaultSuggestions
                    (instance->getAttributeDatatype(RULE_LABEL,
                                                    COMPARATOR_ATTR)));

  _varCombo = new ComboBoxItem (this, VAR_COLUMN);
  _varCombo->addItem("");
  _varCombo->addItems(_varList);
  _varCombo->setEditable(true);
  _varCombo->setEditText(var);

  _comparatorCombo = new ComboBoxItem (this, COMP_COLUMN);
  _comparatorCombo->addItem("");
  _comparatorCombo->addItems(_cmpList);

  changeComboValue(_varCombo, var);
  changeComboValue(_comparatorCombo, comparator);

  QTreeWidgetItem::setText(ELEMENT_COLUMN, RULE_LABEL);
  QTreeWidgetItem::setText(ID_COLUMN, id);
  QTreeWidgetItem::setText(VAR_COLUMN, var);
  QTreeWidgetItem::setText(COMP_COLUMN, comparator);
  QTreeWidgetItem::setText(VALUE_COLUMN, value);

  treeWidget()->setItemWidget(this, VAR_COLUMN, _varCombo);
  treeWidget()->setItemWidget(this, COMP_COLUMN, _comparatorCombo);

  setFlags(flags() | Qt::ItemIsEditable);
}
