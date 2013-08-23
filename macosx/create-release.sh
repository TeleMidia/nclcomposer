#!/bin/bash

VERSION=$1

if [ -z $VERSION ]; then
	echo "create-release.sh: missing operand."
	echo "Try 'create-release.sh <VERSION>'."
	exit   
fi

DIR=`pwd`; DIR="$DIR/../"; cd $DIR;

echo "(1/4) ------ Uninstall ------";
echo "Uninstalling 'core'...";
cd $DIR/composer-core 
sudo make uninstall 
echo "Uninstalling 'gui'...";
cd $DIR/composer-gui
sudo make uninstall 
echo "Uninstalling 'plugins'...";
cd $DIR/composer-plugins
sudo make uninstall 
echo;

echo "(2/4) ------ Clean ------";
echo "Cleaning 'core'...";
cd $DIR/composer-core
sudo make distclean 
echo "Cleaning 'gui'...";
cd $DIR/composer-gui
sudo make ditclean 
sudo rm -Rf Composer.app
echo "Cleaning 'plugins'...";
cd $DIR/composer-plugins
sudo make distclean 
echo;

echo "(3/4) ------ Update ------";
echo "Updating 'core'...";
cd $DIR/composer-core
git pull 

echo "Cleaning 'gui'...";
cd $DIR/composer-gui
git pull 
echo "Cleaning 'plugins'...";
cd $DIR/composer-plugins
git pull  
echo;

echo "(4/4) ------ Install ------";

echo "Installing release version of 'core'...";
cd $DIR/composer-core
qmake -spec macx-g++ composer-core.pro FORCERELEASE=true
sudo make
sudo make install  

echo "Installing release version of 'gui'...";
cd $DIR/composer-gui
qmake -spec macx-g++ Composer.pro FORCERELEASE=true RUNSSHON=true CPRVERSION=$VERSION
lupdate Composer.pro 
lrelease Composer.pro
sudo make
sudo make install  

echo "Installing release version of 'plugins'...";
cd $DIR/composer-plugins
qmake -spec macx-g++ plugin-suite.pro FORCERELEASE=true CPRVERSION=$VERSION
lupdate plugin-suite.pro
lrelease plugin-suite.pro
sudo make
sudo make install  
echo;

echo Done.
