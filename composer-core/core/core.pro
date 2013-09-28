# -------------------------------------------------
# Project created by QtCreator 2010-03-12T13:06:44
# -------------------------------------------------

QT += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
TARGET = ComposerCore

VERSION = 1.0
CONFIG += dll
MOC_DIR = .mocs
OBJECTS_DIR = .objs

# Uses FORCERELEASE variable because CONFIG and SUBDIR force three executions
# if qmake and the last one does not preserves CONFIG from command line.
contains(FORCERELEASE, true) {
  CONFIG += qt warn_on release
  CONFIG -= debug
  DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG_WARNING
  message ("core.pro RELEASE build!")
}
else {
  CONFIG += qt warn_on debug console
  CONFIG -= release
  message ("core.pro DEBUG build!")
}

contains(NO_DEBUG_OUTPUT, true) {
    DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_DEBUG_WARNING
}

macx:CONFIG += lib_bundle

macx {
  INSTALLBASE = /Library/Frameworks
}
else:unix {
 isEmpty(PREFIX) {
    PREFIX = /usr/local
  } 
  INSTALLBASE = $$PREFIX
}
else:win32 {
  INSTALLBASE = C:/Composer
}

DEFINES += COMPOSERCORE_LIBRARY

SOURCES += \
    src/modules/PluginControl.cpp \
    src/modules/MessageControl.cpp \
    src/modules/LanguageControl.cpp \
    src/modules/ProjectControl.cpp \
    src/model/Entity.cpp \
    src/model/Project.cpp \
    src/util/Utilities.cpp \
    src/util/AttributeReferences.cpp \
    src/modules/ProjectReader.cpp \
    src/util/ComposerSettings.cpp \
    src/util/Commands.cpp
    # src/model/Model.cpp

HEADERS_MAN += \
    include/modules/PluginControl.h \
    include/modules/MessageControl.h \
    include/modules/LanguageControl.h \
    include/modules/ProjectControl.h \
    include/modules/ProjectReader.h

HEADERS_EXTENSIONS += \
    include/extensions/ILanguageProfile.h \
    include/extensions/IDocumentParser.h \
    include/extensions/IPlugin.h \
    include/extensions/IPluginFactory.h

HEADERS_UTIL += \
    include/util/ComposerCoreControl_global.h \
    include/util/Singleton.h \
    include/util/AttributeReferences.h \
    include/util/Utilities.h \
    include/util/ComposerSettings.h \
    include/util/Commands.h

HEADERS_EXCEPTION +=  include/model/exception/EntityNotFound.h \
    include/model/exception/ParentNotFound.h \
    include/model/exception/CreationFailed.h

HEADERS_MODEL += include/model/Entity.h \
    include/model/Project.h
    #include/model/Model.h

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
