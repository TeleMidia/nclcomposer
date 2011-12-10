#include "tst_ModulePlugin.h"

void tst_ModulePlugin::initTestCase()
{
#ifdef Q_WS_MAC
    pluginDir = "/Library/Application Support/Composer/";
#elif WIN32
    //TODO:
#else
    pluginDir = "/usr/local/lib/composer/extensions/";
#endif
    pgControl = PluginControl::getInstance();
}

void tst_ModulePlugin::cleanupTestCase()
{
    PluginControl::releaseInstance();
    pgControl = NULL;
}

void tst_ModulePlugin::loadPluginDir()
{
    QList<IPluginFactory*> list;

    pgControl->loadPlugins(pluginDir);
    list = pgControl->getLoadedPlugins();
    QCOMPARE(list.isEmpty(),false);

    /* Load the same plugin */
    int size = list.size();
    QVERIFY(pgControl->loadPlugin(pluginDir+"libdebug_console.so"));
    list = pgControl->getLoadedPlugins();
    QCOMPARE(list.size(),size);

    /* load a non existent plugin */
    QVERIFY(!pgControl->loadPlugin("blah"));
    list = pgControl->getLoadedPlugins();
    QCOMPARE(list.size(),size);
}

void tst_ModulePlugin::pluginName_data()
{
    QTest::addColumn<QString>("fileName");


#ifdef Q_WS_MAC
    QTest::newRow("TEXTUAL") << pluginDir + "libncl_textual_plugin.dylib";
    QTest::newRow("DEBUG") << pluginDir + "libdebug_console.dylib";
    QTest::newRow("OUTLINE") << pluginDir + "liboutline_view.dylib";
    QTest::newRow("PROPERTIES") << pluginDir + "libproperties_view.dylib";
    QTest::newRow("QNLY") << pluginDir + "libQnly.dylib";
    QTest::newRow("QNST") << pluginDir + "libQnst.dylib";
#elif WIN32
    QTest::newRow("NCL") << profileDir+QDir::separator()+
                            "NCL"+QDir::separator()+"NCLLanguageProfile.dll";
#else
    QTest::newRow("TEXTUAL") << pluginDir + "libncl_textual_plugin.so";
    QTest::newRow("DEBUG") << pluginDir + "libdebug_console.so";
    QTest::newRow("OUTLINE") << pluginDir + "liboutline_view.so";
    QTest::newRow("PROPERTIES") << pluginDir + "libproperties_view.so";
    QTest::newRow("QNLY") << pluginDir + "libQnly.so";
    QTest::newRow("QNST") << pluginDir + "libQnst.so";
#endif
}

void tst_ModulePlugin::pluginName()
{
    QFETCH(QString, fileName);

    IPluginFactory *factory = NULL;

    factory = pgControl->loadPlugin(fileName);
    QVERIFY(factory);

}

void tst_ModulePlugin::pluginLoadBenchmark_data()
{
    QTest::addColumn<QString>("fileName");

#ifdef Q_WS_MAC
    QTest::newRow("TEXTUAL") << pluginDir + "libncl_textual_plugin.dylib";
    QTest::newRow("DEBUG") << pluginDir + "libdebug_console.dylib";
    QTest::newRow("OUTLINE") << pluginDir + "liboutline_view.dylib";
    QTest::newRow("PROPERTIES") << pluginDir + "libproperties_view.dylib";
    QTest::newRow("QNLY") << pluginDir + "libQnly.dylib";
    QTest::newRow("QNST") << pluginDir + "libQnst.dylib";
#elif WIN32
    QTest::newRow("NCL") << profileDir+QDir::separator()+
                            "NCL"+QDir::separator()+"NCLLanguageProfile.dll";
#else
    QTest::newRow("TEXTUAL") << pluginDir + "libncl_textual_plugin.so";
    QTest::newRow("DEBUG") << pluginDir + "libdebug_console.so";
    QTest::newRow("OUTLINE") << pluginDir + "liboutline_view.so";
    QTest::newRow("PROPERTIES") << pluginDir + "libproperties_view.so";
    QTest::newRow("QNLY") << pluginDir + "libQnly.so";
    QTest::newRow("QNST") << pluginDir + "libQnst.so";
#endif
}

void tst_ModulePlugin::pluginLoadBenchmark()
{
    if (!isBenchmark)
        QSKIP("This test is not a benchmark test", SkipSingle);

    QFETCH(QString,fileName);

    QBENCHMARK
    {
        pgControl->loadPlugin(fileName);
    }
}
