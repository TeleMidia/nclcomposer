NCL Composer
============
This is NCL Composer. NCL Composer is an authoring tool for interactive
applications for Digital TV based on NCL (Nested Context Language). It provides
different views with graphical and textual abstractions that helps in
developing NCL applications.

Contributing
===========
  * NCL Composer Website (http://composer.telemidia.puc-rio.br)
  * Redmine link: (http://redmine.telemidia.puc-rio.br:8080/redmine/projects/composer3)


Code organization
=================
TODO
  * composer-core
  * composer-gui
  * composer-plugins


Building
========
TODO

Dependencies
============
The main dependency of NCL Composer is:
  * Qt >= 4.7.x

If you want to enable the Run your application on Ginga Virtual Set-top box,
then you should run qmake like this:
  * qmake RUNSSHON=true

By doing so, you have to be installed the following:
  * libssh2
  * libcrypt


Additionally, this project also brings some useful files and scripts related to
code documentation, like Doxyfile, scripts to add the License HEAD to files,
Today, there are three main submodules inside this project:
If you want specific information about one of the above subproject go to its
specfic README file.

Packaging
=========
This repository also provides the necessary files for packaging NCL Composer.

* Generating MSI (Windows installer) package
  To generate the Windows Package installer, you will need the NSIS (Nullsoft
  scriptable install system): http://nsis.sourceforge.net/

  To generate the windows installer, got to win32-nsis directory and run:

  $ qmake
  $ mingw32-make install
  $ makensis composer.nsis

* Generating DEBIAN package
  Before generate the debian package you must update the debian files, inside 
  the debian directory.

  To generate a new debian package, the only thing you need to do is run:
  
  $ ./scripts/deb-package.sh
  
  in your terminal.
  This will generate a composer_x.x.x.deb file one directory behind the root NCL Composer 
  directory. This package is what you need to install on other debian/ubuntu systems.

  You can install it, using dpkg, for example:

  $ dpkg -i composer_x.x.x.deb

//TODO: Mac OS X ???

//TODO: LICENSE

