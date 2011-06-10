#include "PerspectiveManager.h"
#include "ui_PerspectiveManager.h"

#include <QSettings>
#include <QDebug>
#include <QAbstractItemModel>

namespace composer {
    namespace gui {

PerspectiveManager::PerspectiveManager(QWidget *parent):
        QDialog(parent),
        ui(new Ui::PerpectiveManager())
{
    ui->setupUi(this);

    this->behavior = PERSPEC_SAVE; // default behavior
}

PerspectiveManager::~PerspectiveManager()
{
    delete ui;
}

void PerspectiveManager::setBehavior(PERSPEC_BEHAVIOR behavior)
{
    this->behavior = behavior;
}

void PerspectiveManager::showEvent(QShowEvent *evt)
{
    QDialog::showEvent(evt);

    QSettings settings("telemidia", "composer");
    settings.beginGroup("pluginslayout");
    QStringList keys = settings.allKeys();
    settings.endGroup();

    while(ui->tableWidget->rowCount())
        ui->tableWidget->removeRow(0);

    int i;
    for(i = 0; i < keys.size(); i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(keys.at(i));
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(0, i, item);
    }

    if(behavior == PERSPEC_SAVE) // If the widget is saving a new perspective
    {
        QTableWidgetItem *item = new QTableWidgetItem("New perspective...");
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, item);
        ui->tableWidget->setCurrentCell(i, 0,
                                        QItemSelectionModel::ToggleCurrent);

        ui->tableWidget->setEditTriggers(QAbstractItemView::AnyKeyPressed |
                                         QAbstractItemView::DoubleClicked);

        this->setWindowTitle(tr("Saving a perspective"));
    }
    else // If it is loading a already saved perspective:
    {
        ui->tableWidget->setCurrentCell(0, 0,
                                        QItemSelectionModel::ToggleCurrent);

        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        this->setWindowTitle(tr("Reloading a perspective"));
    }
}

void PerspectiveManager::accept()
{
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    selectedName = items.at(0)->text();
    QDialog::accept();
}

QString PerspectiveManager::getSelectedName()
{
    return selectedName;
}

} } //end namespace
