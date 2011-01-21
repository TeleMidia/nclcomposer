# -------------------------------------------------
# Project created by QtCreator 2010-03-12T13:06:44
# -------------------------------------------------
#QT -= gui
QT += xml
TEMPLATE = lib
TARGET = ComposerCoreControl
VERSION = 1.0
CONFIG += qt release
MOC_DIR = mocs
OBJECTS_DIR = objs

macx:CONFIG += lib_bundle x86
debug:INSTALLBASE = ./debug

release {
    macx:INSTALLBASE = /Library/Frameworks
    else:unix:INSTALLBASE = /usr/local
    win32:INSTALLBASE = C:\Composer
}

#macx::MODEL_LOCATION = /Library/Frameworks
#else:unix:MODEL_LOCATION = /usr/local

#macx {
#    LIBS += -framework \
#        ComposerCoreModel
#    INCLUDEPATH += /Library/Frameworks/ComposerCoreModel.framework/
#}
#else:unix {
 #   LIBS += -L/usr/local/lib/composer -lComposerCoreModel
 #  INCLUDEPATH += $$MODEL_LOCATION/include/composer
#}

DEFINES += COMPOSERCORECONTROL_LIBRARY

SOURCES += \
    src/modules/ProjectControl.cpp \
    src/modules/PluginControl.cpp \
    src/modules/TransactionControl.cpp \
    src/modules/LanguageControl.cpp \
    src/util/Project.cpp \
    src/util/DocumentParserFactory.cpp \
    src/util/DocumentParser.cpp \
    src/model/Entity.cpp \
    src/model/Document.cpp \
    src/util/Utilities.cpp


HEADERS_MAN += include/modules/ProjectControl.h \
    include/modules/PluginControl.h \
    include/modules/TransactionControl.h \
    include/modules/LanguageControl.h

HEADERS_PLUGIN += include/extension/plugin/IPlugin.h \
    include/extension/plugin/IPluginFactory.h

HEADERS_EXTENSION += include/extension/core/ILanguageProfile.h \
    include/extension/core/IDocumentParser.h

HEADERS_UTIL +=include/util/ComposerCoreControl_global.h \
    include/util/Project.h \
    include/util/DocumentParserFactory.h \
    include/util/DocumentParser.h \
    include/util/Singleton.h \
    include/util/Utilities.h

HEADERS_EXCEPTION +=  include/util/exception/EntityNotFound.h \
    include/util/exception/ParentNotFound.h \
    include/util/exception/CreationFailed.h

HEADERS_MODEL += include/model/Entity.h \
    include/model/Document.h

HEADERS = $$HEADERS_UTIL $$HEADERS_PLUGIN $$HEADERS_MAN $$HEADERS_MODEL \
          $$HEADERS_EXTENSION

headers_man.files = $$HEADERS_MAN
headers_plugin.files = $$HEADERS_PLUGIN
headers_extension.files = $$HEADERS_EXTENSION
headers_util.files = $$HEADERS_UTIL
headers_exception.files = $$HEADERS_EXCEPTION
headers_model.files = $$HEADERS_MODEL


macx { 
    DESTDIR = $$INSTALLBASE/
    QMAKE_FRAMEWORK_BUNDLE_NAME = $$TARGET
    headers_man.path = control/modules
    headers_plugin.path = control/extension/plugin
    headers_extension.path =control/extension/core
    headers_util.path = control/util
    headers_exception.path=control/util/exception
    headers_model.path = control/model
    QMAKE_BUNDLE_DATA += headers_man headers_plugin headers_extension \
                         headers_util headers_exception headers_model

}
else:unix { 
    target.path = $$INSTALLBASE/lib/composer
    HEADERS_PATH_UNIX = $$INSTALLBASE/include/composer
    headers_man.path = $$HEADERS_PATH_UNIX/control/modules
    headers_plugin.path = $$HEADERS_PATH_UNIX/control/plugin
    headers_util.path = $$HEADERS_PATH_UNIX/control/util
    headers_model.path = $$HEADERS_PATH_UNIX/control/model
    #headers.CONFIG += no_all
    INSTALLS += headers_man headers_plugin headers_util headers_model target
    PRE_TARGETDEPS += install_headers_man install_headers_plugin \
                      install_headers_util install_headers_model
}else:win32:
