#ifndef COMBOBOXITEM_H
#define COMBOBOXITEM_H

#include <QComboBox>
#include <QTreeWidgetItem>

class ComboBoxItem : public QComboBox
{
  Q_OBJECT
public:
  explicit ComboBoxItem(QTreeWidgetItem*, int, QWidget *parent=0);

signals:

public slots:
  void changeItem(int);

private:
  QTreeWidgetItem *_item;
  int _column;
};

#endif // COMBOBOXITEM_H
