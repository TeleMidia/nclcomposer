#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDir>
#include <QDebug>
#include <QPluginLoader>
#include <QDialog>
#include <QListWidgetItem>

#include "IPreferencesPage.h"

#define PLUGIN_DIR "/usr/local/lib/composer/plugins/preferences"

namespace Ui {
    class PreferencesWidget;
}

class PreferencesWidget : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesWidget(QWidget *parent = 0);
    ~PreferencesWidget();

private:
    Ui::PreferencesWidget *ui;
    QListWidgetItem *currentItem;

    bool loadPreferencesPages();
    QMap <QString, IPreferencesPage *> pages;

private slots:
    void changeActivePage();
};

#endif // PREFERENCES_H
