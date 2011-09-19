#!/bin/bash

DIR=Composer.app/Contents/PlugIns/composer/* 

for lib in $DIR; do
	LIB=`echo $lib |
             sed 's/Composer.app\/Contents\/PlugIns\/composer\/\(.*\.dylib\)/\1/g'`

        install_name_tool -id @executable_path/../PlugIns/composer/$LIB $lib

	echo $LIB

	otool -L $lib |	

	while read line; do
		ORIGINAL=`echo $line |
                	  sed 's/ (.*)//g'`
			  #sed 's/Application Support/Application\\\ Support/g'`

		EDITED=`echo $line |
			sed 's/ (.*)//g' |
			sed 's/\/Library\/Application Support\/Composer/@executable_path\/\.\.\/PlugIns\/composer/g' |
			sed 's/.*\/\([a-zA-Z]*\.framework\)/@executable_path\/\.\.\/Frameworks\/\1/g' |
			sed 's/ComposerCore.framework/@executable_path\/\.\.\/Frameworks\/ComposerCore.framework/g'`

	
		install_name_tool -change "$ORIGINAL" "$EDITED" $lib
	done
done
