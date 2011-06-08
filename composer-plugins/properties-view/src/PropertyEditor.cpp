#include "PropertyEditor.h"

#include <map>
#include "NCLStructure.h"
using namespace composer::language;

PropertyEditor::PropertyEditor(QWidget *parent):
        QWidget(parent)
{
    ui = new Ui::PropertyEditorWidget();
    ui->setupUi(this);

    connect(ui->tableWidget, SIGNAL(cellChanged(int,int)),
            this, SLOT(udpateEntityWithCellContent(int,int)));
}

PropertyEditor::~PropertyEditor()
{

}

void PropertyEditor::setTagname(QString tagname, QString name)
{
    //Clear previos items
    propertyToLine.clear();
    for(int i = 0; i < currentItems.size(); i++)
        delete currentItems.at(i);
    currentItems.clear();
    while(ui->tableWidget->rowCount())
        ui->tableWidget->removeRow(0);

    ui->label->setText(tagname + ":" + name);
    // add the new ones
    map <QString, bool> *attrs =
            NCLStructure::getInstance()->getAttributes(tagname);
    map <QString, bool>::iterator it;

    int i;
    for(i=0,it = attrs->begin(); it != attrs->end(); ++it, i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem((*it).first);
        /* make the item not editable */
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        currentItems.push_back(item);
        internalPropertyChange = true;
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, item);
        propertyToLine.insert((*it).first, ui->tableWidget->rowCount()-1);
    }
}

void PropertyEditor::setAttributeValue(QString property, QString value)
{
    // Set the attibute just if this property is a valid property of the current
    // tagname.
    if(propertyToLine.contains(property))
    {
        int line = propertyToLine.value(property);
        QTableWidgetItem *item = new QTableWidgetItem(value);
        currentItems.push_back(item);
        internalPropertyChange = true;
        ui->tableWidget->setItem(line, 1, item);
    }
}

void PropertyEditor::udpateEntityWithCellContent(int row, int column)
{
    if(column == 0) return; //not for me!

    if(internalPropertyChange){
        internalPropertyChange = false;
        return;
    }

    QTableWidgetItem *item = ui->tableWidget->item(row, column-1);
    QString name = "", value = "";
    if(item != NULL)
    {
        name = item->text();
    }

    item = ui->tableWidget->item(row, column);
    if(item != NULL)
    {
        value = item->text();
    }

    emit propertyChanged(name, value);
}
