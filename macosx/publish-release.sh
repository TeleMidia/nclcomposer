#!/bin/bash

VERSION=$1

if [ -z $VERSION ]; then
	echo "create-release.sh: missing operand."
	echo "Try 'publish-release.sh <VERSION>'."
	exit   
fi

DIR=`pwd`; DIR="$DIR/../"; cd $DIR/bin/release/;

echo "(1/3) ------ Setting ------";

BUNDLE=Composer.dmg
NAME=nclcomposer-$VERSION-`date '+%Y%m%d'`.dmg

cp $BUNDLE $NAME
 
echo "(2/3) ------ Pulishing ------";

scp $NAME edcaraujo@telemidia.puc-rio.br:/Library/WebServer/Documents/composer/downloads/unstable
ssh edcaraujo@telemidia.puc-rio.br "chmod 777 /Library/WebServer/Documents/composer/downloads/unstable/$NAME" 

echo "(3/3) ------ Cleaning ------";

rm -Rf $NAME
