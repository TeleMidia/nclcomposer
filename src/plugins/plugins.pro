TEMPLATE = \
    subdirs

# Handling dependencies between plug-ins
outline-view.depends = ncl-profile

qscintilla2_telem.file = ncl-textual-view/deps/QScintilla-gpl-2.9/Qt4Qt5/qscintilla.pro
ncl-textual-view.file = ncl-textual-view/ncl-textual-view.pro
ncl-textual-view.depends = ncl-profile qscintilla2_telem

properties-view.depends = ncl-profile
ncl-layout-view.depends = ncl-profile
ncl-structural-view.depends = ncl-profile
validator.depends = ncl-profile
ncl-rules-view.depends = ncl-profile
media-preview.depends = ncl_profile

# Add plug-ins to subdirectory
SUBDIRS = \
    ncl-profile \
    outline-view \
    qscintilla2_telem \
    ncl-textual-view \
    debug-console \
    properties-view \
    ncl-layout-view \
    ncl-structural-view \
    validator \
    ncl-rules-view \
    media-preview
#   clube-ncl

# Uncomment the next two line to enable 'next' build as default option
#CONFIG += \
#  next

next {
  SUBDIRS -= \
    ncl-structural-view

  SUBDIRS += \
    ncl-structural-view-next/ncl-structural-view-next.pro
}

contains(FORCERELEASE, true)
{
  SUBDIRS -= debug-console  # Debug must not be present in release.

  # These plug-ins are still experimental
  #  (and must not be present in the release):
  SUBDIRS -= media-preview
  SUBDIRS -= ncl-structural-view-next
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
