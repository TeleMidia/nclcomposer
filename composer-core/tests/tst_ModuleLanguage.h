#ifndef TST_MODULELANGUAGE_H
#define TST_MODULELANGUAGE_H

#include <QObject>
#include <QTest>

#include <core/modules/LanguageControl.h>
using namespace composer::core::module;

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
