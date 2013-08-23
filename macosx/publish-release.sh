#!/bin/bash

VERSION=$1

if [ -z $VERSION ]; then
	echo "create-release.sh: missing operand."
	echo "Try 'publish-release.sh <VERSION>'."
	exit   
fi

DIR=`pwd`; DIR="$DIR/../"; cd $DIR/composer-gui/;

echo "(1/3) ------ Setting ------";

BUNDLE=Composer.dmg
NAME="nclcomposer-"$VERSION"-"`date '+%Y%m%d'`".dmg"

cp $BUNDLE $NAME
 
echo "(2/3) ------ Pulishing ------";

scp $NAME edcaraujo@xserve1.telemidia.puc-rio.br:/Library/WebServer/Documents/composer/downloads/unstable

echo "(3/3) ------ Cleaning ------";

rm -Rf $NAME