#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T10:04:13
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WizardEngine2
TEMPLATE = app


SOURCES += main.cpp\
        xmetawizardmainwindow.cpp \
    templateparser.cpp \
    wizardgenerator.cpp \
    pdpwriter.cpp \
    wswriter.cpp \
    selectsparser.cpp \
    wizardexecutionengine.cpp \
    xwizardpage.cpp \
    eleminput.cpp \
    attrinput.cpp \
    defaultattrinput.cpp \
    fileattrinput.cpp \
    wizardchooser.cpp

HEADERS  += xmetawizardmainwindow.h \
    templateparser.h \
    constants.h \
    wizardgenerator.h \
    pdpwriter.h \
    wswriter.h \
    selectsparser.h \
    wizardexecutionengine.h \
    xwizardpage.h \
    eleminput.h \
    attrinput.h \
    defaultattrinput.h \
    fileattrinput.h \
    wizardchooser.h

FORMS    += xmetawizardmainwindow.ui \
    wizardchooser.ui

RESOURCES += \
    resources.qrc
