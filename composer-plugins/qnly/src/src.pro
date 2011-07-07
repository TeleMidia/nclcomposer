QT += \
    xml

CONFIG += \
    debug \
    plugin

TEMPLATE = \
    lib

VERSION = \
    0.1.0

TARGET = \
    Qnly

SOURCES += \
    qnlycomposerplugin.cpp \
    qnlycomposerpluginfactory.cpp \
    core/model/qnlydocument.cpp \
    core/model/qnlyregion.cpp \
    core/model/qnlyregionbase.cpp \
    ui/view/qnlyview.cpp \
    ui/view/qnlygraphicsregion.cpp \
    ui/view/qnlygraphicsregionbase.cpp \
    ui/view/qnlycanvas.cpp

HEADERS += \
    qnlycomposerplugin.h \
    qnlycomposerpluginfactory.h \
    core/qnly.h \
    core/model/qnlydocument.h \
    core/model/qnlyregion.h \
    core/model/qnlyregionbase.h \
    ui/view/qnlyview.h \
    ui/view/qnlygraphicsregion.h \
    ui/view/qnlygraphicsregionbase.h \
    ui/view/qnlycanvas.h

UI_DIR = \
    .ui

MOC_DIR = \
    .moc

OBJECTS_DIR = \
    .obj

macx {
    LIBS += \
        $$quote(-framework ComposerCore)

    LIBS += \
        $$quote(-L/Library/Application Support/Composer -lNCLLanguageProfile)

    INCLUDEPATH += \
        $$quote(/Library/Frameworks/ComposerCore.framework) \
        $$quote(/Library/Frameworks/ComposerCore.framework/core) \
        $$quote(/Library/Frameworks/ComposerCore.framework/core/extensions)

    target.path = \
        $$quote(/Library/Application Support/Composer)

    INSTALLS = target
}

else:unix {
    LIBS += \
        $$quote(-L/usr/local/lib/composer -lComposerCore)

    LIBS += \
        $$quote(-L/usr/local/lib/composer/extension -lNCLLanguageProfile)

    INCLUDEPATH += \
        $$quote(/usr/local/include/composer/) \
        $$quote(/usr/local/include/composer/core) \
        $$quote(/usr/local/include/composer/core/extensions)

    target.path = \
        $$quote(/usr/local/lib/composer/extension)

    INSTALLS = target
}

else:win32 {
    LIBS += \
        $$quote(-LC:/Composer -LC:/Composer/lib/composer -lComposerCore1)

    INCLUDEPATH += \
        $$quote(C:/Composer/include/composer) \
        $$quote(C:/Composer/include/composer/core) \
        $$quote(C:/Composer/include/composer/core/extensions)

    target.path = \
        $$quote(C:/composer/lib/composer)

    INSTALLS = target
}
