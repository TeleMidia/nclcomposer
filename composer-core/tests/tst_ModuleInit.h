/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef TST_MODULEINIT_H
#define TST_MODULEINIT_H

#include <QObject>
#include <QTest>


// #include <modules/DocumentControl.h>
#include <modules/LanguageControl.h>
#include <modules/PluginControl.h>
// #include <modules/TransactionControl.h>
using namespace composer::core;

class tst_ModuleInit : public QObject
{
    Q_OBJECT
public:
    void setInterations(int _interartions) { interations = _interartions; }
    void setBenchmark(bool isBench) { isBenchmark = isBench; }
private:
    LanguageControl *lgControl;
    // DocumentControl *docControl;
    PluginControl *pgControl;
    int interations;
    bool isBenchmark;
    QString profileDir;
    QString pluginDir;
    QString documentDir;

private slots:
    void initTestCase();
    void initBenchmark_data();
    void initTorture();
    void initBenchmark();
    void languageProfile();
//    void pluginProfile();
//    void launchDocument();
//    void launchDocument_data();
//    void closeDocument();
//    void closeDocument_data();
//    void launchAndCloseDocument();
//    void launchAndCloseDocument_data();
    void cleanupTestCase();
};

#endif // TST_MODULEINIT_H
