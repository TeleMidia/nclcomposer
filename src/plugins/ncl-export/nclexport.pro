include (../plugins-common.pri)

DEFINES += ncl-export_LIBRARY

INCLUDEPATH += \
  src

SOURCES += \
    src/nclExportFactory.cpp \
    src/nclExportPlugin.cpp

HEADERS += \
    src/nclExportFactory.h \
    src/nclExportPlugin.h \
    src/nclExport_global.h

RESOURCES += \
  images.qrc

OTHER_FILES += \
    nclexport.json
