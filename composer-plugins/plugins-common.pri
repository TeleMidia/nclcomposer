TEMPLATE    =   lib
CONFIG      +=  qt debug plugin

MOC_DIR     =   .moc
OBJECTS_DIR =   .obj
UI_DIR      =   .ui

macx {
    :   LIBS += -framework \
        ComposerCore \
        -L$$quote(/Library/Application Support/Composer) \
        -lNCLLanguageProfile
        INCLUDEPATH += include /Library/Frameworks/ComposerCore.framework/ \
                       /Library/Frameworks/ComposerCore.framework/core \
                       /Library/Frameworks/ComposerCore.framework/core/extensions
        target.path = $$quote(/Library/Application Support/Composer)
}
else:unix {
    LIBS += -L/usr/local/lib/composer -lNCLLanguageProfile \
        -lComposerCore -L/usr/local/lib/composer/extension

    INCLUDEPATH += include /usr/local/include/composer \
                    /usr/local/include/composer/core \
                   /usr/local/include/composer/core/extensions
    target.path = $$quote(/usr/local/lib/composer/extension)
}
else:win32 {
    LIBS += -LC:/Composer/ -lComposerCore1
    INCLUDEPATH += . include C:/composer/include/composer \
                    C:/Composer/include/composer/core \
                    C:/Composer/include/composer/core/extensions
    target.path = $$quote(C:/composer/lib/composer/)
}
