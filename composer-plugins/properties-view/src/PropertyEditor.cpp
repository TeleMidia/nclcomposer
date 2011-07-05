#include "PropertyEditor.h"

#include <map>
#include "NCLStructure.h"
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
            QTableWidgetItem *item = new QTableWidgetItem(currentAttr);
            /* make the item not editable */
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());

            internalPropertyChange = true;
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, item);
            propertyToLine.insert(currentAttr, ui->tableWidget->rowCount()-1);

            QTableWidgetItem *itemValue = new QTableWidgetItem("");
            internalPropertyChange = true;
            ui->tableWidget->setItem( ui->tableWidget->rowCount()-1, 1,
                                      itemValue);

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

    QString key;
    foreach( key, propertyToValue.keys() )
    {
        if(key.startsWith(text))
        {
            QTableWidgetItem *item = new QTableWidgetItem(key);
            QTableWidgetItem *itemValue = new QTableWidgetItem(propertyToValue[key]);

            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            internalPropertyChange = true;
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, item);
            propertyToLine.insert(key, ui->tableWidget->rowCount()-1);
            internalPropertyChange = true;
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, itemValue);
        }
    }
}
