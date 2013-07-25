TEMPLATE = \
    subdirs

CONFIG += \
    ordered

SUBDIRS = \
    outline-view \
    ncl-text-editor/deps/QScintilla-gpl-2.7.2/Qt4Qt5/qscintilla.pro \
    ncl-text-editor/ncl-textual-plugin.pro \
    debug-console \
    properties-view \
    qncllayout/qnly.pro \
    qnclstructural/deps/libqncg/libqncg.pro \
    qnclstructural/qnst.pro \
    media-preview/mediapreview.pro
#   clube-ncl

contains(FORCERELEASE, true)
{
  SUBDIRS += validator      # Validator is required when releasing.
  SUBDIRS -= debug-console  # Debug must not be present in release.
  SUBDIRS -= media-preview/mediapreview.pro # It is still experimental.
}

macx {
  INSTALLBASE = /Applications/Composer
}
else:unix {
  isEmpty(PREFIX) {
    PREFIX = /usr/local
  }
  INSTALLBASE = $$PREFIX
}
else:win32 {
  INSTALLBASE = "C:/Composer"
}

#TRANSLATIONS
win32 {
    trans.path = $$INSTALLBASE/extensions

} else:macx{
    trans.path = "/Library/Application Support/Composer/Extensions"

}else:unix {
    trans.path = $$INSTALLBASE/lib/composer/extensions
}

trans.files = translations/*.qm

isEmpty(trans.path) {
    trans.path = $(QTDIR)/translations
}

TRANSLATIONS = \
    translations/plugin_suite_es_ES.ts \
    translations/plugin_suite_pt_BR.ts

INSTALLS += trans

RESOURCES += \
    common-resources/images.qrc
