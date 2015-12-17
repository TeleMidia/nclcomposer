TEMPLATE = subdirs

src_core.subdir = src/core
src_plugin_suite.subdir = src/plugins
src_gui.subdir = src/gui

src_plugin_suite.depends = src_core
src_gui.depends = src_core

SUBDIRS = src_core \
          src_plugin_suite \
          src_gui

