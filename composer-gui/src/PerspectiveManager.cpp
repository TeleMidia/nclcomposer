/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "PerspectiveManager.h"
#include "ui_PerspectiveManager.h"

#include <QSettings>
#include <QDebug>
#include <QAbstractItemModel>
#include <QMessageBox>

namespace composer {
    namespace gui {

PerspectiveManager::PerspectiveManager(QWidget *parent):
        QDialog(parent),
        ui(new Ui::PerpectiveManager())
{
    ui->setupUi(this);

    this->behavior = PERSPEC_SAVE; // default behavior

    connect (ui->deleteButton, SIGNAL(clicked()),
             this, SLOT(deleteSelectedPerspective()));

    connect ( ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
              this, SLOT(itemChanged(QTreeWidgetItem*, int)));
    QSettings settings("telemidia", "composer");
    defaultPerspective = settings.value("default_perspective").
                                    toString();
}

PerspectiveManager::~PerspectiveManager()
{
    delete ui;
}

void PerspectiveManager::setBehavior(PERSPEC_BEHAVIOR behavior)
{
    this->behavior = behavior;
}

void PerspectiveManager::updateContent()
{
    QSettings settings("telemidia", "composer");
    settings.beginGroup("pluginslayout");
    QStringList keys = settings.allKeys();
    settings.endGroup();

    ui->treeWidget->clear();

    int i;
    QTreeWidgetItem *item;

    internalChange = true;
    if(behavior == PERSPEC_SAVE) // If the widget is saving a new perspective
    {
        for(i = 0; i < keys.size(); i++)
        {
            item = new QTreeWidgetItem(ui->treeWidget);
            if(keys.at(i) == defaultPerspective)
                item->setCheckState(0, Qt::Checked);
            else
                item->setCheckState(0, Qt::Unchecked);

            item->setText(1, keys.at(i));
            item->setFlags(item->flags() | Qt::ItemIsEditable);
        }

        item = new QTreeWidgetItem(ui->treeWidget);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setCheckState(0, Qt::Unchecked);
        item->setText(1, "New perspective...");
        ui->treeWidget->setCurrentItem(item, 1,
                                       QItemSelectionModel::SelectCurrent);

        ui->treeWidget->setEditTriggers(QAbstractItemView::AnyKeyPressed |
                                         QAbstractItemView::DoubleClicked);

        this->setWindowTitle(tr("Saving a perspective"));
        ui->deleteButton->setVisible(false);
    }
    else // If it is loading an already saved perspective:
    {
        for(i = 0; i < keys.size(); i++)
        {
            item = new QTreeWidgetItem(ui->treeWidget);
            if(keys.at(i) == defaultPerspective)
                item->setCheckState(0, Qt::Checked);
            else
                item->setCheckState(0, Qt::Unchecked);

            item->setText(1, keys.at(i));
            item->setFlags(item->flags());
        }

        this->setWindowTitle(tr("Reloading a perspective"));
        ui->deleteButton->setVisible(true);
    }
    internalChange = false;
}

void PerspectiveManager::showEvent(QShowEvent *evt)
{
    QDialog::showEvent(evt);
    updateContent();
}

void PerspectiveManager::accept()
{
    QList<QTreeWidgetItem*> items = ui->treeWidget->selectedItems();
    if(items.size())
    {
        selectedName = items.at(0)->text(1);
        QDialog::accept();
    }
    else
        selectedName = "";
}

void PerspectiveManager::itemChanged(QTreeWidgetItem *item, int col)
{
    if(internalChange) return;

    internalChange = true;
    if(col == 0) //changed the default
    {
        for(int i = 0; i < ui->treeWidget->topLevelItemCount(); i++)
            ui->treeWidget->topLevelItem(i)->setCheckState(0, Qt::Unchecked);
        item->setCheckState(0, Qt::Checked);
        this->defaultPerspective = item->text(1);
    }
    else {
        if(item->checkState(0) == Qt::Checked)
            this->defaultPerspective = item->text(1);

        if(item->text(1) == "")
            item->setText(1, "empty");

    }
    internalChange = false;
}

QString PerspectiveManager::getSelectedName()
{
    return selectedName;
}

QString PerspectiveManager::getDefaultPerspective()
{
    return defaultPerspective;
}

void PerspectiveManager::deletePerspective(QString name)
{
    QSettings settings("telemidia", "composer");
    settings.beginGroup("pluginslayout");
    //TODO: When the name is empty ("") this remove all the perspectives.
    settings.remove(name);
    settings.endGroup();
}

void PerspectiveManager::deleteSelectedPerspective()
{
    QList<QTreeWidgetItem*> items = ui->treeWidget->selectedItems();
    selectedName = items.at(0)->text(1);

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Deleting perspective"),
                               tr("Are you sure you want to delete the.\n"
                                  "\"%1\" perspective ?").arg(selectedName),
                               QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        deletePerspective(selectedName);
        updateContent();
    }
    else if (ret == QMessageBox::Cancel)
        return;
}

} } //end namespace
