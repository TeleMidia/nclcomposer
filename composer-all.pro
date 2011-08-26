TEMPLATE = subdirs

unix {
  system (git submodule init)
  system (git submodule update)
}

SUBDIRS = composer-core \
				  composer-plugins/plugin-suite.pro \
					composer-gui/Composer.pro

