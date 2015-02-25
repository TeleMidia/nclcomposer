include (../plugins-common.pri)

CONFiG += silent

QT       -= gui
QT       += xml

MOC_DIR = .mocs
OBJECTS_DIR = .objs

TARGET = NCLLanguageProfile
TEMPLATE = lib
CONFIG += plugin dll

DEFINES += NCLLANGUAGEPROFILE_LIBRARY

SOURCES += \
    NCLLanguageProfile.cpp \
    NCLDocumentParser.cpp \
    NCLStructure.cpp

HEADERS +=  \
    NCLLanguageProfile.h \
    NCLLanguageProfile_global.h \
    NCLDocumentParser.h \
    NCLStructure.h

headers_nclprofile.files = $$HEADERS

headers_nclprofile.path = $$INSTALLBASE/include/composer/extensions

OTHER_FILES += \
    nclLanguageProfile.json \
    nclLanguageProfile.json

INSTALLS += headers_nclprofile
