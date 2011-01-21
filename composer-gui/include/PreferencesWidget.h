#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDir>
#include <QDebug>
#include <QPluginLoader>
#include <QDialog>

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

    bool loadPreferencesPages();
    QMap <QString, IPreferencesPage *> pages;
};

#endif // PREFERENCES_H
