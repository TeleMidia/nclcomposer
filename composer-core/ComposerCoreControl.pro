# -------------------------------------------------
# Project created by QtCreator 2010-03-12T13:06:44
# -------------------------------------------------
QT -= gui
QT += xml
TARGET = ComposerCoreControl
CONFIG += release
macx::MODEL_LOCATION = /Library/Frameworks
else:unix:MODEL_LOCATION = /usr/local
macx:CONFIG += lib_bundle \
    x86
debug:INSTALLBASE = ./debug
release {
    macx:INSTALLBASE = /Library/Frameworks
    else:unix:INSTALLBASE = /usr/local
    win32:INSTALLBASE = C:\Composer
}
macx { 
    LIBS += -framework \
        ComposerCoreModel
    INCLUDEPATH += /Library/Frameworks/ComposerCoreModel.framework/
}
else:unix { 
    LIBS += -lComposerCoreModel
    INCLUDEPATH += $$MODEL_LOCATION/include/composer
}

DESTDIR = $$INSTALLBASE/lib/composer/control

TEMPLATE = lib
MOC_DIR = mocs
OBJECTS_DIR = objs
DEFINES += COMPOSERCORECONTROL_LIBRARY

SOURCES += src/util/DocumentParser.cpp \
    src/modules/MessageControl.cpp \
    src/modules/ProjectControl.cpp \
    src/modules/PluginControl.cpp \
    src/modules/TransactionControl.cpp \
    src/util/Project.cpp \
    src/util/EntityUtil.cpp \
    src/util/CoreManager.cpp \
    src/modules/ModuleControl.cpp

HEADERS_MAN += include/modules/MessageControl.h \
    include/modules/ProjectControl.h \
    include/modules/PluginControl.h \
    include/modules/TransactionControl.h \
    include/modules/IModule.h \
    include/modules/ModuleControl.h

HEADERS_UTIL +=include/util/ComposerCoreControl_global.h \
    include/util/Project.h \
    include/util/DocumentParser.h \
    include/util/EntityUtil.h \
    include/util/CoreManager.h

HEADERS_PLUGIN += include/plugin/IPluginMessage.h \
    include/plugin/IPluginFactory.h

HEADERS = $$HEADERS_UTIL $$HEADERS_PLUGIN $$HEADERS_MAN




headers_man.files = $$HEADERS_MAN
headers_plugin.files = $$HEADERS_PLUGIN
headers_util.files = $$HEADERS_UTIL

macx { 
    DESTDIR = $$INSTALLBASE/
    QMAKE_FRAMEWORK_BUNDLE_NAME = ComposerCoreControl
    headers_man.path = control/modules
    headers_plugin.path = control/plugin
    headers_util.path = control/util
    QMAKE_BUNDLE_DATA += headers_man headers_plugin headers_util
}
else:unix { 
    HEADERS_PATH_UNIX = $$INSTALLBASE/include/composer
    headers_man.path = $$HEADERS_PATH_UNIX/control/modules
    headers_plugin.path = $$HEADERS_PATH_UNIX/control/plugin
    headers_util.path = $$HEADERS_PATH_UNIX/control/util
    #headers.CONFIG += no_all
    INSTALLS += headers_man headers_plugin headers_util
    PRE_TARGETDEPS += install_headers_man install_headers_plugin install_headers_util
}else:win32:
