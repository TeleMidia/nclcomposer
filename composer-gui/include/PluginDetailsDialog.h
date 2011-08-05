/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef PLUGINDETAILSDIALOG_H
#define PLUGINDETAILSDIALOG_H

#include <QDialog>

#include <core/extensions/IPluginFactory.h>
using namespace composer::core;

#include "ui_PluginDetailsDialog.h"

namespace composer {
    namespace gui {

class PluginDetailsDialog : public QDialog
{
private:
    Ui::PluginDetailsDialog *ui;
    IPluginFactory *currentPluginFactory;

public:
    PluginDetailsDialog(QWidget *parent = 0);
    virtual ~PluginDetailsDialog();

    void setCurrentPlugin(IPluginFactory *plugin);
};

} }
#endif // PLUGINDETAILSDIALOG_H
