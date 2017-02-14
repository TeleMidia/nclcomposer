include(../plugins-common.pri)

QT += phonon
QT += gui
QT += xml
CONFIG +=qtestlib

DEPENDPATH += .
INCLUDEPATH += . 

release {
LIBS      +=    -L../ncl-textual-view/deps/QScintilla-gpl-2.8/Qt4Qt5/release \
                -lqscintilla2_telem
}else:debug {
LIBS      +=    -L../ncl-textual-view/deps/QScintilla-gpl-2.8/Qt4Qt5/debug \
                -lqscintilla2_telem
}

CODECFORTR = UTF-8

# Input
SOURCES += \
    MediaPreviewFactory.cpp \
    AudioPlayer.cpp \
    ImageView.cpp \
    GifView.cpp \
    TextView.cpp \
    # HypertextView.cpp \
    NCLView.cpp \
    LuaView.cpp \
    MediaPreviewPlugin.cpp \
    VideoPlayer.cpp

HEADERS += \
    MediaPreviewFactory.h \
    MediaPreviewPlugin.h \
    AudioPlayer.h \
    ImageView.h \
    GifView.h \
    TextView.h \
    # HypertextView.h \
    NCLView.h \
    LuaView.h \
    VideoPlayer.h

FORMS += \
    VideoPlayer.ui

RESOURCES += \
    media-preview.qrc

OTHER_FILES += \
    media_preview_plugin.json

