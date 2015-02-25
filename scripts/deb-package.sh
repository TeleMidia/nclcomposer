#!/bin/bash
set -e

#Getting current version
VERSION=`git --git-dir=./composer-plugins/.git describe --tags | sed "s/v//g"`

# I can also FORCE a specific version from command line
if [ $# -eq 1 ]
  then
    VERSION=$1
fi

echo "Generating NCL Composer ${VERSION} deb package"

PARAMS="FORCERELEASE=true RUNSSH=true CPRVERSION=${VERSION} PREFIX=/usr"
QMAKE="qmake-qt4 -recursive ${PARAMS}"
LUPDATE="lupdate-qt4 nclcomposer.pro"
LRELEASE="lrelease-qt4 nclcomposer.pro"

#We need to install before to be able to compile the plugins.
${QMAKE} && ${LUPDATE} && ${LRELEASE} && make install

#Now, we start to generate the deb.
debuild -us -uc
debuild -S -us -uc
pbuilder build ../composer_${VERSION}.dsc

#Remove the previous installed NCL Composer
make uninstall && make clean && make distclean

#TODO: PUBLISH!!

