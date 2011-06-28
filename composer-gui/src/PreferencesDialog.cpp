#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"
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

    currentItem = NULL;
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::addPreferencesPage(IPluginFactory *pF)
{
    if (pF->getPreferencePageWidget() == NULL) return;

    new QListWidgetItem(pF->icon(),
                        pF->name(),
                        ui->listWidget, 0);

    QWidget *page = pF->getPreferencePageWidget();
    pages[pF->name()] = page;
    page->hide();
    ui->scrollAreaVerticalLayout->addWidget(page);
}

void PreferencesDialog::loadPreferencesPages(){
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

void PreferencesDialog::changeActivePage(){
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
        }
    }
}

}} //end namespace
