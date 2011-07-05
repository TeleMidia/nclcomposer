#include "PluginDetailsDialog.h"
#include "ui_PluginDetailsDialog.h"
namespace composer {
    namespace gui {

PluginDetailsDialog::PluginDetailsDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::PluginDetailsDialog())
{
    ui->setupUi(this);
}

PluginDetailsDialog::~PluginDetailsDialog()
{
//    delete ui;
}

} } // end namespace
