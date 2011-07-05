include(../configure.pri)

QT += \
    xml

SOURCES += \
    ui/view/qnlyview.cpp \
    ui/view/qnlygraphicsitem.cpp \
    ui/view/qnlygraphicsview.cpp \
    ui/view/qnlygraphicsscene.cpp \
    core/model/qnlydocument.cpp \
    core/model/qnlyregion.cpp \
    core/model/qnlyregionbase.cpp


HEADERS += \
    core/qnly.h \
    ui/view/qnlyview.h \
    ui/view/qnlygraphicsitem.h \
    ui/view/qnlygraphicsview.h \
    ui/view/qnlygraphicsscene.h \
    core/model/qnlydocument.h \
    core/model/qnlyregion.h \
    core/model/qnlyregionbase.h


contains(QNLYCONFIG, standalone) {

    message("[QNLY] Configuring for 'standalone' build...")

    TARGET = \
        Qnly

    ICON = \
        resources/qncllayout.icns

    SOURCES += \
        main.cpp \
        ui/qnlymainwindow.cpp

    HEADERS += \
        ui/qnlymainwindow.h

# //TODO: macosx, unix and win32 settings.

}else:contains(QNLYCONFIG, plugin) {

    message("[QNLY] Configuring for 'plugin' build...")

    DEFINES += \
        QNLYCOMPOSER

    TEMPLATE = \
        lib

    CONFIG += \
        plugin

    TARGET = \
        Qnly

    SOURCES += \
        extra/qnlycomposerplugin.cpp \
        extra/qnlycomposerpluginfactory.cpp

    HEADERS += \
        extra/qnlycomposerplugin.h \
        extra/qnlycomposerpluginfactory.h

    UI_DIR = \
        .ui

    MOC_DIR = \
        .moc

    OBJECTS_DIR = \
        .objects

    macx {
        LIBS += \
            "-framework ComposerCore"

        LIBS += \
            "-L/Library/Application Support/Composer" -lNCLLanguageProfile

        INCLUDEPATH += \
            "/Library/Frameworks/ComposerCore.framework" \
            "/Library/Frameworks/ComposerCore.framework/core" \
            "/Library/Frameworks/ComposerCore.framework/core/extensions"

        target.path = \
            "/Library/Application Support/Composer"

        INSTALLS = target
    }

    else:unix {
        LIBS += \
            "-L/usr/local/lib/composer -lComposerCore"

        LIBS += \
            "-L/usr/local/lib/composer/extension -lNCLLanguageProfile"

        INCLUDEPATH += \
            "/usr/local/include/composer/" \
            "/usr/local/include/composer/core" \
            "/usr/local/include/composer/core/extensions"

        target.path = \
            "/usr/local/lib/composer/extension"

        INSTALLS = target
    }

    else:win32 {
        LIBS += \
            "-LC:/Composer -LC:/Composer/lib/composer -lComposerCore1"

        INCLUDEPATH += \
            "C:/Composer/include/composer" \
            "C:/Composer/include/composer/core" \
            "C:/Composer/include/composer/core/extensions"

        target.path = \
            "C:/composer/lib/composer"

        INSTALLS = target
    }
}
