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

#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

#include <QMetaMethod>
#include <QMetaObject>

#include <QDialogButtonBox>
#include <QListWidgetItem>

CPR_GUI_BEGIN_NAMESPACE

PreferencesDialog::PreferencesDialog (QWidget *parent)
    : QDialog (parent), ui (new Ui::PreferencesDialog)
{
  ui->setupUi (this);
  this->setModal (true);

  loadPreferencesPages ();

  connect (ui->listWidget, SIGNAL (itemSelectionChanged ()), this,
           SLOT (changeActivePage ()));

  connect (ui->buttonBox_2, SIGNAL (clicked (QAbstractButton *)), this,
           SLOT (buttonClicked (QAbstractButton *)));

  connect (this, SIGNAL (accepted ()), this, SLOT (applyCurrentValues ()));

  currentItem = nullptr;
  currentPage = nullptr;
}

PreferencesDialog::~PreferencesDialog () { delete ui; }

void
PreferencesDialog::addPreferencePage (IPluginFactory *pF)
{
  if (pF->getPreferencePageWidget () == nullptr)
    return;

  addPreferencePage (pF->icon (), pF->metadata ().value ("name").toString (),
                     (IPreferencesPage *)pF->getPreferencePageWidget ());
}

void
PreferencesDialog::addPreferencePage (QIcon icon, QString name,
                                      IPreferencesPage *page)
{
  new QListWidgetItem (icon, name, ui->listWidget, 0);

  pages[name] = page;
  page->hide ();
  ui->scrollAreaVerticalLayout->addWidget (page);
}

void
PreferencesDialog::addPreferencePage (IPreferencesPage *page)
{
  new QListWidgetItem (page->getIcon (), page->getName (), ui->listWidget, 0);

  pages[page->getName ()] = page;
  page->hide ();
  ui->scrollAreaVerticalLayout->addWidget (page);
}

void
PreferencesDialog::loadPreferencesPages ()
{
  QList<IPluginFactory *> plugins
      = PluginControl::instance ()->getLoadedPlugins ();
  QList<IPluginFactory *>::iterator it;

  for (it = plugins.begin (); it != plugins.end (); it++)
  {
    IPluginFactory *pF = *it;
    if (pF->getPreferencePageWidget () == nullptr)
      continue;

    new QListWidgetItem (pF->icon (),
                         pF->metadata ().value ("name").toString (),
                         ui->listWidget, 0);

    IPreferencesPage *page
        = (IPreferencesPage *)pF->getPreferencePageWidget ();
    pages[pF->metadata ().value ("name").toString ()] = page;
    page->hide ();
    ui->scrollAreaVerticalLayout->addWidget (page);
  }
}

void
PreferencesDialog::changeActivePage ()
{
  if (currentItem != nullptr)
  {
    if (pages.contains (currentItem->text ()))
    {
      pages[currentItem->text ()]->hide ();
    }
  }

  currentItem = ui->listWidget->currentItem ();
  if (currentItem != nullptr)
  {
    if (pages.contains (currentItem->text ()))
    {
      pages[currentItem->text ()]->show ();
      currentPage = pages[currentItem->text ()];
      ui->pageTitle->setText (currentItem->text ());
    }
  }
}

void
PreferencesDialog::buttonClicked (QAbstractButton *button)
{
  QDialogButtonBox::StandardButton std
      = ui->buttonBox_2->standardButton (button);

  if (std == QDialogButtonBox::Apply)
    applyCurrentValues ();
}

void
PreferencesDialog::applyCurrentValues ()
{
  if (currentPage)
    currentPage->applyValues ();
}

/* void PreferencesDialog::show()
{
  QDialog::show();
  selectFirst();
}

void PreferencesDialog::selectFirst()
{
  // \todo And if we do not have any child?
  ui->listWidget->item(0)->setSelected(true);
} */

CPR_GUI_END_NAMESPACE
