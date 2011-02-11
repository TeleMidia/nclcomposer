TARGET = ncllayout-plugin

include (../plugins-common.pri)

HEADERS += \
    ui/mainwindow.h \
    ui/view/layoutview.h \
    ui/view/layoutscene.h \
    ui/view/layoutitem.h \
    ui/view/layouteditor.h \
    core/layoutplugin.h \
    core/layoutpluginfactory.h \
    ui/view/layoutcontainer.h \
    ui/view/layoutoutline.h

SOURCES += \
    ui/mainwindow.cpp \
    ui/view/layoutview.cpp \
    ui/view/layoutscene.cpp \
    ui/view/layoutitem.cpp \
    ui/view/layouteditor.cpp \
    core/layoutplugin.cpp \
    core/layoutpluginfactory.cpp \
    ui/view/layoutcontainer.cpp \
    ui/view/layoutoutline.cpp

INSTALLS += target

RESOURCES += \
    resources.qrc
