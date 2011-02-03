TARGET = ncllayout-plugin

include (../plugins-common.pri)

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

INSTALLS += target
