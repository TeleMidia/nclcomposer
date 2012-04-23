TEMPLATE = \
    subdirs

CONFIG += \
    ordered

SUBDIRS = \
    outline-view \
    ncl-text-editor/deps/QScintilla-gpl-2.6.1/Qt4/qscintilla.pro \
    ncl-text-editor/ncl-textual-plugin.pro \
    debug-console \
    properties-view \
    qncllayout/qnly.pro \
    qnclstructural/deps/libqncg/libqncg.pro \
    qnclstructural/qnst.pro
#    media-preview/mediapreview.pro
#    clube-ncl
#    validator

TRANSLATIONS = \
    translations/plugin_suite_es.ts \
    translations/plugin_suite_pt_BR.ts
