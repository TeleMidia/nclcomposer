#!/bin/bash

DIR=`pwd`; DIR="$DIR/../"; cd $DIR/composer-gui/;

echo "(1/4) ------ Setting ------";

BUNDLE=Composer.app

mkdir $BUNDLE/Contents/PlugIns
mkdir $BUNDLE/Contents/PlugIns/composer
    
echo "(2/4) ------ Copying plugins and data ------";

cp /Library/Application\ Support/Composer/Data/* $BUNDLE/Contents/PlugIns/composer
cp /Library/Application\ Support/Composer/Extensions/* $BUNDLE/Contents/PlugIns/composer

echo "(3/4) ------ Fixing libraries linkage ------";

for lib in $BUNDLE/Contents/PlugIns/composer/*; do
    LIB=`echo $lib | sed 's/.*\/\(.*\.dylib\)/\1/g'`

    install_name_tool -id @executable_path/../PlugIns/composer/$LIB $lib

    otool -L $lib | 

    while read line; do
        ORIGINAL=`echo $line | sed 's/ (.*)//g'`

        EDITED=`echo $line |
            sed 's/ (.*)//g' |
            sed 's/\/Library\/Application Support\/Composer\/Extensions/@executable_path\/\.\.\/PlugIns\/composer/g' |
            sed 's/\/opt\/local\/lib/@executable_path\/\.\.\/Frameworks/g' |
            sed 's/.*\/\([a-zA-Z]*\.framework\)/@executable_path\/\.\.\/Frameworks\/\1/g' |
            sed 's/ComposerCore.framework/@executable_path\/\.\.\/Frameworks\/ComposerCore.framework/g'`

    
        install_name_tool -change "$ORIGINAL" "$EDITED" $lib
    done
done

echo "(4/4) ------ Creating DMG ------";
    
macdeployqt $BUNDLE -dmg
