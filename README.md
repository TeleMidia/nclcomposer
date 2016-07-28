![Some screenshots](http://composer.telemidia.puc-rio.br/_media/screenshot/nclcomposer.png)

# NCL Composer
[NCL Composer](http://composer.telemidia.puc-rio.br) is a multiplataform and
flexible authoring tool for creating Interactive multimedia applications for
Digital TV based on NCL ([Nested Context Language](http://www.ncl.org.br)).
It provides a minimal core and different views (implemented as plug-ins)
supporting textual and graphical abstractions.

## Features
  * Textual editing supporting code coloring, autocomplete, error marking,
    etc.
  * Validation of NCL documents
  * Graphical layout definition
  * Structural view of NCL documents
  * Integration with different players (e.g. [Ginga-NCL Reference
    Implemenation](http://www.ginga.org.br))
  * Easily extensible through a plug-in API
  * Multi-platform (Linux, Windows, and Mac officially supported)

## Screenshots


## Source code organization
NCL Composer source code is divided into three main parts:
  * __src/core__: implements a minimal core that allows the loading and
    communication between plug-ins.
  * __src/gui__: implements the GUI engine that is responsible for grouping the
    different views in the same user interface.
  * __src/plugins__: brings the official plug-ins supported by NCL Composer. An
    special type of plug-in are the views.

## Contributing

## Building

### Requirements
The main requirement for NCL Composer is:
  * Qt >= 4.7.x

If you want to enable the Run your application on Ginga Virtual Set-top box,
then you should run qmake like this:
  * qmake RUNSSHON=true

By doing so, you have to be installed the following:
  * libssh2
  * libcrypt

### Linux (Debian/Ubuntu)
  * TODO

### Windows
  * TODO

### Mac OS X
  * TODO

<!-- Additionally, this project also brings some useful files and scripts
related to code documentation, like Doxyfile, scripts to add the License HEAD
to files, Today, there are three main submodules inside this project: If you
want specific information about one of the above subproject go to its specfic
README file. -->

## Packaging
This repository also provides the necessary files for packaging NCL Composer.

### MSI (Windows installer) package
To generate the Windows Package installer, you will need the NSIS ([Nullsoft 
scriptable install system](http://nsis.sourceforge.net/).

To generate the windows installer, go to `win32-nsis` directory and run:

    $ makensis composer.nsis

### Generating DEBIAN package
Before generate the debian package you must update the debian files, inside the
debian directory.

To generate a new debian package, the only thing you need to do is run:
  
  ./scripts/deb-package.sh
  
in your terminal.

This will generate a `composer_x.x.x.deb` file one directory behind the root
NCL Composer directory. This package is what you need to install on other
Debian/Ubuntu systems.

You can install it, using dpkg, for example:

    $ dpkg -i composer_x.x.x.deb

### Mac OS X

  * TODO

## Authors and contributors
  * Roberto Azevedo (robertogerson@telemidia.puc-rio.br)
  * Eduardo Araújo (edcaraujo@telemidia.puc-rio.br)
  * Bruno Lima (bslima@telemidia.puc-rio.br)

## License

NCL Composer is available through
[LGPLv3](http://www.gnu.org/licenses/lgpl-3.0.html).

## Further information
  * NCL Composer Official Website (http://composer.telemidia.puc-rio.br)
  * Redmine link:
    (http://redmine.telemidia.puc-rio.br:8080/redmine/projects/composer3)

