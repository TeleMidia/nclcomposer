#!/bin/bash
files=`find . -path './composer-plugins/ncl-text-editor/deps' -prune -o -path './composer-plugins/content-provider' -prune -o -name '*.h' -or -name '*.cpp'`

#for each file add the head form head.conf
for file in $files; do
  echo $file
#  sed -i '9d' $file
  cat scripts/head.conf >/tmp/file
  cat $file >> /tmp/file
  cp /tmp/file $file
done
