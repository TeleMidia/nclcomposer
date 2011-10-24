CONFIG += qtestlib testcase
MOC_DIR = .mocs
OBJECTS_DIR = .objs

macx {
    LIBS += -L../core \
            -F../core \
            -framework \
            ComposerCore
    INCLUDEPATH += ../core/include
}
else:win32 {
    LIBS += -LC:\Composer\ \
        -lComposerCore1
    INCLUDEPATH += ../core/include
}
else {
    LIBS += -L../core \
        -lComposerCore
    INCLUDEPATH += ../core/include
}

SOURCES += \
    tst_ModuleLanguage.cpp \
    tst_Core.cpp \
    tst_ModuleInit.cpp

HEADERS += \
    tst_ModuleInit.h \
    tst_ModuleLanguage.h
