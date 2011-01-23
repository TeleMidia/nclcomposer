#include "include/PreferencesDialog.h"
#include "ui_PreferencesWidget.h"
#include <QListWidgetItem>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    this->setModal(true);
    loadPreferencesPages();

    connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this, SLOT(changeActivePage()));

    currentItem = NULL;
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::loadPreferencesPages(){
    QList<IPluginFactory*> plugins = PluginControl::getInstance()->
                                                        getLoadedPlugins();
    QList<IPluginFactory*>::iterator it;

    for (it = plugins.begin(); it != plugins.end(); it++)
    {
        IPluginFactory *pF = *it;

        new QListWidgetItem(pF->getPluginIcon(),
                            pF->getPluginName(),
                            ui->listWidget, 0);
        QWidget *page = pF->getPreferencePageWidget();
        pages[pF->getPluginName()] = page;
        page->hide();
        ui->scrollAreaVerticalLayout->addWidget(page);
    }
}

void PreferencesDialog::changeActivePage(){
    if (currentItem != NULL)
            pages[currentItem->text()]->hide();

    currentItem = ui->listWidget->currentItem();
    pages[currentItem->text()]->show();
}
