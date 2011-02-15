# -------------------------------------------------
# Project created by QtCreator 2010-03-12T13:06:44
# -------------------------------------------------
QT -= gui
TEMPLATE = lib
TARGET = ComposerCore
VERSION = 1.0
CONFIG += debug
MOC_DIR = .mocs
OBJECTS_DIR = .objs

macx:CONFIG += lib_bundle

macx:INSTALLBASE = /Library/Frameworks
else:unix:INSTALLBASE = /usr/local
win32:INSTALLBASE = C:/Composer

DEFINES += COMPOSERCORE_LIBRARY

SOURCES += \
    src/modules/PluginControl.cpp \
    src/modules/TransactionControl.cpp \
    src/modules/LanguageControl.cpp \
    src/modules/DocumentControl.cpp \
    src/model/Entity.cpp \
    src/model/Document.cpp \
    src/util/Utilities.cpp

HEADERS_MAN += \
    include/modules/PluginControl.h \
    include/modules/TransactionControl.h \
    include/modules/LanguageControl.h \
    include/modules/DocumentControl.h

HEADERS_EXTENSIONS += \
    include/extensions/ILanguageProfile.h \
    include/extensions/IDocumentParser.h \
    include/extensions/IPlugin.h \
    include/extensions/IPluginFactory.h

HEADERS_UTIL += \
    include/util/ComposerCoreControl_global.h \
    include/util/Singleton.h \
    include/util/Utilities.h

HEADERS_EXCEPTION +=  include/model/exception/EntityNotFound.h \
    include/model/exception/ParentNotFound.h \
    include/model/exception/CreationFailed.h

HEADERS_MODEL += include/model/Entity.h \
    include/model/Document.h

HEADERS = $$HEADERS_MAN $$HEADERS_EXTENSIONS $$HEADERS_UTIL \
          $$HEADERS_EXCEPTION $$HEADERS_MODEL


headers_man.files = $$HEADERS_MAN
headers_extensions.files = $$HEADERS_EXTENSIONS
headers_util.files = $$HEADERS_UTIL
headers_exception.files = $$HEADERS_EXCEPTION
headers_model.files = $$HEADERS_MODEL

INCLUDEPATH += include

macx { 
    QMAKE_FRAMEWORK_BUNDLE_NAME = $$TARGET
    headers_man.path = core/modules
    headers_extensions.path = core/extensions
    headers_util.path = core/util
    headers_exception.path=core/model/exception
    headers_model.path = core/model
    QMAKE_BUNDLE_DATA += headers_man headers_extensions \
                         headers_util headers_exception headers_model
    QMAKE_BUNDLE_DATA.path = $$INSTALLBASE
    target.path = $$INSTALLBASE
    INSTALLS += target

}
else:unix {
    target.path = $$INSTALLBASE/lib/composer
    HEADERS_PATH_UNIX = $$INSTALLBASE/include/composer

    headers_model.path = $$HEADERS_PATH_UNIX/core/model
    headers_man.path = $$HEADERS_PATH_UNIX/core/modules
    headers_extensions.path = $$HEADERS_PATH_UNIX/core/extensions
    headers_util.path = $$HEADERS_PATH_UNIX/core/util
    headers_exception.path = $$HEADERS_PATH_UNIX/core/model/exception

    INSTALLS += headers_man headers_util headers_model \
                headers_extensions headers_exception target
}else:win32 {
    target.path = $$INSTALLBASE
    HEADERS_PATH_UNIX = $$INSTALLBASE/include/composer

    headers_model.path = $$HEADERS_PATH_UNIX/core/model
    headers_man.path = $$HEADERS_PATH_UNIX/core/modules
    headers_extensions.path = $$HEADERS_PATH_UNIX/core/extensions
    headers_util.path = $$HEADERS_PATH_UNIX/core/util
    headers_exception.path = $$HEADERS_PATH_UNIX/core/model/exception

    INSTALLS += headers_man headers_util headers_model \
                headers_extensions headers_exception target
}
