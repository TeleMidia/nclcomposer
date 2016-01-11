#!/bin/bash

VERSION=$1

if [ -z $VERSION ]; then
	echo "create-release.sh: missing operand."
	echo "Try 'create-release.sh <VERSION>'."
	exit   
fi

DIR=`pwd`; DIR="$DIR/../"; cd $DIR;

echo "(1/3) ------ Uninstall ------";
echo "Uninstalling 'all'...";
sudo make uninstall 
echo;

echo "(2/3) ------ Clean ------";
echo "Cleaning 'all'...";
sudo make distclean 
echo;

echo "(3/3) ------ Install ------";
echo "Installing release version of 'all'...";
qmake -spec macx-g++ nclcomposer.pro FORCERELEASE=true RUNSSHON=false CPRVERSION=$VERSION
lupdate nclcomposer.pro 
lrelease nclcomposer.pro
sudo make
sudo make install  

echo Done.
