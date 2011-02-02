#include "tst_ModuleLanguage.h"


void tst_ModuleLanguage::initTestCase()
{
    QString baseDir = "resources";
    profileDir  = baseDir+QDir::separator()+"profiles"+QDir::separator();
    lgControl = LanguageControl::getInstance();
}

void tst_ModuleLanguage::cleanupTestCase()
{
    LanguageControl::releaseInstance();
    lgControl = NULL;
}

void tst_ModuleLanguage::languageLoadBenchmark_data()
{
    QTest::addColumn<QString>("fileName");

#ifdef Q_WS_MAC
    QTest::newRow("NCL Load") << profileDir+"libNCLLanguageProfile.dylib";
#endif
}

void tst_ModuleLanguage::languageLoadBenchmark()
{
    if (!isBenchmark)
        QSKIP("This test is not a benchmark test", SkipSingle);

    QFETCH(QString,fileName);

    QBENCHMARK
    {
        lgControl->loadProfile(fileName);
        lgControl->removeProfile(NCL);
    }
}

void tst_ModuleLanguage::languageProfile_data()
{
    QTest::addColumn<QString>("fileName");

#ifdef Q_WS_MAC
    QTest::newRow("NCL") << profileDir+"libNCLLanguageProfile.dylib";
#endif
}

void tst_ModuleLanguage::languageProfile()
{
    QFETCH(QString, fileName);

    QList<ILanguageProfile*> list;

    /* Try to load the same profile */
    QVERIFY(lgControl->loadProfile
            (fileName));
    list = lgControl->getLoadedProfiles();
    QCOMPARE(list.size(),1);

    /* remove the loaded profile */
    QVERIFY(lgControl->removeProfile(NCL));
    list = lgControl->getLoadedProfiles();
    QCOMPARE(list.isEmpty(),true);

    /* reload the profile from fileName */
    QVERIFY(lgControl->loadProfile
            (fileName));
    list = lgControl->getLoadedProfiles();
    QCOMPARE(list.size(),1);

    /* Try to load a non existing profile */
    QVERIFY(!lgControl->loadProfile("blah"));
    list = lgControl->getLoadedProfiles();
    QCOMPARE(list.size(),1);

    /* remove the loaded profile */
    QVERIFY(lgControl->removeProfile(NCL));
    list = lgControl->getLoadedProfiles();
    QCOMPARE(list.isEmpty(),true);
}

