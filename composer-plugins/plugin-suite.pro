TEMPLATE = \
    subdirs

CONFIG += \
    ordered

SUBDIRS = \
    outline-view \
    ncl-textual-view/deps/QScintilla-gpl-2.8/Qt4Qt5/qscintilla.pro \
    ncl-textual-view/ncl-textual-view.pro \
    debug-console \
    properties-view \
    ncl-layout-view \
    ncl-structural-view \
    qnclstructural/deps/libqncg/libqncg.pro \
    qnclstructural/qnst.pro \
    validator/validator.pro \
    ncl-rules-view \
    media-preview/mediapreview.pro
#   clube-ncl

contains(FORCERELEASE, true)
{
  SUBDIRS += validator      # Validator is required when releasing.
  SUBDIRS -= debug-console  # Debug must not be present in release.

  # These plug-ins are still experimental
  #  (and must not be present in the release):
  SUBDIRS -= media-preview/mediapreview.pro
  SUBDIRS -= nclstructural-20/nclstructural.pro
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
