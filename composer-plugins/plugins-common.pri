TEMPLATE    =   lib
CONFIG      +=  qt debug plugin

MOC_DIR     =   .moc
OBJECTS_DIR =   .obj
UI_DIR      =   .ui

macx {
    :   LIBS += -framework \
        ComposerCore
    INCLUDEPATH += include /Library/Frameworks/ComposerCore.framework/
    target.path = $$quote(/Library/Application Support/Composer)
}
else:unix {
    LIBS += -L/usr/local/lib/composer \
        -lComposerCore
    INCLUDEPATH += include /usr/local/include/composer
    target.path = $$quote(/usr/local/lib/composer/extension)
}
else:win32 {
    LIBS += -LC:/Composer/ -lComposerCore1
    INCLUDEPATH += . include C:/composer/include/composer
    target.path = $$quote(C:/composer/lib/composer/)
}
