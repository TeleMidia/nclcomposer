/*
 * Copyright (C) 2011-2018 TeleMidia/PUC-Rio.
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
#include "PropertiesEditor.h"

#include <deque>

#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRect>
#include <QSpinBox>
#include <QTableWidget>
#include <QWidget>

#include "PropertyButton.h"

#include "NCLStructure.h"
using namespace cpr::nclprofile;

PropertiesEditor::PropertiesEditor (QWidget *parent) : QWidget (parent)
{
  _ui = new Ui::PropertyEditorWidget ();
  _ui->setupUi (this);

  ComboBoxDelegate *delegate = new ComboBoxDelegate (this);
  _ui->tableWidget->setItemDelegate (delegate);
  delegate->setTableWidget (_ui->tableWidget);

  connect (_ui->tableWidget, SIGNAL (itemChanged (QTableWidgetItem *)), this,
           SLOT (updateWithItemChanges (QTableWidgetItem *)),
           Qt::DirectConnection);

  connect (_ui->filterLineEdit, SIGNAL (filterTextChanged (const QString &)),
           this, SLOT (filterProperties (const QString &)));

  _internalPropertyChange = false;

  _ui->treeView->setVisible (WITH_TREEVIEW);

#if WITH_TREEVIEW
  standardModel = new QStandardItemModel (0, 2);
  attributesRootItem = new QStandardItem ("Common");
  propertiesRootItem = new QStandardItem ("Properties");
  standardModel->invisibleRootItem ()->appendRow (attributesRootItem);
  standardModel->invisibleRootItem ()->appendRow (propertiesRootItem);
  ui->treeView->setModel (standardModel);
  ui->treeView->expandAll ();
#endif
}

PropertiesEditor::~PropertiesEditor () {}

void
PropertiesEditor::setTagname (const QString &tagname, const QString &name,
                              const QStringList &attrs,
                              const QStringList &attrs_datatype,
                              const QList<QStringList> &attrs_suggestions)
{
  this->_currentTagname = tagname;
  ComboBoxDelegate *delegate
      = qobject_cast<ComboBoxDelegate *> (_ui->tableWidget->itemDelegate ());

  if (delegate)
    delegate->setCurrentTagname (tagname);

  // Clear previous items
  _attrs.clear ();
  _attrDatatype.clear ();
  _attr2Line.clear ();
  _attrValue.clear ();
  _attrValueSuggestions.clear ();

  while (_ui->tableWidget->rowCount ())
    _ui->tableWidget->removeRow (0);

  setCurrentName (name);

  int i = 0;
  foreach (const QString &a, attrs)
  {
    _attrValue[a] = "";
    _attrs.push_back (a);

    if (attrs_suggestions.size ())
    {
      setAttributeSuggestions (a, attrs_suggestions.at (i));
    }

    i++;
  }

  _attrDatatype = attrs_datatype;

  filterProperties (this->_currentFilterString);
}

void
PropertiesEditor::setCurrentName (const QString &name)
{
  this->_currentName = name;
  _ui->label->setText (_currentTagname + ":" + _currentName);
}

void
PropertiesEditor::setErrorMessage (const QString &errorMessage)
{
  if (!errorMessage.isEmpty ())
    _ui->label_ErrorMessage->setText (" (" + errorMessage + ")");
  else
    _ui->label_ErrorMessage->setText ("");

  if (!errorMessage.isNull () && !errorMessage.isEmpty ())
  {
    _ui->frame_Name->setStyleSheet ("color: red;");
  }
  else
    _ui->frame_Name->setStyleSheet ("color: black;");

  // ui->frame_Name->adjustSize();
}

void
PropertiesEditor::setAttributeValue (const QString &property,
                                     const QString &value)
{
  // Set the attibute just if this property is a valid property of the current
  // tagname.  Also, if propertyToLine does not contains property it is not
  // been showed by the filter.
  if (_attr2Line.contains (property))
  {
    int line = _attr2Line.value (property);

    QTableWidgetItem *item = _ui->tableWidget->item (line, 1);
    if (item)
    {
      // Try to update if the values are not equal
      if (item->text () != value)
      {
        _internalPropertyChange = true;
        item->setText (value);
        _attrValue[property] = value;
      }
    }
  }
}

void
PropertiesEditor::setAttributeSuggestions (const QString &property,
                                           const QStringList &suggestions)
{
  _attrValueSuggestions[property] = suggestions;
}

QStringList
PropertiesEditor::getAttributeSuggestions (const QString &property)
{
  if (_attrValueSuggestions.contains (property))
    return _attrValueSuggestions[property];
  else
    return QStringList ();
}

QString
PropertiesEditor::getAttributeDatatype (const QString &attr)
{
  return NCLStructure::instance ()->getAttributeDatatype (_currentTagname,
                                                          attr);
}

void
PropertiesEditor::updateWithItemChanges (QTableWidgetItem *item)
{
  int row = _ui->tableWidget->row (item);
  int column = _ui->tableWidget->column (item);

  if (column == 0)
    return; // not for me!

  if (_internalPropertyChange)
  {
    _internalPropertyChange = false;
    return;
  }

  QTableWidgetItem *leftItem = _ui->tableWidget->item (row, column - 1);
  QString name = "", value = "";

  if (item != nullptr)
  {
    name = leftItem->text ();
  }
  else
    return; // property with empty name isn't usefull

  if (item != nullptr)
  {
    value = item->text ();
  }

  _attrValue[name] = value; // update internal map
  emit propertyChanged (name, value);
}

void
PropertiesEditor::filterProperties (const QString &text)
{
  this->_currentFilterString = text;
  while (_ui->tableWidget->rowCount ())
    _ui->tableWidget->removeRow (0);

  _attr2Line.clear ();

#if WITH_TREEVIEW
  while (attributesRootItem->rowCount ())
    attributesRootItem->removeRow (0);
#endif

  foreach (const QString &attr, _attrs)
  {
    if (attr.toLower ().startsWith (text.toLower ()))
    {
      QTableWidgetItem *item = new QTableWidgetItem (attr);
      QTableWidgetItem *itemValue = new QTableWidgetItem (_attrValue[attr]);

#if WITH_TREEVIEW
      QList<QStandardItem *> property;
      property << new QStandardItem (attr);
      property << new QStandardItem (propertyToValue[attr]);
      attributesRootItem->appendRow (property);
#endif

      _ui->tableWidget->insertRow (_ui->tableWidget->rowCount ());
      _internalPropertyChange = true;
      _ui->tableWidget->setItem (_ui->tableWidget->rowCount () - 1, 0, item);
      _attr2Line.insert (attr, _ui->tableWidget->rowCount () - 1);

      _internalPropertyChange = true;
      _ui->tableWidget->setItem (_ui->tableWidget->rowCount () - 1, 1,
                                 itemValue);
    }
  }
}
