/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
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

    ui->label_VersionValue->setText(currentPluginFactory->metadata().value("version").toString());
    ui->label_CategoryValue->setText(currentPluginFactory->metadata().value("category").toString());

    ui->label_URLValue->setText(currentPluginFactory->metadata().value("url").toString());

    ui->textBrowser_Description->setText(currentPluginFactory->metadata().value("description").toString());

    ui->label_CopyrightValue->setText(currentPluginFactory->metadata().value("copyright").toString());

    ui->textBrowser_License->setText(currentPluginFactory->metadata().value("license").toString());
}

} } // end namespace
