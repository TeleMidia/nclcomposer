/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

#include <QMetaObject>
#include <QMetaMethod>

#include <QDialogButtonBox>
#include <QListWidgetItem>

namespace composer {
namespace gui {

PreferencesDialog::PreferencesDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PreferencesDialog)
{
  ui->setupUi(this);
  this->setModal(true);

  loadPreferencesPages();

  connect( ui->listWidget,
          SIGNAL(itemSelectionChanged()),
          this,
          SLOT(changeActivePage()));

  connect(ui->buttonBox_2,
          SIGNAL(clicked(QAbstractButton*)),
          this,
          SLOT(buttonClicked(QAbstractButton*)));

  currentItem = NULL;
}

PreferencesDialog::~PreferencesDialog()
{
  delete ui;
}

void PreferencesDialog::addPreferencePage(IPluginFactory *pF)
{
  if (pF->getPreferencePageWidget() == NULL) return;


  addPreferencePage(pF->icon(), pF->name(), pF->getPreferencePageWidget());
}

void PreferencesDialog::addPreferencePage(QIcon icon, QString name,
                                          QWidget *page)
{
  new QListWidgetItem(icon, name, ui->listWidget, 0);

  pages[name] = page;
  page->hide();
  ui->scrollAreaVerticalLayout->addWidget(page);
}

void PreferencesDialog::loadPreferencesPages()
{
  QList<IPluginFactory*> plugins = PluginControl::getInstance()->
      getLoadedPlugins();
  QList<IPluginFactory*>::iterator it;

  for (it = plugins.begin(); it != plugins.end(); it++)
  {
    IPluginFactory *pF = *it;
    if (pF->getPreferencePageWidget() == NULL) continue;

    new QListWidgetItem(pF->icon(),
                        pF->name(),
                        ui->listWidget, 0);

    QWidget *page = pF->getPreferencePageWidget();
    pages[pF->name()] = page;
    page->hide();
    ui->scrollAreaVerticalLayout->addWidget(page);
  }
}

void PreferencesDialog::changeActivePage()
{
  if (currentItem != NULL)
  {
    if(pages.contains(currentItem->text())){
      pages[currentItem->text()]->hide();
    }
  }

  currentItem = ui->listWidget->currentItem();
  if(currentItem != NULL)
  {
    if(pages.contains(currentItem->text()))
    {
      pages[currentItem->text()]->show();
      currentPage = pages[currentItem->text()];
    }
  }
}

void PreferencesDialog::buttonClicked(QAbstractButton* button)
{
  QDialogButtonBox::StandardButton std = ui->buttonBox_2->standardButton(button);

  if(std == QDialogButtonBox::Apply)
  {
    QString slotName("applyValues()");

    QWidget *inst = currentPage;
    int idxSlot = inst->metaObject()
        ->indexOfSlot( slotName.toStdString().c_str() );
    if(idxSlot != -1)
    {
      QMetaMethod method = inst->metaObject()->method(idxSlot);
      method.invoke(inst, Qt::DirectConnection);
    }
  }
}

}} //end namespace
