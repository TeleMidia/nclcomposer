#!/bin/bash
set -e

#We need to install before to be able to compile the plugins.
qmake FORCERELEASE=true PREFIX=/usr && make install

#Now, we start to generate the deb.
debuild -us -uc
debuild -S -us -uc
pbuilder build ../composer_0.1.3.dsc

#Remove the previous installed NCL Composer
make uninstall && make clean && make distclean

#TODO: PUBLISH!!
