TEMPLATE = subdirs

!exists(composer-core/.git) {
	message ("It appears that you don't have the submodules source code.")
	message ("I will automatically try to init, update and checkout each submodule.")
	system (git submodule update --init)
  system (git submodule foreach \"git checkout master\")
}

SUBDIRS = composer-core \
				  composer-plugins/plugin-suite.pro \
					composer-gui/Composer.pro

