TEMPLATE = \
    subdirs

CONFIG += \
    ordered

SUBDIRS = \
    deps \
    outline-view \
    ncl-text-editor/deps/QScintilla-gpl-2.5.1/Qt4/qscintilla.pro \
    ncl-text-editor/ncl-textual-plugin.pro \
    debug-console \
    properties-view \
    qncllayout/qnly.pro \
    qnclstoryboard/qnsb.pro \
    qnclstructural/qnst.pro
