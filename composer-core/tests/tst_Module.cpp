#include "tst_Module.h"

void tst_Module::initTestCase()
{

    QString baseDir = "resources";
    profileDir  = baseDir+QDir::separator()+"profiles"+QDir::separator();
    pluginDir   = baseDir+QDir::separator()+"plugins"+QDir::separator();
    documentDir = baseDir+QDir::separator()+"documents"+QDir::separator();

}

void tst_Module::init()
{
    docControl = DocumentControl::getInstance();
    QVERIFY(docControl);
    lgControl = LanguageControl::getInstance();
    QVERIFY(lgControl);
    pgControl = PluginControl::getInstance();
    QVERIFY(pgControl);

    QList<ILanguageProfile*> lgList;
    QList<IPluginFactory*> pgList;

    /* Load 1 one profile in the resource dir*/
    lgControl->loadProfiles(profileDir);
    lgList = lgControl->getLoadedProfiles();
    QCOMPARE(lgList.isEmpty(),false);
    QCOMPARE(lgList.size(),1);

    pgControl->loadPlugins(pluginDir);
    pgList = pgControl->getLoadedPlugins();
    QCOMPARE(pgList.isEmpty(),false);

}

void tst_Module::cleanup()
{
    DocumentControl::releaseInstance();
    LanguageControl::releaseInstance();
    PluginControl::releaseInstance();
}

void tst_Module::languageProfile()
{
    QList<ILanguageProfile*> list;

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

void tst_Module::pluginProfile()
{
    QList<IPluginFactory*> list;

    pgControl->loadPlugins(pluginDir);
    list = pgControl->getLoadedPlugins();
    QCOMPARE(list.isEmpty(),false);

    /* Load the same plugin */
    QVERIFY(pgControl->loadPlugin(pluginDir+"libdebug_console.dylib"));
    list = pgControl->getLoadedPlugins();
    QCOMPARE(list.size(),1);

    /* load a non existent plugin */
    QVERIFY(!pgControl->loadPlugin("blah"));
    list = pgControl->getLoadedPlugins();
    QCOMPARE(list.size(),1);

}

void tst_Module::launchDocument_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<bool>("openDocument");

    QDir docDir(documentDir);

    foreach (QString fileName, docDir.entryList(QDir::Files | QDir::NoSymLinks))
    {
        QString absFile = docDir.absoluteFilePath(fileName);
        QTest::newRow("openDoc") << absFile << true;
        QTest::newRow("closeDoc") << absFile << false;
    }
}

void tst_Module::launchDocument()
{

    QFETCH(QString, fileName);
    QFETCH(bool, openDocument);

    if (openDocument)
    {
        QBENCHMARK
        {
            docControl->launchDocument("testing",fileName);
        }
    } else {
        QBENCHMARK
        {
            docControl->closeDocument(fileName);
        }
    }
}

void tst_Module::cleanupTestCase()
{
      DocumentControl::releaseInstance();
      LanguageControl::releaseInstance();
      PluginControl::releaseInstance();
}

QTEST_MAIN(tst_Module)
