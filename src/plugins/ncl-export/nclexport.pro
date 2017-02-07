include (../plugins-common.pri)

DEFINES += ncl-export_LIBRARY

INCLUDEPATH += \
  .

SOURCES += \
  nclExportFactory.cpp \
  nclExportPlugin.cpp

HEADERS += \
  nclExportFactory.h \
  nclExportPlugin.h \
  nclExport_global.h

RESOURCES += \
  images.qrc

OTHER_FILES += \
  nclexport.json

