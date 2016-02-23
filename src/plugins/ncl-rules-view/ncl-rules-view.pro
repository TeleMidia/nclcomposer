CONFIG += link_ncl_profile

include (../plugins-common.pri)

INCLUDEPATH += . \
            src

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
