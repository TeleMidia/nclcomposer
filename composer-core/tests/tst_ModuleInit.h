#ifndef TST_MODULEINIT_H
#define TST_MODULEINIT_H

#include <QObject>
#include <QTest>


#include <core/modules/DocumentControl.h>
#include <core/modules/LanguageControl.h>
#include <core/modules/PluginControl.h>
#include <core/modules/TransactionControl.h>
using namespace composer::core::module;

class tst_ModuleInit : public QObject
{
    Q_OBJECT
public:
    void setInterations(int _interartions) { interations = _interartions; }
    void setBenchmark(bool isBench) { isBenchmark = isBench; }
private:
    LanguageControl *lgControl;
    DocumentControl *docControl;
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
//    void languageProfile();
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
