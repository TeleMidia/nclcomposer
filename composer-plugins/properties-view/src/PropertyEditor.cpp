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
  ui = new Ui::PropertyEditorWidget();
  ui->setupUi(this);

  ComboBoxDelegate *delegate = new ComboBoxDelegate();
  ui->tableWidget->setItemDelegate(delegate);
  delegate->setTableWidget(ui->tableWidget);

  connect(ui->tableWidget,
          SIGNAL(itemChanged(QTableWidgetItem *)),
          this,
          SLOT(updateWithItemChanges(QTableWidgetItem *)),
          Qt::DirectConnection);

  connect(ui->filterLineEdit,
          SIGNAL(filterTextChanged(const QString&)),
          this,
          SLOT(filterProperties(const QString&)));

  internalPropertyChange = false;

  ui->treeView->setVisible(WITH_TREEVIEW);
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
  this->currentTagname = tagname;
  //this->currentFilterString = "";
  ComboBoxDelegate *delegate =
      qobject_cast<ComboBoxDelegate *> (ui->tableWidget->itemDelegate());

  if(delegate)
    delegate->setCurrentTagname(tagname);

  //Clear previous items
  propertyToLine.clear();
  propertyToValue.clear();
  orderedProperties.clear();

  while(ui->tableWidget->rowCount())
    ui->tableWidget->removeRow(0);

  setCurrentName(name);

  // add the new ones
  deque <QString> *attrs =
      NCLStructure::getInstance()->getAttributesOrdered(currentTagname);

  if(attrs != NULL)
  {
    deque <QString>::iterator it;

    int i;
    for(i=0, it = attrs->begin(); it != attrs->end(); ++it, i++)
    {
      QString currentAttr = (*it);
      propertyToValue[currentAttr] = "";
      orderedProperties.push_back(currentAttr);
    }
  }

  filterProperties(this->currentFilterString);
}

void PropertyEditor::setCurrentName(QString name)
{
  this->currentName = name;
  ui->label->setText(currentTagname + ":" + currentName);
}

void PropertyEditor::setErrorMessage(QString errorMessage)
{
  if(!errorMessage.isEmpty())
    ui->label_ErrorMessage->setText(" (" + errorMessage + ")");
  else
    ui->label_ErrorMessage->setText("");

  if(!errorMessage.isNull() && !errorMessage.isEmpty())
  {
    ui->frame_Name->setStyleSheet("color: red;");
  }
  else
    ui->frame_Name->setStyleSheet("color: black;");

  // ui->frame_Name->adjustSize();
}

void PropertyEditor::setAttributeValue(QString property, QString value)
{
  // Set the attibute just if this property is a valid property of the current
  // tagname.
  // Also, if propertyToLine does not contains property it is not been showed by
  // the filter.
  if( propertyToValue.contains(property) &&
      propertyToLine.contains(property) )
  {
    int line = propertyToLine.value(property);

    // \todo This must be improved to use NCLStructure
    if(isURL(currentTagname, property))
    {
      PropertyButtons *item = dynamic_cast <PropertyButtons*>
          (ui->tableWidget->cellWidget(line, 1));
      if(item)
      {
        // Try to update if the values are not equal
        if(item->text() != value)
        {
          //          internalPropertyChange = true;
          item->setText(value);
          propertyToValue[property] = value;
        }
      }
    }
    else
    {
      QTableWidgetItem *item = ui->tableWidget->item(line, 1);
      if(item)
      {
        // Try to update if the values are not equal
        if(item->text() != value)
        {
          internalPropertyChange = true;
          item->setText(value);
          propertyToValue[property] = value;
        }
      }
    }
  }
}

void PropertyEditor::updateWithItemChanges(QTableWidgetItem *item)
{
  //  qDebug() << "updateWithItemChanges " << internalPropertyChange;
  int row = ui->tableWidget->row(item);
  int column = ui->tableWidget->column(item);

  if(column == 0) return; //not for me!

  if(internalPropertyChange)
  {
    internalPropertyChange = false;
    return;
  }

  QTableWidgetItem *leftItem = ui->tableWidget->item(row, column-1);
  QString name = "", value = "";

  if(item != NULL)
  {
    name = leftItem->text();
  }
  else return; //property with empty name isn't usefull

  if(item != NULL)
  {
    value = item->text();
  }

  propertyToValue[name] = value; //update internal map
  emit propertyChanged(name, value);
}

void PropertyEditor::filterProperties(const QString& text)
{
  this->currentFilterString = text;
  while(ui->tableWidget->rowCount())
    ui->tableWidget->removeRow(0);

  propertyToLine.clear();

#if WITH_TREEVIEW
  while(attributesRootItem->rowCount())
    attributesRootItem->removeRow(0);
#endif

  QString attr;
  deque <QString>::iterator it;

  for (it = orderedProperties.begin(); it != orderedProperties.end(); ++it)
    // foreach( attr, propertyToValue.keys() )
  {
    attr = *it;
    if(attr.toLower().startsWith(text.toLower()))
    {
      QTableWidgetItem *item = new QTableWidgetItem(attr);
      QTableWidgetItem *itemValue = new QTableWidgetItem(propertyToValue[attr]);

#if WITH_TREEVIEW
      QList<QStandardItem *> property;
      property << new QStandardItem(attr);
      property << new QStandardItem(propertyToValue[attr]);
      attributesRootItem->appendRow(property);
#endif

      ui->tableWidget->insertRow(ui->tableWidget->rowCount());
      internalPropertyChange = true;
      ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, item);
      propertyToLine.insert(attr, ui->tableWidget->rowCount()-1);

      internalPropertyChange = true;
      ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, itemValue);

      if(isURL(currentTagname, attr))
      {
        PropertyButtons *control = new PropertyButtons( attr );
        ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1, 1,
                                       control);

        control->setText(propertyToValue[attr]);
        control->show();

        QObject::connect(control,SIGNAL(newValue(QString,QString)),
                         SIGNAL(propertyChanged(QString,QString)));
      }
    }
  }
}


bool PropertyEditor::isURL(const QString &tagname, const QString &attr)
{
  /*! \todo This must be improved to use NCLStructure */
  return (attr == "src" ||
          attr == "focusSrc" ||
          attr == "focusSelSrc" ||
          attr == "documentURI");
}
