include (../plugins-common.pri)

CONFiG += silent

QT       -= gui
QT       += xml

MOC_DIR = .mocs
OBJECTS_DIR = .objs

TARGET = NCLLanguageProfile

DEFINES += NCLLANGUAGEPROFILE_LIBRARY

mac {
    EXTENSIONS_DIR = $$quote(/Library/Application Support/Composer/Extensions)
    QMAKE_LFLAGS += -Wl,-install_name,'\'$$EXTENSIONS_DIR/lib'$$TARGET'.dylib\''
}

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

