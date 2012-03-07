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
#include<propertybuttons.h>
#include <QMap>
#include <QSpinBox>
#include "NCLStructure.h"
#include<QPushButton>
#include<QTableWidget>
#include<QHBoxLayout>
#include<QWidget>
#include<QFileDialog>
#include<QRect>
using namespace composer::language;

PropertyEditor::PropertyEditor(QWidget *parent):
    QWidget(parent)
{
    ui = new Ui::PropertyEditorWidget();
    ui->setupUi(this);

    connect(    ui->tableWidget,
                SIGNAL(itemChanged(QTableWidgetItem *)),
                this,
                SLOT(updateWithItemChanges(QTableWidgetItem *)));

    connect(    ui->filterLineEdit,
                SIGNAL(filterTextChanged(const QString&)),
                this,
                SLOT(filterProperties(const QString&)));
}

PropertyEditor::~PropertyEditor()
{

}

void PropertyEditor::setTagname(QString tagname, QString name)
{
    this->currentName = name;
    this->currentTagname = tagname;
    //this->currentFilterString = "";

    //Clear previous items
    propertyToLine.clear();
    propertyToValue.clear();

    while(ui->tableWidget->rowCount())
        ui->tableWidget->removeRow(0);

    qDebug() << "properties-view !!!";

    ui->label->setText(currentTagname + ":" + currentName);

    // add the new ones
    map <QString, bool> *attrs =
            NCLStructure::getInstance()->getAttributes(currentTagname);

    if(attrs != NULL)
    {
        map <QString, bool>::iterator it;

        int i;
        for(i=0,it = attrs->begin(); it != attrs->end(); ++it, i++)
        {
            QString currentAttr = (*it).first;
            propertyToValue[currentAttr] = "";
        }
    }
    filterProperties(this->currentFilterString);
}

void PropertyEditor::setAttributeValue(QString property, QString value)
{
    // Set the attibute just if this property is a valid property of the current
    // tagname.
    if(propertyToValue.contains(property))
    {
        int line = propertyToLine.value(property);
        if(property == "src" || property =="focusSelSrc" || property=="focusSrc")
        {
            QPushButton *item = (QPushButton*) ui->tableWidget->cellWidget(line, 1);
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
    int row = ui->tableWidget->row(item);
    int column = ui->tableWidget->column(item);

    if(column == 0) return; //not for me!

    if(internalPropertyChange) {
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

    QString key1;
    foreach( key1, propertyToValue.keys() )
    {
        if(key1.startsWith(text))
        {
            QTableWidgetItem *item = new QTableWidgetItem(key1);
            QTableWidgetItem *itemValue = new QTableWidgetItem(propertyToValue[key1]);

            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            internalPropertyChange = true;
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, item);
            propertyToLine.insert(key1, ui->tableWidget->rowCount()-1);
            internalPropertyChange = true;
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, itemValue);

            if (key1  == "src"||key1=="focusSelSrc" || key1=="focusSrc")
            {
               QPushButton *explorer=new QPushButton;
               explorer->setText("Explorer");
               propertybuttons *control = new propertybuttons( key1 );


               ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-1, 1,
                                              explorer);

               explorer->setText(propertyToValue[key1]);


                 qDebug() << "explorer2!!";
                 explorer->show();


                 QObject::connect(explorer,SIGNAL(clicked()),control,SLOT(openfile()));
                 QObject::connect(control,SIGNAL(newValue(QString,QString)),
                         SIGNAL(propertyChanged(QString,QString)));

        }
    }
}
}





