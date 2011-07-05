#ifndef PLUGINDETAILSDIALOG_H
#define PLUGINDETAILSDIALOG_H

#include <QDialog>

#include <core/extensions/IPlugin.h>
using namespace composer::core;

#include "ui_PluginDetailsDialog.h"

namespace composer {
    namespace gui {

class PluginDetailsDialog : public QDialog
{
private:
    Ui::PluginDetailsDialog *ui;

public:
    PluginDetailsDialog(QWidget *parent = 0);
    virtual ~PluginDetailsDialog();

    void setCurrentPlugin(IPlugin *plugin);
};

} }
#endif // PLUGINDETAILSDIALOG_H
