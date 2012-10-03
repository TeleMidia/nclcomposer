include (../../plugins-common.pri)

#QT      +=  xml

VERSION  =  0.1.0

TARGET   =  Qnly

SOURCES +=  qnlycomposerplugin.cpp \
            qnlycomposerpluginfactory.cpp \
            core/model/qnlydocument.cpp \
            core/model/qnlyregion.cpp \
            core/model/qnlyregionbase.cpp \
            ui/view/qnlyview.cpp \
            ui/view/qnlygraphicsregion.cpp \
            ui/view/qnlygraphicsregionbase.cpp \
            ui/view/qnlycanvas.cpp \
    ui/view/qnlygraphicsgrid.cpp

HEADERS +=  qnlycomposerplugin.h \
            qnlycomposerpluginfactory.h \
            core/qnly.h \
            core/model/qnlydocument.h \
            core/model/qnlyregion.h \
            core/model/qnlyregionbase.h \
            ui/view/qnlyview.h \
            ui/view/qnlygraphicsregion.h \
            ui/view/qnlygraphicsregionbase.h \
            ui/view/qnlycanvas.h \
    ui/view/qnlygraphicsgrid.h

INSTALLS =  target
