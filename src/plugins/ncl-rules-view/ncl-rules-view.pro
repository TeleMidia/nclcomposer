CONFIG += link_ncl_profile

include (../plugins-common.pri)

INCLUDEPATH += .

SOURCES += \
    ComboBoxItem.cpp \
    CompositeRuleItem.cpp \
    RuleItem.cpp \
    RulesTreeWidget.cpp \
    RulesView.cpp \
    RulesViewFactory.cpp \
    RulesViewPlugin.cpp

HEADERS += \
    Util.h \
    RulesViewPlugin.h \
    RulesViewFactory.h \
    RulesView.h \
    RulesTreeWidget.h \
    RuleItem.h \
    CompositeRuleItem.h \
    ComboBoxItem.h

RESOURCES += \
    ../common-resources/images.qrc
