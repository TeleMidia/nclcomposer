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

void PluginDetailsDialog::setCurrentPlugin(IPluginFactory *currentPluginFactory)
{
    this->currentPluginFactory = currentPluginFactory;

    ui->label_NameValue->setText(currentPluginFactory->name());
    ui->label_VendorValue->setText(currentPluginFactory->version());
    ui->label_CompatibilityVersionValue
                ->setText(currentPluginFactory->compatVersion());

    ui->label_VersionValue->setText(currentPluginFactory->version());
    ui->label_CategoryValue->setText(currentPluginFactory->category());

    ui->label_URLValue->setText(currentPluginFactory->url());

    ui->textBrowser_Description->setText(currentPluginFactory->description());

    ui->label_CopyrightValue->setText(currentPluginFactory->copyright());

    ui->textBrowser_License->setText(currentPluginFactory->license());
}

} } // end namespace
