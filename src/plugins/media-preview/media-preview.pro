include(../plugins-common.pri)

QT += phonon
QT += gui
QT += xml
CONFIG +=qtestlib

DEPENDPATH += .
INCLUDEPATH += . src

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
    src/MediaPreviewFactory.cpp \
    src/AudioPlayer.cpp \
    src/ImageView.cpp \
    src/GifView.cpp \
    src/TextView.cpp \
    # src/HypertextView.cpp \
    src/NCLView.cpp \
    src/LuaView.cpp \
    src/MediaPreviewPlugin.cpp \
    src/VideoPlayer.cpp

HEADERS += \
    src/MediaPreviewFactory.h \
    src/MediaPreviewPlugin.h \
    src/AudioPlayer.h \
    src/ImageView.h \
    src/GifView.h \
    src/TextView.h \
    # src/HypertextView.h \
    src/NCLView.h \
    src/LuaView.h \
    src/VideoPlayer.h

FORMS += \
    ui/VideoPlayer.ui

RESOURCES += \
    media-preview.qrc

OTHER_FILES += \
    media_preview_plugin.json

