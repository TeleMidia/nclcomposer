#!/bin/bash

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
sudo make install  

echo "Installing release version of 'gui'...";
cd $DIR/composer-gui
qmake -spec macx-g++ Composer.pro FORCERELEASE=true RUNSSH_ON=true CPRVERSION=0.1.4 
lupdate Composer.pro 
lrelease Composer.pro
sudo make install  

echo "Installing release version of 'plugins'...";
cd $DIR/composer-plugins
qmake -spec macx-g++ plugin-suite.pro FORCERELEASE=true CPRVERSION=0.1.4
lupdate plugin-suite.pro
lrelease plugin-suite.pro
sudo make install  
echo;

echo Done.
