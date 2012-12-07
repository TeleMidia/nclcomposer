#!/bin/bash
set -e

#Getting current version
VERSION=`git --git-dir=./composer-plugins/.git describe --tags | sed "s/v//g"`

# I caan also FORCE a specific version
VERSION=0.1.4

QMAKE="qmake FORCERELEASE=true RUNSSH=true CPRVERSION=${VERSION} PREFIX=/usr"

#We need to install before to be able to compile the plugins.
${QMAKE} && make install

#Now, we start to generate the deb.
debuild -us -uc
debuild -S -us -uc
pbuilder build ../composer_${VERSION}.dsc

#Remove the previous installed NCL Composer
make uninstall && make clean && make distclean

#TODO: PUBLISH!!
