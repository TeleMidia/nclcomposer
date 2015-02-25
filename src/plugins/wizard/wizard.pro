#-------------------------------------------------
#
# Project created by QtCreator 2013-07-02T10:04:13
#
#-------------------------------------------------

QT       += core gui xml webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = wizardEngine

TEMPLATE = app

INCLUDEPATH += . \
            src

SOURCES += src/main.cpp\
    src/AttrInput.cpp \
    src/ComboAttrInput.cpp \
    src/DefaultAttrInput.cpp \
    src/ElemInput.cpp \
    src/FileAttrInput.cpp \
    src/PDPWriter.cpp \
    src/SelectsParser.cpp \
    src/TemplateParser.cpp \
    src/WizardChooser.cpp \
    src/WizardExecutionEngine.cpp \
    src/WizardGenerator.cpp \
    src/WSWriter.cpp \
    src/XMetaWizardMainWindow.cpp \
    src/XWizardPage.cpp

HEADERS  += \
    src/AttrInput.h \
    src/ComboAttrInput.h \
    src/Constants.h \
    src/DefaultAttrInput.h \
    src/ElemInput.h \
    src/FileAttrInput.h \
    src/PDPWriter.h \
    src/TemplateParser.h \
    src/WizardChooser.h \
    src/WizardExecutionEngine.h \
    src/WizardGenerator.h \
    src/WSWriter.h \
    src/XMetaWizardMainWindow.h \
    src/XWizardPage.h \
    src/SelectsParser.h

FORMS    += ui/xmetawizardmainwindow.ui \
    ui/wizardchooser.ui

RESOURCES += \
    wizard.qrc
