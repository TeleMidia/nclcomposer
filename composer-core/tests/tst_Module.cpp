#include "tst_Module.h"

void tst_Module::initTestCase()
{
    docControl = DocumentControl::getInstance();
    QVERIFY(docControl);
    lgControl = LanguageControl::getInstance();
    QVERIFY(connect(lgControl,SIGNAL(notifyLoadedProfile(QString,QString)),
            SLOT(onLoadedProfile(QString,QString))));
    QVERIFY(lgControl);
    pgControl = PluginControl::getInstance();
    QVERIFY(pgControl);
}

void tst_Module::initCoreModules()
{
    lgControl->loadProfiles("../extensions/NCL");
}

void tst_Module::cleanupTestCase()
{
    DocumentControl::releaseInstance();
    LanguageControl::releaseInstance();
    PluginControl::releaseInstance();
}


void tst_Module::onLoadedProfile(QString profileName,QString fileName)
{
    QVERIFY(lgControl);
    QCOMPARE(profileName, QString("Nested Context Language Profile"));
}

QTEST_APPLESS_MAIN(tst_Module)
