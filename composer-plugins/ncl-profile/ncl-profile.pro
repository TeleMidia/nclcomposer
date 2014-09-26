include (../plugins-common.pri)

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

message ($$INSTALLBASE)
headers_nclprofile.path = $$INSTALLBASE/include/composer/extensions
message ($$headers_nclprofile.files)

OTHER_FILES += \
    nclLanguageProfile.json \
    nclLanguageProfile.json

INSTALLS += target headers_nclprofile
