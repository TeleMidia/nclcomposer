#ifndef TST_MODULEALLOC_H
#define TST_MODULEALLOC_H

#include <QObject>
#include <QTest>

#include <modules/DocumentControl.h>
#include <modules/LanguageControl.h>
#include <modules/PluginControl.h>
#include <modules/TransactionControl.h>
using namespace composer::core::module;

class tst_Module : public QObject
{
    Q_OBJECT
private:
    LanguageControl *lgControl;
    DocumentControl *docControl;
    PluginControl *pgControl;

public slots:
    void onLoadedProfile(QString,QString);

private slots:
    void initTestCase();
    void initCoreModules();
    void cleanupTestCase();
};

#endif // TST_MODULEALLOC_H
