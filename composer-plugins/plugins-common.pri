TEMPLATE    =   lib
CONFIG      +=  qt debug plugin

MOC_DIR     =   .moc
OBJECTS_DIR =   .obj
UI_DIR      =   .ui

macx:LOCATION = /Library/Frameworks
else:unix:LOCATION = /usr/local
else:win32:LOCATION = C:/Composer

macx:INSTALLBASE = /Applications/Composer
else:unix:INSTALLBASE = /usr/local
win32:INSTALLBASE = C:/Composer

macx {
    :   LIBS += -framework \
                ComposerCore

        LIBS +=  $$quote(-L/Library/Application Support/Composer)

        INCLUDEPATH +=  include /Library/Frameworks/ComposerCore.framework/ \
                       /Library/Frameworks/ComposerCore.framework/core \
                    /Library/Frameworks/ComposerCore.framework/core/extensions

        target.path = $$quote(/Library/Application Support/Composer)
}
else:unix {
    LIBS += -L/usr/local/lib/composer -lNCLLanguageProfile \
            -L/usr/local/lib/composer/extension

    INCLUDEPATH += include /usr/local/include/composer \
                    /usr/local/include/composer/core \
                   /usr/local/include/composer/core/extensions

    target.path = $$quote(/usr/local/lib/composer/extension)
}
else:win32 {
    LIBS += -L$$LOCATION \
            -lComposerCore1 \
            -L$$LOCATION/lib/composer \
            -L$$LOCATION/lib/composer/extensions

    INCLUDEPATH += . include $$LOCATION/include/composer \
                   $$LOCATION/include/composer/core \
                   $$LOCATION/include/composer/core/extensions

    target.path = $$LOCATION/lib/composer
}
