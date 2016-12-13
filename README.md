![Some screenshots](http://composer.telemidia.puc-rio.br/_media/screenshot/nclcomposer.png)

# NCL Composer
[NCL Composer](http://composer.telemidia.puc-rio.br) is a multiplataform and
flexible authoring tool for creating Interactive multimedia applications for
Digital TV based on NCL ([Nested Context Language](http://www.ncl.org.br)).
It provides a minimal core and different views (implemented as plug-ins)
supporting textual and graphical abstractions.

## Main features
  * Advanced textual editing supporting code coloring, autocomplete, error
    marking, etc.
  * Validation of NCL documents
  * Graphical layout definition
  * Structural view of NCL documents
  * Integration with different players (e.g. [Ginga-NCL Reference
    Implementation](http://www.ginga.org.br))
  * Easily extensible through plug-ins
  * Multi-platform (Linux, Windows, and MacOS X officially supported)

## Source code organization
NCL Composer source code is divided into three main parts:
  * __src/core__: implements a minimal core that allows the loading and
    communication between plug-ins.
  * __src/gui__: implements the GUI engine that is responsible for grouping the
    different views in the same user interface.
  * __src/plugins__: brings the official plug-ins supported by NCL Composer. An
    special type of plug-in are the views.

## Contribute!

  * TODO

## Building

  * TODO

### Requirements
The main requirement for NCL Composer is:

  * Qt >= 5.4

### Linux (Debian/Ubuntu)
In a Debian-based system (e.g. Debian, Ubuntu, or Linux Mint) you can easily
install Qt libraries through:

    $ apt-get install libqtbase5-dev libqttool5-dev

After installing the dependencies and cloning the repository, you can compile
NCL Composer by entering in the nclcomposer root directory and typing:

    $ qmake
    $ make

If everything is alright with the compilation process you should be able to
find the executable (and run):

    $ ./bin/debug/nclcomposer

> If you want build a release version of the NCL Composer executable and plugins,
> you can call `qmake FORCERELEASE=true`.  The executable will be available at
> `./bin/release/nclcomposer`.

### Windows
  * TODO

### Mac OS X
  * TODO

## Packaging NCL Composer
This repository also provides the necessary files for packaging NCL Composer.

### MSI (Windows installer) package
To generate the Windows Package installer, you will need the NSIS ([Nullsoft 
scriptable install system](http://nsis.sourceforge.net/)).

To generate the windows installer, go to `win32-nsis` directory and run:

    $ makensis nclcomposer.nsis

### Generating DEBIAN package
To generate a new debian package, the only thing you need to do is run:
  
    $ ./scripts/deb-package.sh
  
in your terminal.

This will generate a `nclcomposer_x.x.x.deb` file one directory behind the root
NCL Composer directory. This package all you need to install NCL Composer on 
other Debian/Ubuntu systems.

You can install it, using dpkg, for example:

    $ dpkg -i nclcomposer_x.x.x.deb

### Mac OS X

  * TODO

## Authors and contributors
  * Roberto Azevedo <robertogerson@telemidia.puc-rio.br>
  * Eduardo Araújo <edcaraujo@telemidia.puc-rio.br>

### Previous contributors
  * Marcelo Moreno <moreno@telemidia.puc-rio.br>
  * Bruno Lima <bslima@telemidia.puc-rio.br>

## License

NCL Composer is available through
[LGPLv3](http://www.gnu.org/licenses/lgpl-3.0.html).

## Further information
  * NCL Composer Official Website (http://composer.telemidia.puc-rio.br)
  * Redmine link:
    (http://redmine.telemidia.puc-rio.br:8080/redmine/projects/composer3)

## Acknowledgments

NCL Composer would not be possible without many other open source software we
use.  In special:

  * [Qt](http://qt.io) for multi-plataform, gui, etc.
  * [QScintilla](https://riverbankcomputing.com/software/qscintilla/intro) and
    [Scintilla](http://www.scintilla.org/) for textual editing features.
  * [QToolMainWindow](https://github.com/Riateche/toolwindowmanager): for a
    beautiful and easy-to-use organization of the interface.
  * [Graphviz](http://www.graphviz.org): for automatically layout of nodes in
    structural view.


