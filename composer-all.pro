TEMPLATE = subdirs

!exists(composer-core/.git) {
  system (git submodule update --init)
  system (git submodule foreach \"git checkout master\")
}

SUBDIRS = composer-core \
				  composer-plugins/plugin-suite.pro \
					composer-gui/Composer.pro

