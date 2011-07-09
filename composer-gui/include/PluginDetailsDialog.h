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
