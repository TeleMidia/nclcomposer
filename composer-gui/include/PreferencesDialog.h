#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDir>
#include <QDebug>
#include <QPluginLoader>
#include <QDialog>
#include <QListWidgetItem>

#include <core/extensions/IPluginFactory.h>
using namespace composer::extension;

#include <core/modules/PluginControl.h>
using namespace composer::core;

namespace Ui {
    class PreferencesDialog;
}

namespace composer {
    namespace gui {

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

}} //end namespace

#endif // PREFERENCES_H
