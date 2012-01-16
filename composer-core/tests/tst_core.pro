CONFIG += qtestlib testcase
MOC_DIR = .mocs
OBJECTS_DIR = .objs
QT += gui

DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG_WARNING

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
    tst_ModulePlugin.cpp \
    tst_ModuleProject.cpp \
    tst_ModuleInit.cpp \
    tst_InsertNode.cpp

HEADERS += \
    tst_ModuleLanguage.h \
    tst_ModulePlugin.h \
    tst_ModuleProject.h \
    tst_ModuleInit.h \
    tst_InsertNode.h \
    tst_Utils.h
