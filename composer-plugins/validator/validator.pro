#-------------------------------------------------
#
# Project created by QtCreator 2011-10-11T11:55:41
#
#-------------------------------------------------

include (../plugins-common.pri)

INCLUDEPATH += "/usr/lib/"

LIBS += -lre2 -lxerces-c

TARGET = ValidatorPlugin

INSTALLS += target

SOURCES += \
    ValidatorPlugin.cpp \
    ValidatorFactory.cpp \
    langstruct/langstruct.cpp \
    langstruct/components/ReferenceStructure.cpp \
    langstruct/components/ElementStructure.cpp \
    langstruct/components/AttributeStructure.cpp \
    message/message.cpp \
    nclmodel/nclmodel.cpp \
    validation/Validator.cpp \
    composerncladapter.cpp \
    validation/semanticvalidation.cpp \
    validation/structuralvalidation.cpp \
    validatortreewidgetitem.cpp \
    nclmodel/textualparser.cpp

HEADERS  += \
    ValidatorPlugin.h \
    ValidatorFactory.h \
    definitions/definitions.h \
    langstruct/langstruct.h \
    langstruct/components/ReferenceStructure.h \
    langstruct/components/ElementStructure.h \
    langstruct/components/AttributeStructure.h \
    message/message.h \
    nclmodel/nclmodel.h \
    validation/Validator.h \
    composerncladapter.h \
    validation/semanticvalidation.h \
    validation/structuralvalidation.h \
    validatortreewidgetitem.h \
    nclmodel/textualparser.h

OTHER_FILES +=

RESOURCES += \
    Resource.qrc
