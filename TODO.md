TODO file for NCL Composer
==========================
Copyright (C) 2011-2017 PUC-Rio/Laboratorio Telemidia

TODO file for nclcomposer.  This is a long-term view on features that can be
implemented in nclcomposer.  For a bug tracking and short-term features, please
see the [github issues](https://github.com/TeleMidia/nclcomposer/issues).

NCL Composer Core
-----------------
  * New features:
    - Generalize NCLStructure for all kinds of languages.
    - ToolGUI integration API.
    - Transaction support (plug-in could emit N messages as a single
      transaction).
    - Separate each plug-in as a different process.

  * Optimizations and clean up

  * Tests:
    - A tool for testing plug-in API:
      . Performance tests.
      . Memory leaks (valgrind)
        . Security??
        . Integration with other plug-ins?

  * Documentation:
    - Add a section in the manual on how to develop plug-ins.
    - Points to be mentioned:
      . Key handling (pass to the father).

NCL Composer GUI
----------------
  * New features:
    - Some defaults:
      . Structural, Textual and Layout default perspectives.
    - Message to add new buttons to title bar of composer dock.
    - Keybinding to save new perspective.
    - Keybinding to open "choose perpective menu".
    - Allow to choose MDI or Dock enviroment.
      - Include save the current plugins session inside the project.
    - Check for updates (on going).
    - Plugin repository.
    - Install new Plugins.
    - Run features:
      . Local Run
      . Remote Run
        . Check md5sum for local and remote files and send it again only if they
          are differents.

NCL Composer Plug-ins
---------------------
### General
  * New features:
    - Try to keep the element/attribute order in the core.
    - Refactory the API to have Plugin/View instead of Plugin/Factory.
    - Script to automatically create the Plugin and Factory classes.
    - Copy and paste among plugins.
    - Undo/Redo
       * Should it be a new plugin?

  * Optimizations and cleanup:
    - All views should inherent from a common class (Composer View?!)

  * Known bugs:

### Debug Console

  * New features:
    - Add a window that describe in more details the message from core.

  * Optimizations and cleanup:

  * Known bugs:

### Textual View

  * New features:
    -  Contextual code suggestion:
      . Sugestion of default attribute values. (ok)
      . Sugestion of references. (ok)
      . Suggestion of references based on scope.
    - Code format(Ctrl+Shift+F). (This is done when the text is synchronized
      with core).
    - Split the Textual View Window; thus, allowing to see different parts of
      the text in the same moment (this should be simple).
    - Suggestion of code corrections.
    - Hyperlinks navigation.
    - Preview of media objects, connectors, etc.
    - Preferences Widget:
      . Default font size.
      . Font family.
      . Font colors.
      . Whitespace visibility.
    - Incremental synchronization with composer core.
      . xml simple diff based on elements id (ok).
      . diffx algorithm.

  * Optimizations and cleanup:
    - Code refactoring:
      . Rename NCLTextEditor to NCLTextView ?!
      . Rename NCLTreeWidget to NCLTreeView ?!
     . Improve attribute navigation performance.

  * Known bugs:

### Outline View

  * New features:
    - Update from model.

  * Optimizations and cleanup:

  * Known bugs:

### Properties View

  * New features:
    - Specific controls for each type of attribute.
    - Suggestion of attribute values.

  * Optimization and cleanup:

  * Known bugs:

### Layout View

  * New features:
    - Plugin preferences.
    - Support regions specified in pixels.
    - Add color to regions (based on media type).
    - Draw region base id.

  * Optimization and cleanup:

  * Known bugs:

### Structural View

  * New features:
    - Add hide/show system (as in layout plugin).
    - Display link option based on core document.
    - Add full preview for easy navigation.
    - Prevent addition of more than one body item.

  * Optimization and cleanup:

  * Known bugs:

### Validation Plug-in

  * New features:
    - Message to ask for Validator re-send all the error messages.

  * Optimization and cleanup:

  * Known bugs:

### New Plug-ins

  * Global Actions.
  * Lua Scripting plugin.
  * Storeboard plugin.
  * Temporal view plugin.
  * Add support for authoring multi-device apps.
  * A plugin to handle CPR project:
    . Two subview:
      . Project
      . Files
    . Some actions:
      . Add media
      . Add file
      . etc.

//TODO: License
