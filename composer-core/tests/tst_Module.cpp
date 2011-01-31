#include "tst_Module.h"

void tst_Module::initTestCase()
{

    QString baseDir = "resources";
    profileDir  = baseDir+QDir::separator()+"profiles"+QDir::separator();
    pluginDir   = baseDir+QDir::separator()+"plugins"+QDir::separator();
    documentDir = baseDir+QDir::separator()+"documents"+QDir::separator();

    docControl = DocumentControl::getInstance();
    QVERIFY(docControl);
    lgControl = LanguageControl::getInstance();
    QVERIFY(lgControl);
    pgControl = PluginControl::getInstance();
    QVERIFY(pgControl);
}

void tst_Module::languageProfile()
{
    QList<ILanguageProfile*> list;
    /* Load profiles starts empty*/
    list = lgControl->getLoadedProfiles();
    QCOMPARE(list.isEmpty(),true);

    /* Load 1 one profile in the resource dir*/
    lgControl->loadProfiles(profileDir);
    list = lgControl->getLoadedProfiles();
    QCOMPARE(list.isEmpty(),false);
    QCOMPARE(list.size(),1);

    /* Try to load the same profile */
    QVERIFY(lgControl->loadProfile
            (profileDir+"libNCLLanguageProfile.dylib"));
    list = lgControl->getLoadedProfiles();
    QCOMPARE(list.size(),1);

    /* remove the loaded profile */
    QVERIFY(lgControl->removeProfile(NCL));
    list = lgControl->getLoadedProfiles();
    QCOMPARE(list.isEmpty(),true);

    /* reload the profile from fileName */
    QVERIFY(lgControl->loadProfile
            (profileDir+"libNCLLanguageProfile.dylib"));
    list = lgControl->getLoadedProfiles();
    QCOMPARE(list.size(),1);

    /* Try to load a non existing profile */
    QVERIFY(!lgControl->loadProfile("blah"));
    list = lgControl->getLoadedProfiles();
    QCOMPARE(list.size(),1);

}

void tst_Module::cleanupTestCase()
{
      DocumentControl::releaseInstance();
      LanguageControl::releaseInstance();
      PluginControl::releaseInstance();
}

QTEST_APPLESS_MAIN(tst_Module)
