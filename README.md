![Some screenshots](https://raw.githubusercontent.com/TeleMidia/nclcomposer/master/docs/sphinx/img/nclcomposer.png)

<p align="center">
  <a href="https://semaphoreci.com/robertogerson/nclcomposer"><img src="https://semaphoreci.com/api/v1/robertogerson/nclcomposer/branches/master/shields_badge.svg" alt="Build Status (Linux)" title="Build Status (Linux)"></a>
  <a href="https://ci.appveyor.com/project/robertogerson/nclcomposer">
    <img src="https://ci.appveyor.com/api/projects/status/uq1btj2acxf2eqxi?svg=true" alt="Build Status (Windows)" title="Build Status (Windows)">
  </a>
  <a href='http://ncl-composer-manual.readthedocs.io/en/latest/?badge=latest'>
    <img src='https://readthedocs.org/projects/ncl-composer-manual/badge/?version=latest' alt='Documentation Status' />
  </a>
</p>

# NCL Composer
[NCL Composer](http://composer.telemidia.puc-rio.br) is a multiplataform and
flexible authoring tool for creating interactive multimedia applications for
Digital TV based on NCL ([Nested Context Language](http://www.ncl.org.br)).
It provides a minimal core and different views (implemented as plug-ins)
supporting textual and graphical abstractions.

## Main features
  * Advanced textual editing, such as code coloring, autocomplete, and error
    marking
  * Validation of NCL documents
  * WYSISWG definition of the application layout
  * Structure viewing/edition of NCL documents
  * Integration with different players (e.g. [Ginga-NCL Reference
    Implementation](http://www.ginga.org.br))
  * Multi-platform (Linux, Windows, and MacOS X are officially supported)
  * Easily extended through plug-ins

## Source code organization
NCL Composer source code is divided into three main parts:
  * __src/core__: a minimal core that supports loading and communication
    between plug-ins.
  * __src/gui__: the GUI engine, responsible for grouping the different plugins
    in the same user interface.
  * __src/plugins__: the official plug-ins supported by NCL Composer.


## Building

### Requirements
The main requirement for building NCL Composer is:

  * cmake >= 3.2
  * Qt >= 5.4

### Linux
In a Debian-based system (e.g. Debian, Ubuntu, or Linux Mint) you can easily
install Qt libraries through:

    $ apt-get install libqtbase5-dev libqttool5-dev

After installing the dependencies and cloning the NCL Composer's repository,
you can compile NCL Composer by (in the nclcomposer's root directory):

    $ mkdir _build && cd _build
    $ cmake ..
    $ make

If everything is alright with the compilation process you should be able to
find the executable (and run):

    $ ./_build/bin/nclcomposer

## Main authors and contributors
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

NCL Composer would not be possible without other open source software we use.
In special, we would like to acknowledge:

  * [Qt](http://qt.io) for multi-plataform, gui, etc.
  * [QScintilla](https://riverbankcomputing.com/software/qscintilla/intro) and
    [Scintilla](http://www.scintilla.org/) for textual editing features.
  * [QToolMainWindow](https://github.com/Riateche/toolwindowmanager): for a
    beautiful and easy-to-use organization of the interface.
  * [Graphviz](http://www.graphviz.org): for automatically layout of nodes in
    structural view.

