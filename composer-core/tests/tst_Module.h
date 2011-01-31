#ifndef TST_MODULEALLOC_H
#define TST_MODULEALLOC_H

#include <QObject>
#include <QTest>

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
    void languageProfile();
    void cleanupTestCase();
};

#endif // TST_MODULEALLOC_H
