#!/bin/bash
# add_header.sh -- Adds a header to all CPP/H files on a given path.
# This is a bash-only script.
#

PROGRAM_NAME='build.sh'
PROGRAM_VERSION='1.0'
AUTHORS='Roberto Gerson Azevedo'
BUGS_TO='robertogerson@telemidia.puc-rio.br'


files=`find . -path './composer-plugins/ncl-text-editor/deps' -prune -o -path './composer-plugins/content-provider' -prune -o -name '*.h' -or -name '*.cpp'`

#for each file add the head form head.conf
for file in $files; do
  echo $file
  
  # Test if the License HEAD is Already on File.
  if head $file -n 9 | grep "Eclipse Public License" >/dev/null; then 
    echo "... File Already with License"; 
    echo "... Removing previous License"; 
    sed -i '1,9d' $file
  fi 
  echo ".. Adding LICENSE HEADER."
  cat scripts/header_epl.conf >/tmp/file
  cat $file >> /tmp/file
  cp /tmp/file $file
done
