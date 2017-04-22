/*
 * Copyright 2011 TeleMidia/PUC-Rio.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "PropertyEditor.h"

#include <deque>

#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QRect>

#include "PropertyButton.h"
#include "NCLStructure.h"

using namespace composer::language;

PropertyEditor::PropertyEditor(QWidget *parent):
  QWidget(parent)
{
  _ui = new Ui::PropertyEditorWidget();
  _ui->setupUi(this);

  ComboBoxDelegate *delegate = new ComboBoxDelegate();
  _ui->tableWidget->setItemDelegate(delegate);
  delegate->setTableWidget(_ui->tableWidget);

  connect(_ui->tableWidget,
          SIGNAL(itemChanged(QTableWidgetItem *)),
          this,
          SLOT(updateWithItemChanges(QTableWidgetItem *)),
          Qt::DirectConnection);

  connect(_ui->filterLineEdit,
          SIGNAL(filterTextChanged(const QString&)),
          this,
          SLOT(filterProperties(const QString&)));

  _internalPropertyChange = false;

  _ui->treeView->setVisible(WITH_TREEVIEW);

#if WITH_TREEVIEW
  standardModel = new QStandardItemModel(0, 2);
  attributesRootItem = new QStandardItem("Common");
  propertiesRootItem = new QStandardItem("Properties");
  standardModel->invisibleRootItem()->appendRow(attributesRootItem);
  standardModel->invisibleRootItem()->appendRow(propertiesRootItem);
  ui->treeView->setModel(standardModel);
  ui->treeView->expandAll();
#endif
}

PropertyEditor::~PropertyEditor()
{

}

void PropertyEditor::setTagname(QString tagname, QString name)
{
  this->_currentTagname = tagname;
  //this->currentFilterString = "";
  ComboBoxDelegate *delegate =
      qobject_cast<ComboBoxDelegate *> (_ui->tableWidget->itemDelegate());

  if(delegate)
    delegate->setCurrentTagname(tagname);

  //Clear previous items
  _propertyToLine.clear();
  _propertyToValue.clear();
  _orderedProperties.clear();

  while(_ui->tableWidget->rowCount())
    _ui->tableWidget->removeRow(0);

  setCurrentName(name);

  // add the new ones
  deque <QString> *attrs =
      NCLStructure::getInstance()->getAttributesOrdered(_currentTagname);

  if(attrs != nullptr)
  {
    deque <QString>::iterator it;

    int i;
    for(i=0, it = attrs->begin(); it != attrs->end(); ++it, i++)
    {
      QString currentAttr = (*it);
      _propertyToValue[currentAttr] = "";
      _orderedProperties.push_back(currentAttr);
    }
  }

  filterProperties(this->_currentFilterString);
}

void PropertyEditor::setCurrentName(QString name)
{
  this->_currentName = name;
  _ui->label->setText(_currentTagname + ":" + _currentName);
}

void PropertyEditor::setErrorMessage(QString errorMessage)
{
  if(!errorMessage.isEmpty())
    _ui->label_ErrorMessage->setText(" (" + errorMessage + ")");
  else
    _ui->label_ErrorMessage->setText("");

  if(!errorMessage.isNull() && !errorMessage.isEmpty())
  {
    _ui->frame_Name->setStyleSheet("color: red;");
  }
  else
    _ui->frame_Name->setStyleSheet("color: black;");

  // ui->frame_Name->adjustSize();
}

void PropertyEditor::setAttributeValue(QString property, QString value)
{
  // Set the attibute just if this property is a valid property of the current
  // tagname.
  // Also, if propertyToLine does not contains property it is not been showed by
  // the filter.
  if( _propertyToValue.contains(property) &&
      _propertyToLine.contains(property) )
  {
    int line = _propertyToLine.value(property);

    QTableWidgetItem *item = _ui->tableWidget->item(line, 1);
    if(item)
    {
      // Try to update if the values are not equal
      if(item->text() != value)
      {
        _internalPropertyChange = true;
        item->setText(value);
        _propertyToValue[property] = value;
      }
    }
  }
}

void PropertyEditor::updateWithItemChanges(QTableWidgetItem *item)
{
  //  qDebug() << "updateWithItemChanges " << internalPropertyChange;
  int row = _ui->tableWidget->row(item);
  int column = _ui->tableWidget->column(item);

  if(column == 0) return; //not for me!

  if(_internalPropertyChange)
  {
    _internalPropertyChange = false;
    return;
  }

  QTableWidgetItem *leftItem = _ui->tableWidget->item(row, column-1);
  QString name = "", value = "";

  if(item != nullptr)
  {
    name = leftItem->text();
  }
  else return; //property with empty name isn't usefull

  if(item != nullptr)
  {
    value = item->text();
  }

  _propertyToValue[name] = value; //update internal map
  emit propertyChanged(name, value);
}

void PropertyEditor::filterProperties(const QString& text)
{
  this->_currentFilterString = text;
  while(_ui->tableWidget->rowCount())
    _ui->tableWidget->removeRow(0);

  _propertyToLine.clear();

#if WITH_TREEVIEW
  while(attributesRootItem->rowCount())
    attributesRootItem->removeRow(0);
#endif

  QString attr;
  deque <QString>::iterator it;

  for (it = _orderedProperties.begin(); it != _orderedProperties.end(); ++it)
    // foreach( attr, propertyToValue.keys() )
  {
    attr = *it;
    if(attr.toLower().startsWith(text.toLower()))
    {
      QTableWidgetItem *item = new QTableWidgetItem(attr);
      QTableWidgetItem *itemValue = new QTableWidgetItem(_propertyToValue[attr]);

#if WITH_TREEVIEW
      QList<QStandardItem *> property;
      property << new QStandardItem(attr);
      property << new QStandardItem(propertyToValue[attr]);
      attributesRootItem->appendRow(property);
#endif

      _ui->tableWidget->insertRow(_ui->tableWidget->rowCount());
      _internalPropertyChange = true;
      _ui->tableWidget->setItem(_ui->tableWidget->rowCount()-1, 0, item);
      _propertyToLine.insert(attr, _ui->tableWidget->rowCount()-1);

      _internalPropertyChange = true;
      _ui->tableWidget->setItem(_ui->tableWidget->rowCount()-1, 1, itemValue);
    }
  }
}
