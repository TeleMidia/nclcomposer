CONFIG       += release

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

HEADERS       = mainwindow.h
SOURCES       = main.cpp \
                mainwindow.cpp
RESOURCES     = application.qrc
;INCLUDEPATH   = /usr/local/lib/composer/extensions
LIBS         += -L/usr/local/lib/composer/extensions \
	        -lqscintilla2
