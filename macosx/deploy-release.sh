#!/bin/bash

DIR=`pwd`; DIR="$DIR/../"; cd $DIR/bin/;

echo "(1/4) ------ Setting ------";

BUNDLE=Composer.app

sudo mkdir $BUNDLE/Contents/PlugIns
sudo mkdir $BUNDLE/Contents/PlugIns/composer
    
echo "(2/4) ------ Copying plugins and data ------";

sudo cp /Library/Application\ Support/Composer/Data/* $BUNDLE/Contents/PlugIns/composer
sudo cp /Library/Application\ Support/Composer/Extensions/* $BUNDLE/Contents/PlugIns/composer

echo "(3/4) ------ Fixing libraries linkage ------";

for lib in $BUNDLE/Contents/PlugIns/composer/*; do
    LIB=`echo $lib | sed 's/.*\/\(.*\.dylib\)/\1/g'`

    sudo install_name_tool -id @executable_path/../PlugIns/composer/$LIB $lib

    sudo otool -L $lib | 

    while read line; do
        ORIGINAL=`echo $line | sed 's/ (.*)//g'`

        EDITED=`echo $line |
            sed 's/ (.*)//g' |
            sed 's/\/Library\/Application Support\/Composer\/Extensions/@executable_path\/\.\.\/PlugIns\/composer/g' |
            sed 's/\/opt\/local\/lib/@executable_path\/\.\.\/Frameworks/g' |
            sed 's/.*\/\([a-zA-Z]*\.framework\)/@executable_path\/\.\.\/Frameworks\/\1/g' |
            sed 's/ComposerCore.framework/@executable_path\/\.\.\/Frameworks\/ComposerCore.framework/g'`

    
        sudo install_name_tool -change "$ORIGINAL" "$EDITED" $lib
    done
done

echo "(4/4) ------ Creating DMG ------";
    
sudo macdeployqt $BUNDLE -dmg
