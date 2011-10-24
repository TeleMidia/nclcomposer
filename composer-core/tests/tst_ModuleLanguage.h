/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef TST_MODULELANGUAGE_H
#define TST_MODULELANGUAGE_H

#include <QObject>
#include <QTest>

#include <modules/LanguageControl.h>
using namespace composer::core;

class tst_ModuleLanguage : public QObject
{
    Q_OBJECT
public:
    void setBenchmark(bool isBench) { isBenchmark = isBench; }
private:
    LanguageControl *lgControl;
    QString profileDir;
    bool isBenchmark;

public slots:

private slots:
    void initTestCase();
    void languageProfile_data();
    void languageProfile();
    void languageLoadBenchmark_data();
    void languageLoadBenchmark();
    void cleanupTestCase();
};

#endif // TST_MODULELANGUAGE_H
