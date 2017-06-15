/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

#include "PerspectiveManager.h"
#include "ui_PerspectiveManager.h"

#include <QAbstractItemModel>
#include <QMessageBox>

#include <util/ComposerSettings.h>
using namespace cpr::core;

CPR_GUI_BEGIN_NAMESPACE

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

  GlobalSettings settings;
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
  GlobalSettings settings;
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
  else
  {
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
  GlobalSettings settings;
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

CPR_GUI_END_NAMESPACE
