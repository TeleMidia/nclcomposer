#ifndef RULEITEM_H
#define RULEITEM_H

#include <QTreeWidgetItem>
#include "comboboxitem.h"
#include "util.h"

class RuleItem : public QObject, public QTreeWidgetItem
{
  Q_OBJECT
public:
  explicit RuleItem(QTreeWidget *view, const QString &id, const QString &var,
                    const QString &comparator, const QString &value,
                    const int &type, QWidget *parent);

  explicit RuleItem(QTreeWidgetItem *item, const QString &id,const QString &var,
                    const QString &comparator, const QString &value,
                    const int &type, QWidget *parent = 0);

  inline QString getId () const
  { return text(ID_COLUMN); }

  inline QString getVar () const
  { return _varCombo->currentText(); }

  inline QString getComparator () const
  { return _comparatorCombo->currentText(); }

  inline QString getValue () const
  { return text(VALUE_COLUMN); }

  inline void setId (const QString &id)
  { setText (ID_COLUMN, id); }

  inline void setVar (const QString &var)
  { changeComboValue(_varCombo, var); }

  inline void setComparator (const QString &comparator)
  { changeComboValue(_comparatorCombo, comparator); }

  inline void setValue (const QString &value)
  { setText(VALUE_COLUMN, value); }

  inline void changeComboValue(QComboBox *combo, const QString &value)
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
  void init (const QString &id, const QString &var, const QString &comparator,
             const QString &value);
  void changeComboValue (QComboBox &combo, const QString &comparator);

  static QStringList _varList;
  static QStringList _cmpList;

  QString _comparator;

  QLineEdit * _idEdit;
  QLineEdit * _valueEdit;
  ComboBoxItem *_varCombo;
  ComboBoxItem *_comparatorCombo;
};

#endif // RULEITEM2_H
