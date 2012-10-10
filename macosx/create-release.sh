#!/bin/bash

DIR=`pwd`; DIR="$DIR/../"; cd $DIR;

if [[ $1 = "-v"  ]]; then
    VERBOSE=`&> /dev/null`
fi;


echo "(1/4) ------ Uninstall ------";
echo "Uninstalling 'core'...";
cd $DIR/composer-core 
sudo make uninstall $VERBOSE
echo "Uninstalling 'gui'...";
cd $DIR/composer-gui
sudo make uninstall $VERBOSE
echo "Uninstalling 'plugins'...";
cd $DIR/composer-plugins
sudo make uninstall $VERBOSE
echo;

echo "(2/4) ------ Clean ------";
echo "Cleaning 'core'...";
cd $DIR/composer-core
sudo make distclean $VERBOSE
echo "Cleaning 'gui'...";
cd $DIR/composer-gui
sudo make ditclean $VERBOSE
echo "Cleaning 'plugins'...";
cd $DIR/composer-plugins
sudo make distclean $VERBOSE
echo;

echo "(3/4) ------ Update ------";
echo "Updating 'core'...";
cd $DIR/composer-core
git pull $VERBOSE

echo "Cleaning 'gui'...";
cd $DIR/composer-gui
git pull $VERBOSE
echo "Cleaning 'plugins'...";
cd $DIR/composer-plugins
git pull $VERBOSE 
echo;

echo "(4/4) ------ Install ------";
echo "Installing release version of 'core'...";
cd $DIR/composer-core
qmake -spec macx-g++ FORCERELEASE=true $VERBOSE 
make $VERBOSE 
sudo make install $VERBOSE 
echo "Installing release version of 'gui'...";
cd $DIR/composer-gui
qmake -spec macx-g++ FORCERELEASE=true $VERBOSE 
make $VERBOSE 
sudo make install $VERBOSE 
echo "Installing release version of 'plugins'...";
cd $DIR/composer-plugins
qmake -spec macx-g++ FORCERELEASE=true $VERBOSE 
make $VERBOSE 
sudo make install $VERBOSE 
echo;
