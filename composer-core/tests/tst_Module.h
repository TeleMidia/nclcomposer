#ifndef TST_MODULEALLOC_H
#define TST_MODULEALLOC_H

#include <QObject>
//#include <QtGui>
#include <QtTest/QtTest>


#include <core/modules/DocumentControl.h>
#include <core/modules/LanguageControl.h>
#include <core/modules/PluginControl.h>
#include <core/modules/TransactionControl.h>
using namespace composer::core::module;

class tst_Module : public QObject
{
    Q_OBJECT
private:
    LanguageControl *lgControl;
    DocumentControl *docControl;
    PluginControl *pgControl;
    QString profileDir;
    QString pluginDir;
    QString documentDir;

public slots:

private slots:
    void initTestCase();
    void init();
    void cleanup();
    void languageProfile();
    void pluginProfile();
    void launchDocument();
    void launchDocument_data();
    void cleanupTestCase();
};

#endif // TST_MODULEALLOC_H
