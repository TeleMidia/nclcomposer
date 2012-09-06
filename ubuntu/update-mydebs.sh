#! /bin/bash
 dpkg-scanpackages . /dev/null | gzip -9c > Packages.gz
