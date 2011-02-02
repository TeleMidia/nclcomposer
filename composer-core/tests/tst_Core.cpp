#include "tst_ModuleInit.h"
#include "tst_ModuleLanguage.h"

int run_ModuleInit()
{
    tst_ModuleInit testModuleInit;
    testModuleInit.setInterations(10240);
    testModuleInit.setBenchmark(true);
    QStringList args;
    args.append("-v2");
    return QTest::qExec(&testModuleInit,args);
}

int run_ModuleLanguage()
{
    tst_ModuleLanguage testModuleLanguage;
    testModuleLanguage.setBenchmark(true);
    QStringList args;
    args.append("-v2");
    return QTest::qExec(&testModuleLanguage, args);
}

int main(int argc, char** argv)
{
    //run_ModuleInit();
    run_ModuleLanguage();
}
