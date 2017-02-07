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

#include "PluginDetailsDialog.h"

namespace composer {
    namespace gui {

PluginDetailsDialog::PluginDetailsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::PluginDetailsDialog())
{
    ui->setupUi(this);
}

PluginDetailsDialog::~PluginDetailsDialog()
{
    delete ui;
}

void PluginDetailsDialog::setCurrentPlugin(IPluginFactory *currentPluginFactory)
{
    this->currentPluginFactory = currentPluginFactory;

    ui->label_NameValue->setText(currentPluginFactory->metadata().value("name").toString());
    ui->label_VendorValue->setText(currentPluginFactory->metadata().value("vendor").toString());
    ui->label_CompatibilityVersionValue
                ->setText(currentPluginFactory->metadata().value("compatVersion").toString());

    ui->label_PluginPathValue->setText(currentPluginFactory->metadata().value("path").toString());

    ui->label_VersionValue->setText(currentPluginFactory->metadata().value("version").toString());
    ui->label_CategoryValue->setText(currentPluginFactory->metadata().value("category").toString());

    ui->label_URLValue->setText(currentPluginFactory->metadata().value("url").toString());

    ui->textBrowser_Description->setText(currentPluginFactory->metadata().value("description").toString());

    ui->label_CopyrightValue->setText(currentPluginFactory->metadata().value("copyright").toString());

    ui->textBrowser_License->setText(currentPluginFactory->metadata().value("license").toString());
}

} } // end namespace
