/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDir>
#include <QDebug>
#include <QPluginLoader>
#include <QDialog>
#include <QAbstractButton>
#include <QListWidgetItem>

#include <core/extensions/IPluginFactory.h>
using namespace composer::extension;

#include <core/modules/PluginControl.h>
using namespace composer::core;

#include "IPreferencesPage.h"

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

    void addPreferencePage(IPluginFactory *);
    void addPreferencePage(IPreferencesPage *page);
    /*!
     * \deprecated
     */
    void addPreferencePage(QIcon icon, QString name, QWidget *widget);

private:
    Ui::PreferencesDialog *ui;
    QListWidgetItem *currentItem;
    QWidget *currentPage;

    void loadPreferencesPages();
    QMap <QString, QWidget *> pages;

private slots:
    void changeActivePage();
    void applyCurrentValues();
    void buttonClicked(QAbstractButton*);

/*public Q_SLOTS:
    void show();
    void selectFirst(); */
};

}} //end namespace

#endif // PREFERENCES_H
