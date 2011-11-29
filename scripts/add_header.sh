#!/bin/bash
# add_header.sh -- Adds a header to all CPP/H files on a given path.
# This is a bash-only script.
#
PROGRAM_NAME='build.sh'
PROGRAM_VERSION='1.0'
PWD=`pwd`
AUTHORS='Roberto Gerson Azevedo'
BUGS_TO='robertogerson@telemidia.puc-rio.br'

ISTEST=0         # You should turn this variable to 0 when really want to apply
                 # the changes.
FIND_PATH=$1     # Where to find the files?
LICENSE_FILE=$2  # Which header content to put in the file?
                 # This must be a text file

#if license file doesn't exist do nothing!
if [ -f $LICENSE_FILE ]; then
  echo "LICENSE FILE DOESN'T EXIST!"
  exit
fi

files=`find $1 -path '*ncl-text-editor/deps' -prune -o -path \
      '*content-provider' -prune -o -path '*composer-gui/deps' -prune -o -name '*.h' -or -name '*.cpp'`


#for each file add the head form head.conf
for file in $files; do
  echo $file
  
  # Test if the License HEAD is Already on the file.
  if head $file -n 9 | grep "Eclipse Public License" >/dev/null; then 
    echo "... File Already with EPL License"; 
    echo "... Removing previous EPL License";
    if [ $ISTEST -eq 0 ]; then
      sed -i '1,9d' $file
    fi
  elif head $file -n 9 | grep "" >/dev/null; then
    echo "... File Already with LGPL License"
    echo "... Removing LGPL License"
    if [ $ISTEST -eq 0 ]; then
      sed -i '1,17d' $file
    fi
  fi
  if [ $ISTEST -eq 0 ]; then   
    echo ".. Adding $LICENSE_FILE LICENSE HEADER."
    cat ./scripts/$LICENSE_FILE >/tmp/file
    cat $file >> /tmp/file
    cp /tmp/file $file
  fi
done

