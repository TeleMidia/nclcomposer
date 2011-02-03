TEMPLATE = \
    lib

TARGET = \
    ncllayout-plugin

MOC_DIR = \
    .moc

UI_DIR = \
    .ui

OBJECTS_DIR = \
    .obj

CONFIG += \
    plugin \
    debug

INSTALLS += \
    target

HEADERS += \
    ui/mainwindow.h \
    ui/view/abstractview.h \
    ui/view/abstractscene.h \
    ui/view/abstractitem.h \
    ui/view/abstracteditor.h \
    ui/view/layout/layoutview.h \
    ui/view/layout/layoutscene.h \
    ui/view/layout/layoutitem.h \
    ui/view/layout/layouteditor.h \
    core/layoutplugin.h \
    core/layoutpluginfactory.h \
    ui/view/abstractcontainer.h \
    ui/view/layout/layoutcontainer.h

SOURCES += \
    ui/mainwindow.cpp \
    ui/view/abstractview.cpp \
    ui/view/abstractscene.cpp \
    ui/view/abstractitem.cpp \
    ui/view/abstracteditor.cpp \
    ui/view/layout/layoutview.cpp \
    ui/view/layout/layoutscene.cpp \
    ui/view/layout/layoutitem.cpp \
    ui/view/layout/layouteditor.cpp \
    core/layoutplugin.cpp \
    core/layoutpluginfactory.cpp \
    ui/view/abstractcontainer.cpp \
    ui/view/layout/layoutcontainer.cpp

macx {
    LIBS += \
        -framework ComposerCore

    INCLUDEPATH += \
        /Library/Frameworks/ComposerCore.framework/

    target.path = \
        $$quote(/Library/Application Support/Composer)
}

else:unix {
    LIBS += \
        -L/usr/local/lib/composer -lComposerCore

    INCLUDEPATH += \
        /usr/local/include/composer

    target.path = \
        $$quote(/usr/local/lib/composer/extension)
}

else:win32 {
    LIBS += \
        -LC:/composer/lib/composer -lComposerCore1

    INCLUDEPATH += \
        C:/composer/include/composer

    target.path = \
        $$quote(C:/composer/lib/composer/)
}
