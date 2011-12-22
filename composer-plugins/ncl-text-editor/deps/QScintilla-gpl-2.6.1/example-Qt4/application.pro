CONFIG       += release
HEADERS       = mainwindow.h
SOURCES       = main.cpp \
                mainwindow.cpp
RESOURCES     = application.qrc
;INCLUDEPATH   = /usr/local/lib/composer/extensions
LIBS         += -L/usr/local/lib/composer/extensions \
	        -lqscintilla2
