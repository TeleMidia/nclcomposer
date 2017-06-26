#include "ComboBoxItem.h"

ComboBoxItem::ComboBoxItem (QTreeWidgetItem *item, int column, QWidget *parent)
    : QComboBox (parent)
{
  _item = item;
  _column = column;
  connect (this, SIGNAL (currentIndexChanged (int)), SLOT (changeItem (int)));
}

void
ComboBoxItem::changeItem (int index)
{
  if (index >= 0)
  {
    _item->setData (this->_column, Qt::UserRole, this->itemText (index));
  }
}
