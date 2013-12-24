#ifndef RULEITEM_H
#define RULEITEM_H

#include <QTreeWidgetItem>
#include "comboboxitem.h"
#include "util.h"

class RuleItem : public QObject, public QTreeWidgetItem
{
  Q_OBJECT
public:
  explicit RuleItem(QTreeWidget *view, QString id, QString var,
                      QString comparator, QString value, int type,
                      QWidget *parent);

  explicit RuleItem(QTreeWidgetItem *item, QString id, QString var,
                      QString comparator, QString value,
                      int type, QWidget *parent = 0);

  inline QString id () const { return text(ID_COLUMN); }
  inline QString var () const { return _varCombo->currentText(); }
  inline QString comparator () const { return _comparatorCombo->currentText(); }
  inline QString value () const { return text(VALUE_COLUMN); }

  inline void setId (QString id) { setText (ID_COLUMN, id); }
  inline void setVar (QString var)
  {
    changeComboValue(_varCombo, var);
  }

  inline void setComparator (QString comparator)
  {
    changeComboValue(_comparatorCombo, comparator);
  }
  inline void setValue (QString value) { setText(VALUE_COLUMN, value); }

  inline void changeComboValue(QComboBox *combo, QString value)
  {
    int index = combo->findText(value);
    if (index != -1)
      combo->setCurrentIndex(index);
    else
    {
      combo->setItemText(0, value);
      combo->setCurrentIndex(0);
    }
  }

private:
  void init (QString id, QString var, QString comparator, QString value);
  void changeComboValue (QComboBox &combo, QString value);

  static QStringList _varList;
  static QStringList _cmpList;

  QString _comparator;

  QLineEdit * _idEdit;
  QLineEdit * _valueEdit;
  ComboBoxItem *_varCombo;
  ComboBoxItem *_comparatorCombo;
};

#endif // RULEITEM2_H
