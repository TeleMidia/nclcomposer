include (../plugins-common.pri)

INCLUDEPATH += . \
            src

macx:LIBS +=    -L$$quote(/Library/Application Support/Composer/Extensions) \
                -lNCLLanguageProfile

win32:LIBS +=   -lNCLLanguageProfile

SOURCES += \
    src/ComboBoxItem.cpp \
    src/CompositeRuleItem.cpp \
    src/RuleItem.cpp \
    src/RulesTreeWidget.cpp \
    src/RulesView.cpp \
    src/RulesViewFactory.cpp \
    src/RulesViewPlugin.cpp

HEADERS += \
    src/Util.h \
    src/RulesViewPlugin.h \
    src/RulesViewFactory.h \
    src/RulesView.h \
    src/RulesTreeWidget.h \
    src/RuleItem.h \
    src/CompositeRuleItem.h \
    src/ComboBoxItem.h

RESOURCES += \
    ../common-resources/images.qrc
