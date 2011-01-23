#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDir>
#include <QDebug>
#include <QPluginLoader>
#include <QDialog>
#include <QListWidgetItem>

#include <core/extensions/IPluginFactory.h>
using namespace composer::core::extension::plugin;

#include <core/modules/PluginControl.h>
using namespace composer::core::module;

namespace Ui {
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

private:
    Ui::PreferencesDialog *ui;
    QListWidgetItem *currentItem;

    void loadPreferencesPages();
    QMap <QString, QWidget *> pages;

private slots:
    void changeActivePage();
};

#endif // PREFERENCES_H
