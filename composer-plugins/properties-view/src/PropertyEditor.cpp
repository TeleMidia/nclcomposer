#include "PropertyEditor.h"

#include <map>
#include "NCLStructure.h"
using namespace composer::language;

PropertyEditor::PropertyEditor(QWidget *parent):
        QWidget(parent),
        ui(new Ui::PropertyEditorWidget)
{
    ui->setupUi(this);
}

PropertyEditor::~PropertyEditor()
{

}

void PropertyEditor::setTagname(QString tagname)
{
    //Clear previos items
    propertyToLine.clear();
    for(int i = 0; i < currentItems.size(); i++)
        delete currentItems.at(i);
    currentItems.clear();
    while(ui->tableWidget->rowCount())
        ui->tableWidget->removeRow(0);

    // add the new ones
    map <QString, bool> *attrs =
            NCLStructure::getInstance()->getAttributes(tagname);
    map <QString, bool>::iterator it;

    int i;
    for(i=0,it = attrs->begin(); it != attrs->end(); ++it, i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem((*it).first);
        currentItems.push_back(item);
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
        ui->tableWidget->setItem(line, 1, item);
    }
}
