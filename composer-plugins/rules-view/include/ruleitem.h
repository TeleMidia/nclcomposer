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

private:
  void init (QString id, QString var, QString comparator, QString value);

  static QStringList _varList;
  static QStringList _cmpList;

  QString _id;
  QString _var;
  QString _comparator;
  QString _value;

  QLineEdit * _idEdit;
  QLineEdit * _valueEdit;
  ComboBoxItem *_varCombo;
  ComboBoxItem *_comparatorCombo;
};

#endif // RULEITEM2_H
