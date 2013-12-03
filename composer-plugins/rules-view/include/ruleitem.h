#ifndef RULEITEM_H
#define RULEITEM_H

#include <QTreeWidgetItem>
#include <QLineEdit>
#include "comboboxitem.h"

class RuleItem : public QObject, public QTreeWidgetItem
{
  Q_OBJECT
public:
  explicit RuleItem(QTreeWidget *view, QString id, QString var,
                    QString comparator, QString value,
                    int type, QWidget *parent = 0);

  explicit RuleItem(QTreeWidgetItem *item, QString id, QString var,
                    QString comparator, QString value,
                    int type, QWidget *parent = 0);

  inline QString id () const { return text(ID); }
  inline QString var () const { return _varCombo->currentText(); }
  inline QString comparator () const { return _comparatorCombo->currentText(); }
  inline QString value () const { return text(VALUE); }

signals:

public slots:

private:
  void init (QString id, QString var, QString comparator, QString value);

  static int ID;
  static int VAR;
  static int COMP;
  static int VALUE;

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

#endif // RULEITEM_H

